#include "../Logger.h"
#include "../Headers/AutoTileset.h"
#include "../Headers/StaticTileset.h"
#include "../Headers/TilesetSystem.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;
/**
*	Standard Constructor.
*/
TilesetSystem::TilesetSystem()
	: _tilesets(0) {

		Logger *logger = Logger::instance();
		logger->addMessage("Created an tileset system");
		logger->setStatus(true);
}

/**
*	Standard destructor.
*	Destroys all registered tilesets.
*/
TilesetSystem::~TilesetSystem() {
	for (int i = 0; i < _tilesets.size(); i++) {
		Tileset *it = _tilesets.at(i);

		if (!!it)
			delete it;
	}

	_tilesets.clear();
}

/**
*	Get all registered Tilesets.
*/
const QVector<Tileset*>& TilesetSystem::getTilesets() const {
	return _tilesets;
}

/**
*	Get a tileset by name <i>name</i>.
*	If no matching tileset can be found, a null-pointer is returned.
*/
Tileset* TilesetSystem::getTileset(QString name) const {
	if (name == QString())
		throw ProjectException("Cannot search for an tileset with an empty name.");

	return getComponent<Tileset>(_tilesets, name);
}

/**
*	Add a tileset to the system.
*/
void TilesetSystem::addTileset(Tileset *tileset) {
	if (!tileset)
		throw ProjectException("Cannot add a null tileset.");

	if (!!getTileset(tileset->getName()))
		throw ProjectException("A tileset with the same name already exists.");

	_tilesets.append(tileset);

	Logger *logger = Logger::instance();
	logger->addMessage("Added tileset " + tileset->getName() + " to the project");
	logger->setStatus(true);
}

/**
*	Remove the tileset with name <i>name</i> from the system.
*/
void TilesetSystem::removeTileset(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for an tileset with an empty name.");

	Tileset *tileset = removeComponentAndLog<Tileset>(_tilesets, name, "tileset", "project");

	if (!!tileset)
		delete tileset;
}

/**
*	Create an instance of TilesetSystem.
*/
TilesetSystem* TilesetSystem::create() {
	return new TilesetSystem;
}

/**
*	Translate all tilset elements from the XML-document into the appropriate type of Tileset.
*/
TilesetSystem* TilesetSystem::translateFromXML(QDomElement &element) {
	TilesetSystem *system = create();

	QDomNodeList tilesetElements = element.childNodes();

	for (int i = 0; i < tilesetElements.size(); i++) {
		QDomElement it = tilesetElements.at(i).toElement();
		Tileset *tileset;
		QString type = it.firstChildElement("type").attributeNode("value").value();
		if (type == "STATIC")
			tileset = StaticTileset::translateFromXML(it);
		else if (type == "AUTO")
			tileset = AutoTileset::translateFromXML(it);
		else
			throw ProjectException("Tileset has unknown type: " + type);

		system->addTileset(tileset);
	}

	return system;
}

/**
*	Go over all tilesets and let them be translated into XML.
*/
void TilesetSystem::translateToXML(TilesetSystem *system, QDomDocument &document, QDomElement &parent) {
	QDomElement systemElement = document.createElement("tilesets");

	const QVector<Tileset*> tilesets = system->getTilesets();
	for (int i = 0; i < tilesets.size(); i++) {
		Tileset *it = tilesets.at(i);
		switch (it->getType()) {
		case Tileset::STATIC:
			StaticTileset::translateToXML(dynamic_cast<StaticTileset*>(it), document, systemElement);
			break;
		case Tileset::AUTO:
			AutoTileset::translateToXML(dynamic_cast<AutoTileset*>(it), document, systemElement);
			break;
		}
	}

	parent.appendChild(systemElement);
}