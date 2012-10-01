#ifndef TEXTEVENTWINDOW_H
#define TEXTEVENTWINDOW_H

#include "qlabel.h"
#include "qframe.h"
#include "qstring.h"
#include "qcombobox.h"
#include "qlistwidget.h"
#include "qbuttongroup.h"
#include "qplaintextedit.h"

#include "../Components/Headers/Project.h"

using projectcomponents::Project;

namespace interfacemodule {

	/**
	*	This window allows to select text events from a list and edit the object of the event
	*	and the text blocks of the selected text event.
	*/
	class TextEventWindow : public QFrame {

		Q_OBJECT

	public:
		TextEventWindow(QWidget *parent, Project *project);
		virtual ~TextEventWindow();

	private:
		void createWindow();
		void loadEvents();
		void loadMessages();

		Project *_project;
		
		QListWidget *_eventsList;
		QButtonGroup *_triggerButtons;
		QComboBox *_objectList;
		QLabel *_currentObject;
		QListWidget *_textList;
		QPlainTextEdit *_textField;

	public slots:
		void updateWindow();

	private slots:
		void eventSelected(const QString &name);
		void messageSelected(int index);
		void newTextEvent();
		void editTextEvent();
		void deleteTextEvent();
		void moveTextEventUp();
		void moveTextEventDown();
		void triggerChanged(int id);

		void objectSelected(const QString &);

		void newMessage();
		void deleteMessage();

		void saveMessage();
		void reloadMessage();
	};
}

#endif