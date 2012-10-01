#ifndef ACCESSOIREWINDOW_H
#define ACCESSOIREWINDOW_H

#include "qlabel.h"
#include "qframe.h"
#include "qboxlayout.h"
#include "qlistwidget.h"
#include "qplaintextedit.h"

#include "../Components/Headers/Properties.h"

using projectcomponents::Properties;

namespace interfacemodule {

	/**
	*	The accessories window allows to select accessories from a list and edit their properties.
	*/
	class AccessoriesWindow : public QFrame {

		Q_OBJECT

	public:
		AccessoriesWindow(QWidget *parent, Properties *properties);
		virtual ~AccessoriesWindow();

	private:
		void createWindow();
		void loadAccessories();

		Properties *_properties;

		QHBoxLayout *_layout;

		QListWidget *_accessoryList;
		QLabel *_accessoryDescLabel;
		QPlainTextEdit *_accessoryDescription;
		QLabel *_accessoryAttLabel;
		QListWidget *_accessoryAttributes;
		QLabel *_accessoryElemLabel;
		QListWidget *_accessoryElements;
		QLabel *_accessoryClassLabel;
		QListWidget *_accessoryClasses;

	signals:
		void updateSignal();

	public slots:
		void updateWindow();

	private slots:
		void accessorySelected(const QString &name);
		void newAccessory();
		void editAccessory();
		void deleteAccessory();
		void moveAccessoryUp();
		void moveAccessoryDown();

		void saveDescription();
		void reloadDescription();

		void addElement();
		void deleteElement();
		void moveElementUp();
		void moveElementDown();

		void addAttribute();
		void editAttribute();
		void deleteAttribute();
		void moveAttributeUp();
		void moveAttributeDown();

		void addClass();
		void editClass();
		void deleteClass();
		void moveClassUp();
		void moveClassDown();
	};
}

#endif