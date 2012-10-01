#ifndef CLASSESWINDOW_H
#define CLASSESWINDOW_H

#include "qlabel.h"
#include "qframe.h"
#include "qboxlayout.h"
#include "qlistwidget.h"
#include "qplaintextedit.h"

#include "../Components/Headers/Properties.h"

using projectcomponents::Properties;

namespace interfacemodule {

	/**
	*	The classes window allows to create and edit classes.
	*/
	class ClassesWindow : public QFrame {

		Q_OBJECT

	public:
		ClassesWindow(QWidget *parent, Properties *properties);
		virtual ~ClassesWindow();

	private:
		void createWindow();
		void loadClasses();

		Properties *_properties;

		QHBoxLayout *_layout;

		QListWidget *_classList;
		QLabel *_classDescLabel;
		QPlainTextEdit *_classDesc;
		QLabel *_classAttLabel;
		QListWidget *_classAttributes;
		QLabel *_classAbLabel;
		QListWidget *_classAbilities;

	signals:
		void updateSignal();

	public slots:
		void updateWindow();

	private slots:
		void classSelected(const QString &name);
		void newClass();
		void editClass();
		void deleteClass();
		void moveClassUp();
		void moveClassDown();

		void saveClassDescription();
		void reloadClassDescription();

		void editAttribute();

		void newAbility();
		void editAbility();
		void deleteAbility();
		void moveAbilityUp();
		void moveAbilityDown();
	};
}
#endif