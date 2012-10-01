#ifndef ABILTITIESWINDOW_H
#define ABILTITIESWINDOW_H

#include "qlabel.h"
#include "qframe.h"
#include "qspinbox.h"
#include "qcombobox.h"
#include "qboxlayout.h"
#include "qlistwidget.h"
#include "qplaintextedit.h"

#include "../Components/Headers/Properties.h"

using projectcomponents::Properties;

namespace interfacemodule {

	/**
	*	The abilities window allows to create and edit abilities.
	*/
	class AbilitiesWindow : public QFrame {

		Q_OBJECT

	public:
		AbilitiesWindow(QWidget *parent, Properties *properties);
		virtual ~AbilitiesWindow();

	private:
		void createWindow();
		void loadAbilities();

		Properties *_properties;

		QHBoxLayout *_layout;
		
		QListWidget *_abilitiesList;
		QLabel *_descLabel;
		QPlainTextEdit *_description;
		QLabel *_elemLabel;
		QListWidget *_elements;

	signals:
		void updateSignal();

	public slots:
		void updateWindow();

	private slots:
		void abilitySelected(const QString &name);
		void newAbility();
		void editAbility();
		void deleteAbility();
		void moveAbilityUp();
		void moveAbilityDown();

		void addElement();
		void deleteElement();
		void moveElementUp();
		void moveElementDown();

		void saveDescription();
		void reloadDescription();
	};
}

#endif