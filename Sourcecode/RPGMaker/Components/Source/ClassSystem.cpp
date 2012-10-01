#include "../Logger.h"
#include "../Headers/ClassSystem.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

ClassSystem::ClassSystem()
	: _classes(0) {

		Logger *logger = Logger::instance();
		logger->addMessage("Created an cat system");
		logger->setStatus(true);
}

ClassSystem::~ClassSystem() {
	for (int i = 0; i < _classes.size(); i++) {
		Class *it = _classes.at(i);

		if (!!it)
			delete it;
	}

	_classes.clear();
}

const QVector<Class*>& ClassSystem::getClasses() const {
	return _classes;
}

Class* ClassSystem::getClass(QString name) const {
	if (name == QString())
		throw ProjectException("Cannot search for an cat with an empty name.");

	return getComponent<Class>(_classes, name);
}

void ClassSystem::addClass(Class *cat) {
	if (!cat)
		throw ProjectException("Cannot add a null cat.");

	_classes.append(cat);
	emit addedClass(cat);

	Logger *logger = Logger::instance();
	logger->addMessage("Added cat " + cat->getName() + " to the project");
	logger->setStatus(true);
}

void ClassSystem::removeClass(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for an cat with an empty name.");

	Class *cat = removeComponentAndLog(_classes, name, "cat", "project");

	if (!!cat)
		delete cat;
}

ClassSystem* ClassSystem::create() {
	return new ClassSystem;
}

ClassSystem* ClassSystem::translateFromXML(QDomElement &element) {
	ClassSystem *system = create();

	QDomNodeList catElements = element.childNodes();

	for (int i = 0; i < catElements.size(); i++) {
		QDomElement catElement = catElements.at(i).toElement();
		system->addClass(Class::translateFromXML(catElement));
	}

	return system;
}

void ClassSystem::translateToXML(ClassSystem *system, QDomDocument &document, QDomElement &parent) {
	QDomElement systemElement = document.createElement("classes");

	const QVector<Class*> classes = system->getClasses();
	for (int i = 0; i < classes.size(); i++) {
		Class::translateToXML(classes.at(i), document, systemElement);
	}

	parent.appendChild(systemElement);
}

void ClassSystem::addedAttribute(Attribute *attribute) {
	for (int i = 0; i < _classes.size(); i++)
		_classes.at(i)->addAttribute(attribute, 1, 1.0);
}