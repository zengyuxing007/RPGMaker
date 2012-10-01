#ifndef BATTLEEVENTWINDOW_H
#define BATTLEEVENTWINDOW_H

#include "qlabel"
#include "qframe.h"
#include "qstring.h"
#include "qcombobox.h"
#include "qlistwidget.h"
#include "qtablewidget.h"
#include "qbuttongroup.h"
#include "phonon/mediaobject.h"
#include "phonon/audiooutput.h"

#include "../Components/Headers/Project.h"

using projectcomponents::Project;

namespace interfacemodule {

	/**
	*	The battle event window allows to select battle events from a list
	*	and edit the battlegroups and the locations where it should appear.
	*/
	class BattleEventWindow : public QFrame {

		Q_OBJECT

	public:
		BattleEventWindow(QWidget *parent, Project *project);
		virtual ~BattleEventWindow();

	private:
		void createWindow();
		void loadEvents();
		void loadBattlegroups();
		void paintEnvironment(Environment *environment);
		void paintTile(Environment *environment, QPoint location);
		void markTile(Environment *environment, QPoint location);

		Project *_project;

		QListWidget *_eventsList;
		QButtonGroup *_triggerButtons;
		QListWidget *_groupsList;
		QComboBox *_environmentList;
		QTableWidget *_environment;
		QComboBox *_objectList;
		QLabel *_currentObject;
		QComboBox *_musicList;
		Phonon::MediaObject *_mediaObject;
		Phonon::AudioOutput *_audioOuput;

	public slots:
		void updateWindow();

	private slots:
		void eventSelected(const QString &name);
		void newBattleEvent();
		void editBattleEvent();
		void deleteBattleEvent();
		void moveBattleEventUp();
		void moveBattleEventDown();
		void triggerChanged(int id);

		void playMusic();
		void stopMusic();

		void objectSelected(const QString &name);

		void addBattlegroup();
		void removeBattlegroup();
		void moveBattlegroupUp();
		void moveBattlegroupDown();

		void environmentSelected(const QString &name);
		void environmentCellClicked(int row, int column);
	};
}

#endif