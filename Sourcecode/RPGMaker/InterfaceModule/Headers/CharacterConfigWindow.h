#ifndef CHARACTERCONFIGWINDOW_H
#define CHARACTERCONFIGWINDOW_H

#include "qlabel.h"
#include "qdialog.h"
#include "qspinbox.h"
#include "qlineedit.h"
#include "qcombobox.h"
#include "qboxlayout.h"

#include "../Components/Headers/Project.h"

using projectcomponents::Project;

namespace interfacemodule {

	/**
	*	The character configuration window allows to create and edit a given character.
	*/
	class CharacterConfigWindow : public QDialog {

		Q_OBJECT

	public:
		CharacterConfigWindow(QWidget *parent, Project *project);
		virtual ~CharacterConfigWindow();

		QString getName() const;
		int getLevel() const;
		QString getClass() const;
		QString getEnvironmentImage() const;
		QString getBattleImage() const;

		void setName(QString name);
		void setLevel(int level);
		void setClass(QString name);
		void setEnvironmentImage(QString image);
		void setBattleImage(QString image);

	private:
		void loadWindow();

		void createLeftPane();
		void createRightPane();

		Project *_project;
		QString _oldName;

		QHBoxLayout *_layout;
		QLineEdit *_nameField;
		QSpinBox *_levelSelection;
		QComboBox *_classSelection;
		QComboBox *_environmnentSelection;
		QComboBox *_battleSelection;
		QLabel *_environmentLabel;
		QLabel *_battleLabel;

	private slots:
		void checkInput();
		void environmentChanged(const QString &name);
		void battleChanged(const QString &name);
	};
}

#endif