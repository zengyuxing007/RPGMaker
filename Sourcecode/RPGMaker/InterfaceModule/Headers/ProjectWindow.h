#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include "qframe.h"
#include "qstackedlayout.h"

#include "../Components/Headers/Project.h"

using namespace projectcomponents;

namespace interfacemodule {

	/**
	*	The project window is the main window that holds all of the other windows.
	*/
	class ProjectWindow : public QFrame {

		Q_OBJECT

	public:
		ProjectWindow(QWidget *parent, Project *project);
		virtual ~ProjectWindow();

	public slots:
		void renameSlot();
		void propertiesSlot();
		void charactersAndEnemiesSlot();
		void objectsAndEventsSlot();
		void tilesetsAndEnvironmentsSlot();
		void playSlot();

	private:
		Project *_project;
		QStackedLayout *_layout;
		QWidget *_currentFrame;

	};

}

#endif