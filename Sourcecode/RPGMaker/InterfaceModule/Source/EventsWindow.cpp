#include "qgroupbox.h"

#include "../Headers/ObjectWindow.h"
#include "../Headers/EventsWindow.h"
#include "../Headers/TextEventWindow.h"
#include "../Headers/PortalEventWindow.h"
#include "../Headers/BattleEventWindow.h"

using namespace interfacemodule;

EventsWindow::EventsWindow(QWidget *parent, Project *project)
	: QTabWidget(parent), _project(project) {
		
		ObjectWindow *objects = new ObjectWindow(this, _project);
		TextEventWindow *textEvents = new TextEventWindow(this, _project);
		PortalEventWindow *portalEvents = new PortalEventWindow(this, _project);
		BattleEventWindow *battleEvents = new BattleEventWindow(this, _project);

		addTab(objects, "Objects");
		addTab(textEvents, "Text Events");
		addTab(portalEvents, "Portal Events");
		addTab(battleEvents, "Battle Events");

		connect(objects, SIGNAL(updateSignal()), textEvents, SLOT(updateWindow()));
		connect(objects, SIGNAL(updateSignal()), portalEvents, SLOT(updateWindow()));
		connect(objects, SIGNAL(updateSignal()), battleEvents, SLOT(updateWindow()));
}

EventsWindow::~EventsWindow() {}