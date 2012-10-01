#include "../Logger.h"
#include "../Headers/ObjectSystem.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

/**
*	Standard constructor.
*/
ObjectSystem::ObjectSystem()
	: _objects(0) {

		Logger *logger = Logger::instance();
		logger->addMessage("Created an object system");
		logger->setStatus(true);
}

/**
*	Standard destructor.
*/
ObjectSystem::~ObjectSystem() {
	for (int i = 0; i < _objects.size(); i++) {
		Object *it = _objects.at(i);

		if (!!it)
			delete it;
	}

	_objects.clear();
}

/**
*	Get objects.
*/
const QVector<Object*>& ObjectSystem::getObjects() const {
	return _objects;
}

/**
*	Get object object with specified name <i>name</i>.
*/
Object* ObjectSystem::getObject(QString name) const {
	if (name == QString())
		throw ProjectException("Cannot search for an object with an empty name.");

	return getComponent<Object>(_objects, name);
}

/**
*	Add object to the system.
*/
void ObjectSystem::addObject(Object *object) {
	if (!object)
		throw ProjectException("Cannot add a null object.");

	_objects.append(object);

	Logger *logger = Logger::instance();
	logger->addMessage("Added object " + object->getName() + " to the project");
	logger->setStatus(true);
}

/**
*	Remove object with specified name from the system.
*/
void ObjectSystem::removeObject(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for an object with an empty name.");

	Object *object = removeComponentAndLog<Object>(_objects, name, "object", "project");

	if (!!object)
		delete object;
}

/**
*	Create the object system and return a pointer to it.
*/
ObjectSystem* ObjectSystem::create() {
	return new ObjectSystem;
}

/**
*	Create and translate an object system from XML and return a pointer to it.
*/
ObjectSystem* ObjectSystem::translateFromXML(QDomElement &element) {
	ObjectSystem *system = create();

	QDomNodeList objectElements = element.childNodes();

	for (int i = 0; i < objectElements.size(); i++) {
		QDomElement objectElement = objectElements.at(i).toElement();
		system->addObject(Object::translateFromXML(objectElement));
	}

	return system;
}

/**
*	Translate the object system and append it to the XML-document.
*/
void ObjectSystem::translateToXML(ObjectSystem *system, QDomDocument &document, QDomElement &parent) {
	QDomElement systemElement = document.createElement("objects");

	QVector<Object*> objects = system->getObjects();
	for (int i = 0; i < objects.size(); i++) {
		Object::translateToXML(objects.at(i), document, systemElement);
	}

	parent.appendChild(systemElement);
}