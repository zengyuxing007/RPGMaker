#ifndef ENVIRONMENTSWINDOW_H
#define ENVIRONMENTSWINDOW_H

#include "qpair.h"
#include "qframe.h"
#include "qlabel.h"
#include "qpixmap.h"
#include "qcombobox.h"
#include "qboxlayout.h"
#include "qtablewidget.h"
#include "qbuttongroup.h"

#include "ProgramDefaults.h"
#include "../Components/Headers/Tile.h"
#include "../Components/Headers/Project.h"

using projectcomponents::Tile;
using projectcomponents::Project;

namespace interfacemodule {

	/**
	*	The environments window contains all the functionalitiy to create and edit tilesets and environments.
	*/
	class EnvironmentsWindow : public QFrame {
		
		Q_OBJECT

	public:
		EnvironmentsWindow(QWidget *parent, Project *project);
		virtual ~EnvironmentsWindow();

	private:
		void createTilesetBox();
		void createEnvironmentBox();
		void loadStaticTilesets();
		void loadAutoTilesets();
		void loadEnvironments();

		void paintEnvironment(Environment *environemnt);
		void markEnvironment(Environment *environment);
		void paintTile(Environment *environment, QPoint location);
		void markTile(Environment *environment, QPoint location);
		void placeTile(QPoint location);
		void placeEvent(QPoint location);

		// Window objects
		Project *_project;
		QHBoxLayout *_layout;

		// Tileset objects
		QPair<QLabel*, Tile*> _currenttile;
		QComboBox *_statictilesetselection;
		QTableWidget *_statictileset;
		QTableWidget *_autotileset;
		int _currenttab;

		// Environment objects
		QTableWidget *_environment;
		QComboBox *_environmentselection;
		QButtonGroup *_layergroup;
		QButtonGroup *_modegroup;
		int _currentlayer;

	private slots:
		void newTileset();
		void editTileset();
		void deleteTileset();
		void tilesetTabClicked(int index);
		void tilesetChanged(const QString& name);
		void tilesetCellClicked(int row, int column);
		void openAccessControl();

		void newEnvironment();
		void editEnvironment();
		void deleteEnvironment();
		void environmentChanged(const QString &name);
		void environmentCellClicked(int row, int column);
		
		void layerSelected(int layer);
		void displayPreview();
	};

}

#endif