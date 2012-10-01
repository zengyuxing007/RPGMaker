#ifndef NEWENVIRONMENTWINDOW_H
#define NEWENVIRONMENTWINDOW_H

#include "qpair.h"
#include "qlabel.h"
#include "qstring.h"
#include "qdialog.h"
#include "qspinbox.h"
#include "qcombobox.h"
#include "qlineedit.h"
#include <phonon/audiooutput.h>
#include <phonon/seekslider.h>
#include <phonon/mediaobject.h>
#include <phonon/volumeslider.h>
#include <phonon/backendcapabilities.h>

#include "../Components/Headers/Project.h"

using projectcomponents::Project;

namespace interfacemodule {

	/**
	*	The environment configuration window allows to create or edit a given environment.
	*	It gives access to the dimensions of the environment, its battleback image and background music.
	*/
	class EnvironmentConfigWindow : public QDialog {

		Q_OBJECT

	public:
		EnvironmentConfigWindow(QWidget *parent, Project *project);
		virtual ~EnvironmentConfigWindow();

		QString getName() const;
		int getWidth() const;
		int getHeight() const;
		QString getBackgroundMusic() const;
		QString getBattleback() const;

		void setName(QString name);
		void setWidth(int width);
		void setHeight(int height);
		void setBackgroundMusic(QString title);
		void setBattleback(QString name);

	private:
		void createWindow();
		void loadWindow();

		Project *_project;
		QString _oldname;

		QLineEdit *_nameInput;
		QSpinBox *_widthInput;
		QSpinBox *_heightInput;
		QComboBox *_musicSelection;
		Phonon::MediaObject *_backgroundMusicObject;
		Phonon::AudioOutput *_audioOutputObject;

		QComboBox *_battlebackSelection;
		QLabel *_battlebackLabel;

	private slots:
		void playMusic();
		void stopMusic();

		void battlebackChanged(const QString &name);

		void checkInput();
	};

}

#endif