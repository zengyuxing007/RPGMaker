#ifndef WEAPONWINDOW_H
#define WEAPONWINDOW_H

#include "qlabel.h"
#include "qframe.h"
#include "qboxlayout.h"
#include "qlistwidget.h"
#include "qplaintextedit.h"

#include "../Components/Headers/Properties.h"

using projectcomponents::Properties;

namespace interfacemodule {

	/**
	*	The weapon window allows to select weapons from a list and edit their properties.
	*/
	class WeaponWindow : public QFrame {

		Q_OBJECT

	public:
		WeaponWindow(QWidget *parent, Properties *properties);
		virtual ~WeaponWindow();

	private:
		void createWindow();
		void loadWeapons();

		Properties *_properties;

		QHBoxLayout *_layout;

		QListWidget *_weaponList;
		QLabel *_weaponDescLabel;
		QPlainTextEdit *_weaponDescription;
		QLabel *_weaponAttLabel;
		QListWidget *_weaponAttributes;
		QLabel *_weaponElemLabel;
		QListWidget *_weaponElements;
		QLabel *_weaponClassLabel;
		QListWidget *_weaponClasses;

	signals:
		void updateSignal();

	public slots:
		void updateWindow();

	private slots:
		void weaponSelected(const QString &name);
		void newWeapon();
		void editWeapon();
		void deleteWeapon();
		void moveWeaponUp();
		void moveWeaponDown();
		
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