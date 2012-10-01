#ifndef ARMORWINDOW_H
#define ARMORWINDOW_H

#include "qlabel.h"
#include "qframe.h"
#include "qboxlayout.h"
#include "qlistwidget.h"
#include "qplaintextedit.h"

#include "../Components/Headers/Properties.h"

using projectcomponents::Properties;

namespace interfacemodule {

	/**
	*	The armor window allows to select armor pieces from a list and edit their properties.
	*/
	class ArmorWindow : public QFrame {

		Q_OBJECT

	public:
		ArmorWindow(QWidget *parent, Properties *properties);
		virtual ~ArmorWindow();

	private:
		void createWindow();
		void loadArmor();

		Properties *_properties;

		QHBoxLayout *_layout;

		QListWidget *_armorList;
		QLabel *_armorDescLabel;
		QPlainTextEdit *_armorDescription;
		QLabel *_armorAttLabel;
		QListWidget *_armorAttributes;
		QLabel *_armorElemLabel;
		QListWidget *_armorElements;
		QLabel *_armorClassLabel;
		QListWidget *_armorClasses;

	signals:
		void updateSignal();

	public slots:
		void updateWindow();

	private slots:
		void armorSelected(const QString &name);
		void newArmor();
		void editArmor();
		void deleteArmor();
		void moveArmorUp();
		void moveArmorDown();

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