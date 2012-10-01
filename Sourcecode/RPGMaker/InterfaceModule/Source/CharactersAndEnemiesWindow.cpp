#include "qgroupbox.h"
#include "qboxlayout.h"

#include "../Headers/CharactersAndEnemiesWindow.h"

using namespace interfacemodule;

CharactersAndEnemiesWindow::CharactersAndEnemiesWindow(QWidget *parent, Project *project)
	: QTabWidget(parent), _project(project) {

		CharactersWindow *characters = new CharactersWindow(this, _project);
		EnemiesWindow *enemies = new EnemiesWindow(this, _project);

		addTab(characters, "Characters");
		addTab(enemies, "Enemies");
}

CharactersAndEnemiesWindow::~CharactersAndEnemiesWindow() {}