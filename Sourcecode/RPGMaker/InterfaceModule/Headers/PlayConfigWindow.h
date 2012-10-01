#ifndef PLAYCONFIGWINDOW_H
#define PLAYCONFIGWINDOW_H

#include "qdialog.h"
#include "qcombobox.h"
#include "qtabwidget.h"
#include "qlistwidget.h"
#include "qtablewidget.h"

#include "../Components/Headers/Project.h"

using projectcomponents::Project;

namespace interfacemodule {

	/**
	*	The play configuration screen allows to edit the start settings for the game.
	*	This includes the starting location and the initial party members.
	*/
	class PlayConfigWindow : public QDialog {

		Q_OBJECT

	public:
		PlayConfigWindow(QWidget *parent, Project *project);
		virtual ~PlayConfigWindow();


	private:
		void createLocationTab();
		void createPartyTab();
		void createInventoryTab();
		void loadLocationTab();
		void loadPartyTab();
		void loadInventoryTab();

		void paintEnvironment(Environment *environment);
		void paintTile(Environment *environment, QPoint location);
		void markTile(Environment *environment, QPoint location);

		Project *_project;
		QTabWidget *_tabs;

		// Location tab
		QComboBox *_environmentSelection;
		QTableWidget *_environment;

		// Party tab
		QTableWidget *_party;

	private slots:
		void environmentChanged(const QString &name);
		void environmentCellClicked(int row, int column);

		void addCharacter();
		void removeCharacter();
	};
}

#endif