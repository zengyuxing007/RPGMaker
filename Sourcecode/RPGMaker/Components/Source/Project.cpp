#include "../Logger.h"
#include "../Headers/Project.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

/**
*	Path of the currently opened project.
*/
QString Project::currentProjectPath = QString();

/**
* Construct a Project with title <i>title</i>.
*/
Project::Project(QString title)
	: _title(title),
	_properties(new Properties()),
	_characters(0),
	_enemies(0),
	_battlegroups(0),
	_events(0),
	_objects(0),
	_environments(0),
	_start(0) {

		setCharacters(CharacterSystem::create());
		setEnemies(EnemySystem::create());
		setBattlegroups(BattlegroupSystem::create());
		setEvents(EventSystem::create());
		setObjects(ObjectSystem::create());
		setEnvironments(EnvironmentSystem::create());
		setStartEvent(StartEvent::create());

		Logger *logger = Logger::instance();
		logger->setStatus(true);
		logger->addMessage("Created project " + _title);
}

/**
*	Standard destructor.
*/
Project::Project(const Project&) {}

/**
*	Assignment operator...
*/
Project& Project::operator=(const Project&) {
	return *this;
}

/**
*	Destroys the entire project.
*/
Project::~Project() {
	delete _start;
	delete _environments;
	delete _objects;
	delete _events;
	delete _battlegroups;
	delete _enemies;
	delete _characters;
	delete _properties;

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Destroyed project " + _title);
}

/**
*	Returns the title of the project.
*/
QString Project::getTitle() const {
	return this->_title;
}

/**
*	Returns the project properties.
*/
Properties* Project::getProperties() const {
	return this->_properties;
}

/**
*	Returns the event management system.
*/
EventSystem* Project::getEvents() const {
	return _events;
}

/**
*	Returns the enemy management system.
*/
EnemySystem* Project::getEnemies() const {
	return _enemies;
}

/**
*	Returns the object management system.
*/
ObjectSystem* Project::getObjects() const {
	return _objects;
}

/**
*	Returns the character management system.
*/
CharacterSystem* Project::getCharacters() const {
	return _characters;
}

/**
*	Returns the battlegroup management system.
*/
BattlegroupSystem* Project::getBattlegroups() const {
	return _battlegroups;
}

/**
*	Returns the environment management system.
*/
EnvironmentSystem* Project::getEnvironments() const {
	return _environments;
}

/**
*	Returns the start event.
*/
StartEvent* Project::getStartEvent() const {
	return _start;
}

/**
*	Set the title of the project.
*/
void Project::setTitle(QString title) {
	if (title == QString())
		throw ProjectException("A project should not have an empty title.");

	this->_title = title;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed title of project to " + _title);
}

/**
*	Set the event management system.
*/
void Project::setEvents(EventSystem *events) {
	if (!events)
		throw ProjectException("Cannot set a null event system.");

	if (!!_events)
		delete _events;

	_events = events;
}

/**
*	Set the enemy management system.
*/
void Project::setEnemies(EnemySystem* enemies) {
	if (!enemies)
		throw ProjectException("Cannot set a null enemies system.");

	if (!!_enemies)
		delete _enemies;

	_enemies = enemies;
}

/**
*	Set the object management system.
*/
void Project::setObjects(ObjectSystem *objects) {
	if (!objects)
		throw ProjectException("Cannot set a null objects system.");

	if (!!_objects)
		delete _objects;

	_objects = objects;
}

/**
*	Set the character  management system.
*/
void Project::setCharacters(CharacterSystem *characters) {
	if (!characters)
		throw ProjectException("Cannot set a null characters system.");

	if (!!_characters)
		delete _characters;

	_characters = characters;
}

/**
*	Set the battlegroup management system.
*/
void Project::setBattlegroups(BattlegroupSystem *battlegroups) {
	if (!battlegroups)
		throw ProjectException("Cannot set a null battlegroups system.");

	if (!!_battlegroups)
		delete _battlegroups;

	_battlegroups = battlegroups;
}

/**
*	Set the environment management system.
*/
void Project::setEnvironments(EnvironmentSystem *environments) {
	if (!environments)
		throw ProjectException("Cannot set a null environments system.");

	if (!!_environments)
		delete _environments;

	_environments = environments;
}

/**
*	Set the start event.
*/
void Project::setStartEvent(StartEvent *event) {
	if (!event)
		throw ProjectException("Cannot set a null starting event.");

	if (!!_start)
		delete _start;

	_start = event;

	Logger *logger = Logger::instance();
	logger->addMessage("Placed a new starting event");
	logger->setStatus(true);
}

/**
*	Create a project with the given title <i>title</i> and return a pointer to it.
*/
Project* Project::create(QString title) {
	if (title == QString())
		throw ProjectException("A project should not have an empty title");

	return new Project(title);
}

/**
*	Translate a project from XML and return a pointer to the instance of that project.
*/
Project* Project::translateFromXML(QDomElement &root) {
	QString title = root.attributeNode("title").value();
	if (title == QString())
		throw ProjectException("A project should not have an empty name.");

	Project *project = Project::create(title);
	project->_properties = Properties::translateFromXML(root.firstChildElement("properties"));

	// Events
	EventSystem *events = EventSystem::translateFromXML(root.firstChildElement("events"));
	project->setEvents(events);

	// Enemies
	EnemySystem *enemies = EnemySystem::translateFromXML(root.firstChildElement("enemies"));
	project->setEnemies(enemies);

	// Objects
	ObjectSystem *objects = ObjectSystem::translateFromXML(root.firstChildElement("objects"));
	project->setObjects(objects);

	// Characters
	CharacterSystem *characters = CharacterSystem::translateFromXML(root.firstChildElement("characters"));
	project->setCharacters(characters);

	// Battlegroups
	BattlegroupSystem *battlegroups = BattlegroupSystem::translateFromXML(root.firstChildElement("battlegroups"));
	project->setBattlegroups(battlegroups);

	// Environments
	EnvironmentSystem *environments = EnvironmentSystem::translateFromXML(root.firstChildElement("environments"));
	project->setEnvironments(environments);

	return project;
}

/**
*	Translate the project to XML and add the project to the XML-document.
*/
void Project::translateToXML(Project *project, QDomDocument &document) {
	QDomElement root = document.createElement("project");
	root.setAttribute("title", project->_title);

	Properties::translateToXML(project->_properties, document, root);

	// Characters
	CharacterSystem::translateToXML(project->getCharacters(), document, root);

	// Enemies
	EnemySystem::translateToXML(project->getEnemies(), document, root);

	// Battlegroups
	BattlegroupSystem::translateToXML(project->getBattlegroups(), document, root);

	// Objects
	ObjectSystem::translateToXML(project->getObjects(), document, root);

	// Events
	EventSystem::translateToXML(project->getEvents(), document, root);

	// Environments
	EnvironmentSystem::translateToXML(project->getEnvironments(), document, root);

	// Start event
	StartEvent::translateToXML(project->getStartEvent(), document, root);

	document.appendChild(root);
}