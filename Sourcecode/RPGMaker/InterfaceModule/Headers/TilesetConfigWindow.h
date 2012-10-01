#ifndef TILESETCONFIGWINDOW_H
#define TILESETCONFIGWINDOW_H

#include "qframe.h"
#include "qlabel.h"
#include "qpixmap.h"
#include "qstring.h"
#include "qdialog.h"
#include "qlineedit.h"
#include "qcombobox.h"
#include "qboxlayout.h"
#include "qpushbutton.h"
#include "qradiobutton.h"

#include "ProgramDefaults.h"
#include "InterfaceException.h"
#include "../Components/Headers/Project.h"
#include "../Components/Headers/Tileset.h"

using projectcomponents::Project;
using projectcomponents::Tileset;

namespace interfacemodule {

	/**
	*	Configuration window for Tilesets. This window allows to create or edit a tileset.
	*/
	class TilesetConfigWindow : public QDialog {

		Q_OBJECT

	public:
		enum setupMode {NEW, EDIT};

		TilesetConfigWindow(QWidget *parent, setupMode mode, Project *project);
		virtual ~TilesetConfigWindow();

		QString getName() const;
		QString getImagename() const;
		Tileset::Type getType() const;

		void setupNewTileset();
		void setupEditTileset(QString tilesetname, Tileset::Type type, QString imagename, QPixmap image);

	private:
		void createLeftPane(setupMode mode);
		void createRightPane();

		Project *_project;

		setupMode _mode;
		QHBoxLayout *_layout;
		QLineEdit *_nameframe;
		QWidget *_typeframe;
		QWidget *_imagenameframe;
		QLabel *_imageframe;

	private slots:
		void checkInput();
		void currentTypeChanged(const QString &type);
		void currentImagenameChanged(const QString &name);
	};

}

#endif