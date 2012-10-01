#ifndef CHARACTERSWINDOW_H
#define CHARACTERSWINDOW_H

#include "qlabel.h"
#include "qframe.h"
#include "qlineedit.h"
#include "qcombobox.h"
#include "qboxlayout.h"
#include "qlistwidget.h"
#include "qtablewidget.h"
#include "qplaintextedit.h"

#include "../ProgramDefaults.h"
#include "../Components/Headers/Project.h"

using projectcomponents::Project;

namespace interfacemodule {

	/**
	*	The character window allows to select character from a list and edit the selected character's biography, description, abilities and elements.
	*/
	class CharactersWindow : public QFrame {
		
		Q_OBJECT

	public:
		CharactersWindow(QWidget *parent, Project *project);
		virtual ~CharactersWindow();

	private:
		void createWindow();
		void loadCharacters();

		Project *_project;

		QListWidget *_characterSelection;
		QLabel *_biographyLabel;
		QPlainTextEdit *_biographyField;
		QLabel *_elementsLabel;
		QListWidget *_elementsList;
		QLabel *_abilitiesLabel;
		QListWidget *_abilitiesList;
		QLabel *_equipmentLabel;
		QTableWidget *_equipmentList;
		QLabel *_envImageLabel;
		QLabel *_battleImageLabel;

	private slots:
		void characterSelected(const QString &name);
		void newCharacter();
		void editCharacter();
		void deleteCharacter();
		void moveCharacterUp();
		void moveCharacterDown();

		void saveBiography();
		void reloadBiography();

		void addElement();
		void deleteElement();
		void moveElementUp();
		void moveElementDown();

		void addAbility();
		void editAbility();
		void deleteAbility();
		void moveAbilityUp();
		void moveAbilityDown();

		void editEquipment();
		void deleteEquipment();
	};
}

#endif