#ifndef EVENTSWINDOW_H
#define EVENTSWINDOW_H

#include "qtabwidget.h"

#include "../Components/Headers/Project.h"

using projectcomponents::Project;

namespace interfacemodule {

	/**
	*	The events window contains the windows of the over evenst; portal events, text events battle events and also objects.
	*/
	class EventsWindow : public QTabWidget {

		Q_OBJECT

	public:
		EventsWindow(QWidget *parent, Project *project);
		virtual ~EventsWindow();

	private:
		Project *_project;
	};
}

#endif