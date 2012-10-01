#ifndef OBJECTWINDOW_H
#define OBJECTWINDOW_H

#include "qlabel.h"
#include "qframe.h"
#include "qstring.h"
#include "qcombobox.h"
#include "qlistwidget.h"
#include "qtablewidget.h"
#include "qbuttongroup.h"
#include "qplaintextedit.h"

#include "../Components/Headers/Project.h"

using projectcomponents::Project;

namespace interfacemodule {

	/**
	*	The object window allows to select objects from a list and change their appearence.
	*/
	class ObjectWindow : public QFrame {

		Q_OBJECT

	public:
		ObjectWindow(QWidget *parent, Project *project);
		virtual ~ObjectWindow();

	private:
		void createWindow();
		void loadObjects();

		Project *_project;

		QListWidget *_objectsList;
		QComboBox *_imagesList;
		QLabel *_currentImage;
		QTableWidget *_imageTable;

	private slots:
		void objectsSelected(const QString &name);
		void newObject();
		void editObject();
		void deleteObject();

		void imageSelected(const QString &name);
		void frameClicked(int row, int column);

	signals:
		void updateSignal();
	};
}

#endif