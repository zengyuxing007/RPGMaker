#ifndef ENEMIESWINDOW_H
#define ENEMIESWINDOW_H

#include "qlabel.h"
#include "qframe.h"
#include "qlineedit.h"
#include "qcombobox.h"
#include "qboxlayout.h"
#include "qlistwidget.h"
#include "qplaintextedit.h"

#include "../ProgramDefaults.h"
#include "../Components/Headers/Project.h"

using projectcomponents::Project;

namespace interfacemodule {

	/**
	*	The enemy windows allows to select an enemy from a list and edit its description, item drops, abilities and elements.
	*/
	class EnemiesWindow : public QFrame {

		Q_OBJECT

	public:
		EnemiesWindow(QWidget *parent, Project *project);
		virtual ~EnemiesWindow();

	private:
		void createWindow();
		void loadEnemies();

		Project *_project;

		QListWidget *_enemySelection;
		QLabel *_descriptionLabel;
		QPlainTextEdit *_descriptionField;
		QLabel *_elementsLabel;
		QListWidget *_elementsList;
		QLabel *_abilitiesLabel;
		QListWidget *_abilitiesList;
		QLabel *_dropsLabel;
		QListWidget *_dropList;
		QLabel *_envImageLabel;
		QLabel *_battleImageLabel;

	private slots:
		void enemySelected(const QString &name);
		void newEnemy();
		void editEnemy();
		void deleteEnemy();
		void battlegroupDialog();
		void moveEnemyUp();
		void moveEnemyDown();

		void saveDescription();
		void reloadDescription();

		void addElement();
		void deleteElement();
		void moveElementUp();
		void moveElementDown();

		void addAbility();
		void deleteAbility();
		void moveAbilityUp();
		void moveAbilityDown();

		void newDrop();
		void editDrop();
		void deleteDrop();
		void moveDropUp();
		void moveDropDown();
	};
}

#endif