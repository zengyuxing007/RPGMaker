#include "qgroupbox.h"

#include "../Headers/ArmorWindow.h"
#include "../Headers/WeaponWindow.h"
#include "../Headers/ClassesWindow.h"
#include "../Headers/ElementsWindow.h"
#include "../Headers/AbilitiesWindow.h"
#include "../Headers/PropertiesWindow.h"
#include "../Headers/AttributesWindow.h"
#include "../Headers/AccessoriesWindow.h"

using namespace interfacemodule;

PropertiesWindow::PropertiesWindow(QWidget *parent, Properties *properties) 
	: QTabWidget(parent), _properties(properties) {

		ElementsWindow *elements = new ElementsWindow(this, _properties);
		AttributesWindow *attributes = new AttributesWindow(this, _properties->getAttributes());
		AbilitiesWindow *abilities = new AbilitiesWindow(this, _properties);
		ClassesWindow *classes = new ClassesWindow(this, _properties);
		WeaponWindow *weapons = new WeaponWindow(this, _properties);
		ArmorWindow *armor = new ArmorWindow(this, _properties);
		AccessoriesWindow *accessories = new AccessoriesWindow(this, _properties);

		addTab(elements, "Elements");
		addTab(attributes, "Attributes");
		addTab(abilities, "Abilities");
		addTab(classes, "Classes");
		addTab(weapons, "Weapons");
		addTab(armor, "Armor");
		addTab(accessories, "Accessories");

		connect(elements, SIGNAL(updateSignal()), abilities, SLOT(updateWindow()));
		connect(elements, SIGNAL(updateSignal()), weapons, SLOT(updateWindow()));
		connect(elements, SIGNAL(updateSignal()), armor, SLOT(updateWindow()));
		connect(elements, SIGNAL(updateSignal()), accessories, SLOT(updateWindow()));
		connect(attributes, SIGNAL(updateSignal()), abilities, SLOT(updateWindow()));
		connect(attributes, SIGNAL(updateSignal()), classes, SLOT(updateWindow()));
		connect(attributes, SIGNAL(updateSignal()), weapons, SLOT(updateWindow()));
		connect(attributes, SIGNAL(updateSignal()), armor, SLOT(updateWindow()));
		connect(attributes, SIGNAL(updateSignal()), accessories, SLOT(updateWindow()));
		connect(abilities, SIGNAL(updateSignal()), classes, SLOT(updateWindow()));
		connect(classes, SIGNAL(updateSignal()), weapons, SLOT(updateWindow()));
		connect(classes, SIGNAL(updateSignal()), armor, SLOT(updateWindow()));
		connect(classes, SIGNAL(updateSignal()), accessories, SLOT(updateWindow()));
}

PropertiesWindow::~PropertiesWindow() {}