#include "../Logger.h"
#include "../Headers/EnvironmentSystem.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

EnvironmentSystem::EnvironmentSystem()
	: _environments(0) {

		Logger *logger = Logger::instance();
		logger->addMessage("Created an environment system");
		logger->setStatus(true);
}

EnvironmentSystem::~EnvironmentSystem() {
	for (int i = 0; i < _environments.size(); i++) {
		Environment *it = _environments.at(i);

		if (!!it)
			delete it;
	}

	_environments.clear();
}

const QVector<Environment*>& EnvironmentSystem::getEnvironments() const {
	return _environments;
}

Environment* EnvironmentSystem::getEnvironment(QString name) const {
	if (name == QString())
		throw ProjectException("Cannot search for an environment with an empty name.");

	return getComponent<Environment>(_environments, name);
}

void EnvironmentSystem::addEnvironment(Environment *environment) {
	if (!environment)
		throw ProjectException("Cannot add a null environment.");

	_environments.append(environment);

	Logger *logger = Logger::instance();
	logger->addMessage("Added environment " + environment->getName() + " to the project");
	logger->setStatus(true);
}

void EnvironmentSystem::removeEnvironment(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for an environment with an empty name.");

	Environment *environment = removeComponentAndLog<Environment>(_environments, name, "environment", "project");

	if (!!environment)
		delete environment;
}

EnvironmentSystem* EnvironmentSystem::create() {
	return new EnvironmentSystem;
}

EnvironmentSystem* EnvironmentSystem::translateFromXML(QDomElement &element) {
	EnvironmentSystem *system = create();

	QDomNodeList environmentElements = element.childNodes();

	for (int i = 0; i < environmentElements.size(); i++) {
		QDomElement environmentElement = environmentElements.at(i).toElement();
		system->addEnvironment(Environment::translateFromXML(environmentElement));
	}

	return system;
}

void EnvironmentSystem::translateToXML(EnvironmentSystem *system, QDomDocument &document, QDomElement &parent) {
	QDomElement systemElement = document.createElement("environments");

	const QVector<Environment*> environments = system->getEnvironments();
	for (int i = 0; i < environments.size(); i++) {
		Environment::translateToXML(environments.at(i), document, systemElement);
	}

	parent.appendChild(systemElement);
}