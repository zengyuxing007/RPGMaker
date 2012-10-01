#include "qevent.h"
#include "qapplication.h"
#include "qsizepolicy.h"

#include "../ProgramDefaults.h"
#include "../Headers/PlayModule.h"
#include "../Headers/EnvironmentScene.h"

const QSize resolution = QSize(810, 605);

using namespace playmodule;

PlayModule::PlayModule(QWidget *parent, Project *project)
	: QMainWindow(parent), _project(project), _view(new GameView(0, project)) {
		setWindowModality(Qt::ApplicationModal);
		setWindowTitle("RPGMaker - " + project->getTitle());
		setFixedSize(resolution.width(), resolution.height());
		setCentralWidget(_view);

		//qApp->installEventFilter(this);
}

PlayModule::~PlayModule() {}

void PlayModule::play() {
	StartEvent *event = _project->getStartEvent();
	EnvironmentScene *scene = _view->getEnvironmentScene();
	scene->initialize(buildGameEvents(), event->getLocation().first, event->getParty().at(0), scene->translateCellCoordinates(event->getLocation().second));

	_view->setParty(new Party(event->getParty()));
	_view->show();
}

bool PlayModule::eventFilter(QObject *o, QEvent *e) {
	if(e->type()==QEvent::KeyPress)
	{
		qWarning()<<"The bad guy which steals the keyevent is"<<o;
	}
	return false;
}

PlayModule* PlayModule::create(QWidget *parent, Project *project) {

	if (!project)
		throw PlayException("Cannot initialize a game with an empty project.");

	checkPlayability(project);

	return new PlayModule(parent, project);
}

bool PlayModule::checkPlayability(Project *project) {
	if (!project)
		throw PlayException("Cannot check on a null project.");

	// Check attributes
	{
		AttributeSystem *attributeSystem = project->getProperties()->getAttributes();
		if (!attributeSystem)
			throw PlayException("No attributes exist. Make sure the game has at least a set of standard attributes.");

		if ((attributeSystem->getOffensiveAttributes().size() == 0) ||(attributeSystem->getDefensiveAttributes().size() == 0))
			throw PlayException("At least one offensive and one defensive attribute have to defined.");

		QVector<Attribute*> offensive = project->getProperties()->getAttributes()->getOffensiveAttributes();
		for (int i = 0; i < offensive.size(); i++) {
			if (!offensive.at(i)->getLink())
				throw PlayException("Offensive attribute " + offensive.at(i)->getName() + " is not linked to a defensive attribute.\nEvery offensive attribute should be linked to a defensive attribute.");
		}
	}

	// Check abilities
	{
		QVector<Ability*> abilities = project->getProperties()->getAbilities()->getAbilities();
		for (int i = 0; i < abilities.size(); i++) {
			Ability *it = abilities.at(i);
			if (!!it && !it->getAttribute().first)
				throw PlayException("The ability " + it->getName() + " has no attribute.\nAn ability should be linked to an attribute.");
		}
	}

	// Check classes
	{
		QVector<Class*> classes = project->getProperties()->getClasses()->getClasses();
		if (classes.size() == 0)
			throw PlayException("At least one class has to be defined.");

		for (int i = 0; i < classes.size(); i++) {
			Class *it = classes.at(i);
			if (!!it) {
				if (it->getAbilities().size() == 0)
					throw PlayException("The class " + it->getName() + " has no abilities.\nA class should at least have one ability.");

				if (!it->getPrimaryAttribute())
					throw PlayException("The class " + it->getName() + " has no primary attribute.\nEvery class should have a primary offensive attribute.");
			}	
		}
	}

	// Equipment
	{
		QVector<Weapon*> weapons = project->getProperties()->getEquipment()->getWeapons();
		for (int i = 0; i < weapons.size(); i++) {
			Weapon *it = weapons.at(i);
			if (!!it && (it->getAttributes().size() == 0))
				throw PlayException("A weapon should at least enhance one attribute");

			if (!!it && (it->getClasses().size() == 0))
				throw PlayException("A weapon should be able to be equipped by at least one class.");
		}

		QVector<Armor*> armor = project->getProperties()->getEquipment()->getArmor();
		for (int i = 0; i < armor.size(); i++) {
			Armor *it = armor.at(i);
			if (!!it && (it->getAttributes().size() == 0))
				throw PlayException("Armor should at least enhance one attribute");

			if (!!it && (it->getClasses().size() == 0))
				throw PlayException("Armor should be able to be equipped by at least one class.");
		}

		QVector<Accessory*> accessories = project->getProperties()->getEquipment()->getAccessories();
		for (int i = 0; i < accessories.size(); i++) {
			Accessory *it = accessories.at(i);
			if (!!it && (it->getAttributes().size() == 0))
				throw PlayException("An accessory should at least enhance one attribute");

			if (!!it && (it->getClasses().size() == 0))
				throw PlayException("An accessory should be able to be equipped by at least one class.");
		}
	}

	// Characters
	{
		QVector<Character*> characters = project->getCharacters()->getCharacters();
		if (characters.size() == 0)
			throw PlayException("At least one character has to be created.");

		for (int i = 0; i < characters.size(); i++) {
			Character *it = characters.at(i);
			if (!!it && !it->getClass())
				throw PlayException("Every character should have a class.");
		}
	}

	// Enemies
	{
		QVector<Enemy*> enemies = project->getEnemies()->getEnemies();
		
		for (int i = 0; i < enemies.size(); i++) {
			Enemy *it = enemies.at(i);
			if (!!it && !it->getClass())
				throw PlayException("Every enemy should have a class.");

		}
	}

	// Environments
	{
		QVector<Environment*> environments = project->getEnvironments()->getEnvironments();
		if (environments.size() == 0)
			throw PlayException("At least one environment has to be created.");
	}

	// Start event
	{
		StartEvent *start = project->getStartEvent();
		if (!start)
			throw PlayException("There is no event to start a game.");

		QVector<Character*> party = start->getParty();
		if (party.size() == 0)
			throw PlayException("There has to be at least one character in the initial party.");

		if (party.size() > g_maxPartysize)
			throw PlayException("The initial party is too large.\nThe maximum amount of characters allowed is " + QString::number(g_maxPartysize) + ".");

		QPair<Environment*, QPoint> location = start->getLocation();
		if (!location.first)
			throw PlayException("There is no location specified to start the game.");

		QSize dimensions = location.first->getDimensions();
		if ((location.second.x() < 0) || (location.second.x() >= dimensions.width()) || (location.second.y() < 0) || (location.second.y() >= dimensions.height()))
			throw PlayException("The starting location is not within the bounds of the starting environment.\nPlace the starting point on a new location and try again.");

		if (location.first->getAccess(location.second) != Tileset::OVER)
			throw PlayException("The start should be located on a spot that is accessible.");
	}

	return true;
}

void PlayModule::closeEvent(QCloseEvent *event) {
	event->accept();
	_view->stop();
	delete _view;
	_view = 0;
	setCentralWidget(0);
}

QVector<GameEvent*> PlayModule::buildGameEvents() {
	QVector<GameEvent*> gameEvents(0);

	// Build the game events
	QVector<Event*> events = _project->getEvents()->getEvents();
	for (int i = 0; i < events.size(); i++)
		gameEvents.append(new GameEvent(events.at(i)));

	// Build game event dependencies
	for (int i = 0; i < events.size(); i++) {
		Event *event = events.at(i);
		GameEvent *gameEvent = gameEvents.at(i);

		// Search for the followup event
		if (!!event->getFollowup()) {
			for (int j = 0; j < gameEvents.size(); j++) {
				GameEvent *it = gameEvents.at(j);
				if (it->getEvent() == event->getFollowup())
					gameEvent->setFollowupEvent(it);
			}
		}

		// Search for requirement events
		QVector<Event*> requirements = event->getRequirements();
		for (int j = 0; j < requirements.size(); j++) {
			for (int k = 0; k < gameEvents.size(); k++) {
				if (requirements.at(j) == gameEvents.at(k)->getEvent()) {
					gameEvent->addRequirementEvent(gameEvents.at(k));
					break;
				}
			}
		}
	}

	return gameEvents;
}