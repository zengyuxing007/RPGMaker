#include "../ProgramDefaults.h"
#include "../Headers/XMLTranslator.h"
#include "../Components/Headers/AutoTile.h"
#include "../Components/Headers/StaticTile.h"

using namespace xmlmodule;

XMLTranslator::XMLTranslator() 
	: _project(0), _document(QDomDocument("projectdoc")) {}

XMLTranslator::~XMLTranslator() {}

QString XMLTranslator::translateToXML(Project *project) {
	QDomDocument document("project");
	project->translateToXML(project, document);
	QString projectstring = document.toString();

	return projectstring;
}

Project* XMLTranslator::translateFromXML(QString projectstring) {
	QString errmsg;
	int errline, errcolumn;
	if (!_document.setContent(projectstring, &errmsg, &errline, &errcolumn)) 
		throw XMLException(QString("Unable to process document ")
							+ QString(": ") + errmsg + QString(" at line ")
							+ QString::number(errline) + QString(", column ")
							+ QString::number(errcolumn));

	QDomElement rootnode = _document.documentElement();

	try {
		_project = Project::translateFromXML(rootnode);
		translateDependencies(rootnode);
	} catch (ProjectException &e) {
		delete _project;
		throw XMLException("Failed to translate projectdocument:\n" + e.what());
	}

	Project *project = _project;
	_project = 0;

	return project;
}

void XMLTranslator::translateDependencies(QDomElement &root) {
	QDomElement properties = root.firstChildElement("properties");
	translatePropertiesDependencies(properties);
	
	QDomElement characters = root.firstChildElement("characters");
	translateCharacterDependencies(characters);

	QDomElement enemies = root.firstChildElement("enemies");
	translateEnemyDependencies(enemies);

	QDomElement battlegroups = root.firstChildElement("battlegroups");
	translateBattlegroupDependencies(battlegroups);

	QDomElement events = root.firstChildElement("events");
	translateEventDependencies(events);

	QDomElement environments = root.firstChildElement("environments");
	translateEnvironmentDependencies(environments);

	QDomElement startEvent = root.firstChildElement("start");
	translateStartEventDependencies(startEvent);
}

void XMLTranslator::translatePropertiesDependencies(QDomElement &properties) {
	QDomElement attributes = properties.firstChildElement("attributes");
	this->translateAttributeDependencies(attributes);

	QDomElement elements = properties.firstChildElement("elements");
	this->translateElementDependencies(elements);

	QDomElement abilities = properties.firstChildElement("abilities");
	this->translateAbilityDependencies(abilities);

	QDomElement classes = properties.firstChildElement("classes");
	this->translateClassDependencies(classes);

	QDomElement equipment = properties.firstChildElement("equipment");
	this->translateEquipmentDependencies(equipment);

	QDomElement items = properties.firstChildElement("items");
	this->translateItemDependencies(items);
}

void XMLTranslator::translateAttributeDependencies(QDomElement &attributes) {
	Properties *properties = this->_project->getProperties();

	QDomNodeList offensiveElements = attributes.elementsByTagName("offensive");
	for (int i = 0; i < offensiveElements.size(); i++) {
		QDomElement offensiveElement = offensiveElements.at(i).toElement();
		if (!offensiveElement.firstChildElement("link").isNull()) {
			Attribute *offensiveAttribute = properties->getAttributes()->getAttribute(offensiveElement.attributeNode("name").value());
			Attribute *linkAttribute = properties->getAttributes()->getAttribute(offensiveElement.firstChildElement("link").attributeNode("name").value());

			offensiveAttribute->setLink(linkAttribute);
			linkAttribute->setLink(offensiveAttribute);
		}
	}

	QDomNodeList defensiveElements = attributes.elementsByTagName("defensive");
	for (int i = 0; i < defensiveElements.size(); i++) {
		QDomElement defensiveElement = defensiveElements.at(i).toElement();
		if (!defensiveElement.firstChildElement("link").isNull()) {
			Attribute *defensiveAttribute = properties->getAttributes()->getAttribute(defensiveElement.attributeNode("name").value());
			Attribute *linkAttribute = properties->getAttributes()->getAttribute(defensiveElement.firstChildElement("link").attributeNode("name").value());

			defensiveAttribute->setLink(linkAttribute);
			linkAttribute->setLink(defensiveAttribute);
		}
	}
}

void XMLTranslator::translateElementDependencies(QDomElement &param) {
	QDomNodeList elements = param.childNodes();
	Properties *properties = this->_project->getProperties();

	for (int i = 0; i < elements.size(); i++) {
		QDomElement it = elements.at(i).toElement();
		Element *element = properties->getElements()->getElement(it.attributeNode("name").value());

		QDomNodeList resistants = it.firstChildElement("resistant").childNodes();
		for (int j = 0; j < resistants.size(); j++) {
			QDomElement resistant = resistants.at(j).toElement();
			QString name = resistant.attributeNode("name").value();
			element->addResistantElement(properties->getElements()->getElement(name));
		}

		QDomNodeList vulnerables = it.firstChildElement("vulnerable").childNodes();
		for (int j = 0; j < vulnerables.size(); j++) {
			QDomElement vulnerable = vulnerables.at(j).toElement();
			QString name = vulnerable.attributeNode("name").value();
			element->addVulnerableElement(properties->getElements()->getElement(name));
		}
	}
}

void XMLTranslator::translateAbilityDependencies(QDomElement &param) {
	QDomNodeList offAbilities = param.firstChildElement("offensive").childNodes();
	QDomNodeList defAbilities = param.firstChildElement("defensive").childNodes();
	Properties *properties = this->_project->getProperties();
	
	for (int i = 0; i < offAbilities.size(); i++) {
		QDomElement it = offAbilities.at(i).toElement();
		Ability *ability = properties->getAbilities()->getAbility(it.attributeNode("name").value());

		QDomNodeList elements = it.firstChildElement("elements").childNodes();
		for (int j = 0; j < elements.size(); j++) {
			QDomElement element = elements.at(j).toElement();
			QString name = element.attributeNode("name").value();
			ability->addElement(properties->getElements()->getElement(name));
		}

		QDomElement attributeElement = it.firstChildElement("attribute");
		if (!attributeElement.isNull()) {
			QString name = attributeElement.attributeNode("name").value();
			double multiplier = attributeElement.attributeNode("multiplier").value().toDouble();
			Attribute *attribute = properties->getAttributes()->getAttribute(name);
			ability->setAttribute(attribute, multiplier);
		}
	}

	for (int i = 0; i < defAbilities.size(); i++) {
		QDomElement it = defAbilities.at(i).toElement();
		Ability *ability = properties->getAbilities()->getAbility(it.attributeNode("name").value());

		QDomNodeList elements = it.firstChildElement("elements").childNodes();
		for (int j = 0; j < elements.size(); j++) {
			QDomElement element = elements.at(j).toElement();
			QString name = element.attributeNode("name").value();
			ability->addElement(properties->getElements()->getElement(name));
		}

		QDomElement attributeElement = it.firstChildElement("attribute");
		if (!attributeElement.isNull()) {
			QString name = attributeElement.attributeNode("name").value();
			double multiplier = attributeElement.attributeNode("multiplier").value().toDouble();
			Attribute *attribute = properties->getAttributes()->getAttribute(name);
			ability->setAttribute(attribute, multiplier);
		}
	}
}

void XMLTranslator::translateClassDependencies(QDomElement &param) {
	QDomNodeList classes = param.childNodes();
	Properties *properties = this->_project->getProperties();

	for (int i = 0; i < classes.size(); i++) {

		QDomElement it = classes.at(i).toElement();
		Class *cat = properties->getClasses()->getClass(it.attributeNode("name").value());

		QDomNodeList attributesElement = it.firstChildElement("attributes").childNodes();
		for (int j = 0; j < attributesElement.size(); j++) {
			QDomElement attributeElement = attributesElement.at(j).toElement();
			QString name = attributeElement.attributeNode("name").value();
			int initial = attributeElement.attributeNode("initial").value().toInt();
			double value = attributeElement.attributeNode("multiplier").value().toDouble();
			cat->addAttribute(properties->getAttributes()->getAttribute(name), initial, value);
		}

		QDomElement primaryAttributeElement = it.firstChildElement("primaryAttribute");
		if (!primaryAttributeElement.isNull()) {
			QString name = primaryAttributeElement.attributeNode("name").value();
			cat->setPrimaryAttribute(properties->getAttributes()->getAttribute(name));
		}

		QDomNodeList abilitiesElement = it.firstChildElement("abilities").childNodes();
		for (int j = 0; j < abilitiesElement.size(); j++) {
			QDomElement ability = abilitiesElement.at(j).toElement();
			QString name = ability.attributeNode("name").value();
			int acquire = ability.attributeNode("acquire").value().toInt();
			cat->addAbility(properties->getAbilities()->getAbility(name), acquire);
		}
	}
}

void XMLTranslator::translateEquipmentDependencies(QDomElement &param) {
	QDomElement weapons = param.firstChildElement("weapons");
	this->translateWeaponDependencies(weapons);

	QDomElement armor = param.firstChildElement("armor");
	this->translateArmorDependencies(armor);

	QDomElement accessories = param.firstChildElement("accessories");
	this->translateAccessoryDependencies(accessories);
}

void XMLTranslator::translateWeaponDependencies(QDomElement &param) {
	QDomNodeList weapons = param.childNodes();
	Properties *properties = this->_project->getProperties();

	for (int i = 0; i < weapons.size(); i++) {
		QDomElement it = weapons.at(i).toElement();
		Weapon *weapon = properties->getEquipment()->getWeapon(it.attributeNode("name").value());

		QDomNodeList elements = it.firstChildElement("elements").childNodes();
		for (int j = 0; j < elements.size(); j++) {
			QDomElement element = elements.at(j).toElement();
			QString name = element.attributeNode("name").value();
			weapon->addElement(properties->getElements()->getElement(name));
		}

		QDomNodeList attributes = it.firstChildElement("attributes").childNodes();
		for (int j = 0; j < attributes.size(); j++) {
			QDomElement attribute = attributes.at(j).toElement();
			QString name = attribute.attributeNode("name").value();
			int value = attribute.attributeNode("value").value().toInt();
			weapon->addAttribute(properties->getAttributes()->getAttribute(name), value);
		}

		QDomNodeList classes = it.firstChildElement("classes").childNodes();
		for (int j = 0; j < classes.size(); j++) {
			QDomElement cat = classes.at(j).toElement();
			QString name = cat.attributeNode("name").value();
			int level = cat.attributeNode("level").value().toInt();
			weapon->addClass(properties->getClasses()->getClass(name), level);
		}
	}
}

void XMLTranslator::translateArmorDependencies(QDomElement &param) {
	QDomNodeList armors = param.childNodes();
	Properties *properties = this->_project->getProperties();

	for (int i = 0; i < armors.size(); i++) {
		QDomElement it = armors.at(i).toElement();
		Armor *armor = properties->getEquipment()->getArmor(it.attributeNode("name").value());

		QDomNodeList elements = it.firstChildElement("elements").childNodes();
		for (int j = 0; j < elements.size(); j++) {
			QDomElement element = elements.at(j).toElement();
			QString name = element.attributeNode("name").value();
			armor->addElement(properties->getElements()->getElement(name));
		}

		QDomNodeList attributes = it.firstChildElement("attributes").childNodes();
		for (int j = 0; j < attributes.size(); j++) {
			QDomElement attribute = attributes.at(j).toElement();
			QString name = attribute.attributeNode("name").value();
			int value = attribute.attributeNode("value").value().toInt();
			armor->addAttribute(properties->getAttributes()->getAttribute(name), value);
		}

		QDomNodeList classes = it.firstChildElement("classes").childNodes();
		for (int j = 0; j < classes.size(); j++) {
			QDomElement cat = classes.at(j).toElement();
			QString name = cat.attributeNode("name").value();
			int level = cat.attributeNode("level").value().toInt();
			armor->addClass(properties->getClasses()->getClass(name), level);
		}
	}
}

void XMLTranslator::translateAccessoryDependencies(QDomElement &param) {
	QDomNodeList accessories = param.childNodes();
	Properties *properties = this->_project->getProperties();

	for (int i = 0; i < accessories.size(); i++) {
		QDomElement it = accessories.at(i).toElement();
		Accessory *accessory = properties->getEquipment()->getAccessory(it.attributeNode("name").value());

		QDomNodeList elements = it.firstChildElement("elements").childNodes();
		for (int j = 0; j < elements.size(); j++) {
			QDomElement element = elements.at(j).toElement();
			QString name = element.attributeNode("name").value();
			accessory->addElement(properties->getElements()->getElement(name));
		}

		QDomNodeList attributes = it.firstChildElement("attributes").childNodes();
		for (int j = 0; j < attributes.size(); j++) {
			QDomElement attribute = attributes.at(j).toElement();
			QString name = attribute.attributeNode("name").value();
			int value = attribute.attributeNode("value").value().toInt();
			accessory->addAttribute(properties->getAttributes()->getAttribute(name), value);
		}

		QDomNodeList classes = it.firstChildElement("classes").childNodes();
		for (int j = 0; j < classes.size(); j++) {
			QDomElement cat = classes.at(j).toElement();
			QString name = cat.attributeNode("name").value();
			int level = cat.attributeNode("level").value().toInt();
			accessory->addClass(properties->getClasses()->getClass(name), level);
		}
	}
}

void XMLTranslator::translateItemDependencies(QDomElement &param) {
	// Nothing to do here...
}

void XMLTranslator::translateCharacterDependencies(QDomElement &param) {
	QDomNodeList characters = param.childNodes();
	Properties *properties = this->_project->getProperties();

	for (int i = 0; i < characters.size(); i++) {
		QDomElement it = characters.at(i).toElement();
		Character *character = _project->getCharacters()->getCharacter(it.attributeNode("name").value());

		QDomElement catElement = it.firstChildElement("class");
		if (!catElement.isNull()) {
			Class *cat = properties->getClasses()->getClass(catElement.attributeNode("name").value());
			character->setClass(cat);
		}

		QDomNodeList elements = it.firstChildElement("elements").childNodes();
		for (int j = 0; j < elements.size(); j++) {
			QDomElement element = elements.at(j).toElement();
			QString name = element.attributeNode("name").value();
			character->addElement(properties->getElements()->getElement(name));
		}

		QDomNodeList abilities = it.firstChildElement("abilities").childNodes();
		for (int j = 0; j < abilities.size(); j++) {
			QDomElement ability = abilities.at(j).toElement();
			QString name = ability.attributeNode("name").value();
			int acquire = ability.attributeNode("acquire").value().toInt();
			character->addAbility(properties->getAbilities()->getAbility(name), acquire);
		}

		QDomElement equipment = it.firstChildElement("equipment");
		QDomNodeList weapons = equipment.firstChildElement("weapons").childNodes();
		for (int i = 0; i < weapons.size(); i++) {
			QDomElement weaponElement = weapons.at(i).toElement();
			QString name = weaponElement.attributeNode("name").value();
			character->getEquipment()->setWeapon(properties->getEquipment()->getWeapon(name));
		}

		QDomNodeList armor = equipment.firstChildElement("armor").childNodes();
		for (int i = 0; i < armor.size(); i++) {
			QDomElement armorElement = armor.at(i).toElement();
			QString name = armorElement.attributeNode("name").value();
			character->getEquipment()->setArmor(properties->getEquipment()->getArmor(name));
		}

		QDomNodeList accessories = equipment.firstChildElement("accessories").childNodes();
		for (int i = 0; i < accessories.size(); i++) {
			QDomElement accessoriesElement = accessories.at(i).toElement();
			QString name = accessoriesElement.attributeNode("name").value();
			character->getEquipment()->setAccessory(properties->getEquipment()->getAccessory(name));
		}

	}
}

void XMLTranslator::translateEnemyDependencies(QDomElement &param) {
	QDomNodeList enemies = param.childNodes();
	Properties *properties = this->_project->getProperties();

	for (int i = 0; i < enemies.size(); i++) {
		QDomElement it = enemies.at(i).toElement();
		Enemy *enemy = this->_project->getEnemies()->getEnemy(it.attributeNode("name").value());

		QDomElement cat = it.firstChildElement("class");
		if (!cat.isNull())
			enemy->setClass(properties->getClasses()->getClass(cat.attributeNode("name").value()));

		QDomNodeList elements = it.firstChildElement("elements").childNodes();
		for (int j = 0; j < elements.size(); j++) {
			QDomElement element = elements.at(j).toElement();
			QString name = element.attributeNode("name").value();
			enemy->addElement(properties->getElements()->getElement(name));
		}

		QDomNodeList abilities = it.firstChildElement("abilities").childNodes();
		for (int j = 0; j < abilities.size(); j++) {
			QDomElement ability = abilities.at(j).toElement();
			QString name = ability.attributeNode("name").value();
			enemy->addAbility(properties->getAbilities()->getAbility(name));
		}

		QDomElement equipment = it.firstChildElement("drops").firstChildElement("equipment");
		QDomNodeList weapons = equipment.elementsByTagName("weapon");
		for (int j = 0; j < weapons.size(); j++) {
			QDomElement weaponElement = weapons.at(j).toElement();
			QString name = weaponElement.attributeNode("name").value();
			int chance = weaponElement.attributeNode("chance").value().toInt();
			enemy->getDrops()->addItem(properties->getEquipment()->getWeapon(name), chance);
		}

		QDomNodeList armor = equipment.elementsByTagName("armor");
		for (int j = 0; j < armor.size(); j++) {
			QDomElement armorElement = armor.at(j).toElement();
			QString name = armorElement.attributeNode("name").value();
			int chance = armorElement.attributeNode("chance").value().toInt();
			enemy->getDrops()->addItem(properties->getEquipment()->getArmor(name), chance);
		}

		QDomNodeList accessories = equipment.elementsByTagName("accessory");
		for (int j = 0; j < accessories.size(); j++) {
			QDomElement accessoryElement = accessories.at(j).toElement();
			QString name = accessoryElement.attributeNode("name").value();
			int chance = accessoryElement.attributeNode("chance").value().toInt();
			enemy->getDrops()->addItem(properties->getEquipment()->getAccessory(name), chance);
		}
	}
}

void XMLTranslator::translateBattlegroupDependencies(QDomElement &param) {
	QDomNodeList groups = param.childNodes();
	for (int i = 0; i < groups.size(); i++) {
		QDomElement groupElement = groups.at(i).toElement();
		Battlegroup *group = _project->getBattlegroups()->getBattlegroup(groupElement.attributeNode("name").value());

		if (!!group) {
			QDomNodeList enemies = groupElement.elementsByTagName("enemy");
			for (int j = 0; j < enemies.size(); j++) {
				QDomElement enemyElement = enemies.at(j).toElement();
				Enemy *enemy = _project->getEnemies()->getEnemy(enemyElement.attributeNode("name").value());
				group->addEnemy(enemy);
			}
		}
	} 
}

void XMLTranslator::translateEventDependencies(QDomElement &param) {
	// Text Events
	QDomNodeList textEvents = param.elementsByTagName("textEvent");
	for (int i = 0; i < textEvents.size(); i++) {
		QDomElement textElement = textEvents.at(i).toElement();
		TextEvent *event = _project->getEvents()->getTextEvent(textElement.attributeNode("ID").value());

		if (!!event) {
			// Translate follow up event
			QDomElement followupElement = textElement.firstChildElement("followupEvent");
			if (!followupElement.isNull()) {
				event->setFollowup(_project->getEvents()->getEvent(followupElement.attribute("name")));
			}

			// Translate requirements
			QDomNodeList requirements = textElement.firstChildElement("requirements").childNodes();
			for (int j = 0; j < requirements.size(); j++) {
				QDomElement requirementElement = requirements.at(j).toElement();
				event->addRequirement(_project->getEvents()->getEvent(requirementElement.attribute("name")));
			}

			// Translate object
			QDomElement objectElement = textElement.firstChildElement("object");
			if (!objectElement.isNull()) {
				Object *object = _project->getObjects()->getObject(objectElement.attributeNode("name").value());
				if (!!object)
					event->setObject(object);
			}
		}
	}

	// Portal events
	QDomNodeList portalEvents = param.elementsByTagName("portalEvent");
	for (int i = 0; i < portalEvents.size(); i++) {
		QDomElement portalElement = portalEvents.at(i).toElement();
		PortalEvent *event = _project->getEvents()->getPortalEvent(portalElement.attributeNode("ID").value());
		
		if (!!event) {
			// Translate follow up event
			QDomElement followupElement = portalElement.firstChildElement("followupEvent");
			if (!followupElement.isNull()) {
				event->setFollowup(_project->getEvents()->getEvent(followupElement.attribute("name")));
			}

			// Translate requirements
			QDomNodeList requirements = portalElement.firstChildElement("requirements").childNodes();
			for (int j = 0; j < requirements.size(); j++) {
				QDomElement requirementElement = requirements.at(j).toElement();
				event->addRequirement(_project->getEvents()->getEvent(requirementElement.attribute("name")));
			}

			// Translate object
			QDomElement objectElement = portalElement.firstChildElement("object");
			if (!objectElement.isNull()) {
				Object *object = _project->getObjects()->getObject(objectElement.attributeNode("name").value());
				if (!!object)
					event->setObject(object);
			}

			// Translate destination
			QDomElement toElement = portalElement.firstChildElement("to");
			if (!toElement.isNull()) {
				Environment *environment = _project->getEnvironments()->getEnvironment(toElement.attributeNode("location").value());
				int column = toElement.attributeNode("column").value().toInt();
				int row = toElement.attributeNode("row").value().toInt();

				event->setDestination(environment, QPoint(column, row));
			}
		}
	}

	// Battle events
	QDomNodeList battleEvents = param.elementsByTagName("battleEvent");
	for (int i = 0; i < battleEvents.size(); i++) {
		QDomElement battleElement = battleEvents.at(i).toElement();
		BattleEvent *event = _project->getEvents()->getBattleEvent(battleElement.attributeNode("ID").value());

		if (!!event) {
			// Translate follow up event
			QDomElement followupElement = battleElement.firstChildElement("followupEvent");
			if (!followupElement.isNull()) {
				event->setFollowup(_project->getEvents()->getEvent(followupElement.attribute("name")));
			}

			// Translate requirements
			QDomNodeList requirements = battleElement.firstChildElement("requirements").childNodes();
			for (int j = 0; j < requirements.size(); j++) {
				QDomElement requirementElement = requirements.at(j).toElement();
				event->addRequirement(_project->getEvents()->getEvent(requirementElement.attribute("name")));
			}

			// Translate object
			QDomElement objectElement = battleElement.firstChildElement("object");
			if (!objectElement.isNull()) {
				Object *object = _project->getObjects()->getObject(objectElement.attributeNode("name").value());
				if (!!object)
					event->setObject(object);
			}

			// Translate battlegroups
			QDomNodeList battlegroups = battleElement.elementsByTagName("battlegroup");
			for (int j = 0; j < battlegroups.size(); j++) {
				QDomElement groupElement = battlegroups.at(j).toElement();
				Battlegroup *group = _project->getBattlegroups()->getBattlegroup(groupElement.attributeNode("name").value());
				int value = groupElement.attributeNode("value").value().toInt();
				event->addBattlegroup(group, value);
			}
		}
	}
}

void XMLTranslator::translateEnvironmentDependencies(QDomElement &param) {
	QDomNodeList environments = param.childNodes();
	Properties *properties = this->_project->getProperties();

	for (int i = 0; i < environments.size(); i++) {
		QDomElement it = environments.at(i).toElement();
		Environment *environment = _project->getEnvironments()->getEnvironment(it.attributeNode("name").value());

		// Translate layers
		QDomNodeList layers = it.firstChildElement("layers").childNodes();
		for (int j = 0; j < layers.size(); j++) {
			QDomElement layerElement = layers.at(j).toElement();

			QDomNodeList tilesElement = layerElement.childNodes();
			for (int k = 0; k < tilesElement.size(); k++) {
				QDomElement tileElement = tilesElement.at(k).toElement();
				QDomElement tilesetElement = tileElement.firstChildElement("tileset");

				Tile *tile;
				QPoint tilelocation(tileElement.attributeNode("column").value().toInt(), tileElement.attributeNode("row").value().toInt());

				Tileset *tileset = _project->getProperties()->getTilesets()->getTileset(tilesetElement.attributeNode("name").value());
				QPoint tilesetlocation(tilesetElement.attributeNode("column").value().toInt(), tilesetElement.attributeNode("row").value().toInt());
				switch (tileset->getType()) {
				case Tileset::STATIC:
					tile = StaticTile::create(tileset, tilesetlocation);
					break;
				case Tileset::AUTO:
					tile = AutoTile::create(tileset, tilesetlocation);
					break;
				}

				environment->setTile(tile, layerElement.attributeNode("nr").value().toInt(), tilelocation);
			}
		}

		// Translate autotiles for setting up their surrounding tiles
		for (int j = 0; j < g_layers; j++) {
			QSize dimensions = environment->getDimensions();
			for (int y = 0; y < dimensions.height(); y++) {
				for (int x = 0; x < dimensions.width(); x++) {
					Tile *tile = environment->getTile(j, QPoint(x, y));
					if (!!tile && (tile->getType() == Tile::AUTO)) {
						QVector<Tile*> surroundingtiles(9, 0);

						// Get surrounding tiles
						for (int p = -1; p < 2; p++) {
							for (int q = -1; q < 2; q++) {
								if ((p == 0) && (q == 0))
									continue;
								
								try {
									surroundingtiles.replace((p+1)*3+(q+1), environment->getTile(j, QPoint(x+q, y+p)));
								} catch (ProjectException &e) {
									surroundingtiles.replace((p+1)*3+(q+1), 0);
								}
							}
						}
						
						AutoTile *autotile = dynamic_cast<AutoTile*>(tile);
						autotile->setSurroundingTiles(surroundingtiles);
					}
				}
			}
		}

		// Translate events
		QDomNodeList events = it.firstChildElement("events").childNodes();
		for (int j = 0; j < events.size(); j++) {
			QDomElement eventElement = events.at(j).toElement();
			Event *event = _project->getEvents()->getEvent(eventElement.attributeNode("ID").value());
			int column = eventElement.attributeNode("column").value().toInt();
			int row = eventElement.attributeNode("row").value().toInt();

			environment->setEvent(event, QPoint(column, row));
		}
	}
}

void XMLTranslator::translateStartEventDependencies(QDomElement &param) {
	StartEvent *start = _project->getStartEvent();

	// Location
	QDomElement locationElement = param.firstChildElement("location");
	if (!locationElement.isNull()) {
		Environment *environment = _project->getEnvironments()->getEnvironment(locationElement.attributeNode("environment").value());
		int row = locationElement.attributeNode("row").value().toInt();
		int column = locationElement.attributeNode("column").value().toInt();

		start->setLocation(environment, QPoint(column, row));
	}

	// Party
	QDomNodeList startCharacters = param.firstChildElement("party").childNodes();
	for (int i = 0; i < startCharacters.size(); i++) {
		QDomElement it = startCharacters.at(i).toElement();
		Character *character = _project->getCharacters()->getCharacter(it.attributeNode("name").value());
		start->addCharacter(character);
	}

	// Inventory - Weapons
	QDomNodeList weapons = param.firstChildElement("inventory").elementsByTagName("weapon");
	for (int i = 0; i < weapons.size(); i++) {
		QDomElement it = weapons.at(i).toElement();
		Weapon *weapon = _project->getProperties()->getEquipment()->getWeapon(it.attributeNode("name").value());
		int amount = it.attributeNode("amount").value().toInt();
		// Add weapons
	}

	// Inventory - Armor
	QDomNodeList armor = param.firstChildElement("inventory").elementsByTagName("armor");
	for (int i = 0; i < armor.size(); i++) {
		QDomElement it = armor.at(i).toElement();
		Armor *armor = _project->getProperties()->getEquipment()->getArmor(it.attributeNode("name").value());
		int amount = it.attributeNode("amount").value().toInt();
		// Add armor
	}

	// Inventory - Accessories
	QDomNodeList accessories = param.firstChildElement("inventory").elementsByTagName("accessory");
	for (int i = 0; i < accessories.size(); i++) {
		QDomElement it = weapons.at(i).toElement();
		Accessory *accessory = _project->getProperties()->getEquipment()->getAccessory(it.attributeNode("name").value());
		int amount = it.attributeNode("amount").value().toInt();
		// Add accessory
	}
}