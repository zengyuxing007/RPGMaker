#ifndef BATTLEGROUPSDIALOG_H
#define BATTLEGROUPSDIALOG_H

#include "qdialog.h"
#include "qcombobox.h"
#include "qlistwidget.h"
#include "qtablewidget.h"
#include <phonon/audiooutput.h>
#include <phonon/seekslider.h>
#include <phonon/mediaobject.h>
#include <phonon/volumeslider.h>
#include <phonon/backendcapabilities.h>

#include "../Components/Headers/Project.h"

using projectcomponents::Project;

namespace interfacemodule {

	/**
	*	The battlegroups window allows to create and edit battlegroups by adding or removing enemies.
	*/
	class BattlegroupsDialog : public QDialog {

		Q_OBJECT

	public:
		BattlegroupsDialog(QWidget *parent, Project *project);
		virtual ~BattlegroupsDialog();

	private:
		void createWindow();
		void loadBattlegroups();

		Project *_project;

		QListWidget *_groupsList;
		QTableWidget *_membersList;
		QComboBox *_backgroundmusicSelection;
		QComboBox *_victorymusicSelection;
		Phonon::MediaObject *_backgroundMusicObject;
		Phonon::AudioOutput *_audioOutputObject;

	private slots:
		void groupSelected(const QString &name);
		void newBattlegroup();
		void editBattlegroup();
		void deleteBattlegroup();

		void addEnemy();
		void removeEnemy();

		void backgroundMusicSelected(const QString &name);
		void playBackgroundMusic();
		void stopBackgroundMusic();

		void victoryMusicSelected(const QString &name);
		void playVictoryMusic();
		void stopVictoryMusic();
	};
}

#endif