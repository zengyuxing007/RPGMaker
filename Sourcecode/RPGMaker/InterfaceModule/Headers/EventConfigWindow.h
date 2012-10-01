#ifndef EVENTCONFIGWINDOW_H
#define EVENTCONFIGWINDOW_H

#include "qlabel.h"
#include "qdialog.h"
#include "qspinbox.h"
#include "qlineedit.h"
#include "qcombobox.h"
#include "qboxlayout.h"
#include "qlistwidget.h"

#include "../Components/Headers/Project.h"

using projectcomponents::Project;

namespace interfacemodule {

	/**
	*	The event configuration window allows to create or edit a given event.
	*	In the case of a battle event, it also allows to edit the activation value.
	*/
	class EventConfigWindow : public QDialog {

		Q_OBJECT
			
	public:
		EventConfigWindow(QWidget *parent, Project *project, Event::Type type);
		virtual ~EventConfigWindow();

		QString getName() const;
		int getExecutionLimit() const;
		Event *getFollowupEvent() const;
		int getBattleEventActivation() const;
		QVector<Event*> getRequirementEvents() const;

		void setEvent(Event *event);

	private:
		void createWindow(Event::Type type);
		void loadWindow(Event::Type type);

		Project *_project;
		Event *_event;
		QString _oldName;

		QLineEdit *_nameField;
		QSpinBox *_executionLimitSelection;
		QComboBox *_followpupEventSelection;
		QSpinBox *_battleEventActivationSelection;
		QListWidget *_requirementEventSelection;

	private slots:
		void checkInput();
		void addRequirementEvent();
		void removeRequirementEvent();
	};
}

#endif