#ifndef PORTALEVENTWINDOW_H
#define PORTALEVENTWINDOW_H

#include "qframe.h"
#include "qlabel.h"
#include "qstring.h"
#include "qcombobox.h"
#include "qlistwidget.h"
#include "qtablewidget.h"
#include "qbuttongroup.h"

#include "../Components/Headers/Project.h"
#include "../Components/Headers/Environment.h"

using projectcomponents::Project;

namespace interfacemodule {

	/**
	*	The portal event window allows to select portal events from a list and edit its destination.
	*/
	class PortalEventWindow : public QFrame {

		Q_OBJECT

	public:
		PortalEventWindow(QWidget *parent, Project *project);
		virtual ~PortalEventWindow();

	private:
		void createWindow();
		void loadEvents();
		void paintEnvironment(Environment *environment);
		void paintTile(Environment *environment, QPoint location);
		void markTile(Environment *environment, QPoint location);

		Project *_project;

		QListWidget *_eventsList;
		QButtonGroup *_triggerButtons;
		QComboBox *_objectList;
		QLabel *_currentObject;
		QComboBox *_environmentList;
		QTableWidget *_environment;

	public slots:
		void updateWindow();

	private slots:
		void eventSelected(const QString &name);
		void newPortalEvent();
		void editPortalEvent();
		void deletePortalEvent();
		void movePortalEventUp();
		void movePortalEventDown();
		void triggerChanged(int id);

		void objectSelected(const QString &);

		void environmentSelected(const QString &name);
		void environmentCellClicked(int row, int column);
	};
}

#endif