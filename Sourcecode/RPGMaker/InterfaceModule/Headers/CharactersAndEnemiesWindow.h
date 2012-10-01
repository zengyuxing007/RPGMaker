#ifndef CHARACTERSANDENEMIESWINDOW_H
#define CHARACTERSANDENEMIESWINDOW_H

#include "qframe.h"

#include "../Headers/EnemiesWindow.h"
#include "../Headers/CharactersWindow.h"
#include "../Components/Headers/Project.h"

using namespace interfacemodule;
using projectcomponents::Project;

namespace interfacemodule {

	/**
	*	This window holds the character and enemy windows.
	*/
	class CharactersAndEnemiesWindow : public QTabWidget {
	public:
		CharactersAndEnemiesWindow(QWidget *parent, Project *project);
		virtual ~CharactersAndEnemiesWindow();

	private:
		Project *_project;
	};
}

#endif