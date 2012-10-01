#ifndef TILESETACCESSCONTROLWINDOW_H
#define TILESETACCESSCONTROLWINDOW_H

#include "qdialog.h"
#include "qtablewidget.h"

#include "../Components/Headers/Tileset.h"

using projectcomponents::Tileset;

namespace interfacemodule {
	
	/**
	*	This window allows to edit the tile access level of a given tileset by clicking on the tiles.
	*/
	class TileAccessWindow : public QDialog {

		Q_OBJECT

	public:
		TileAccessWindow(QWidget *parent, Tileset *tileset);
		virtual ~TileAccessWindow();

	private:
		enum Viewmode {NORMAL, ACCESS};

		void createWindow();
		void drawTileset();
		
		Viewmode _viewmode;
		Tileset *_tileset;
		QTableWidget *_tilesetTable;

	private slots:
		void tileClicked(int row, int column);
		void changeViewmode();
	};
}

#endif