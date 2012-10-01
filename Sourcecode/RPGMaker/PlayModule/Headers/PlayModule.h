#ifndef PLAYMODULE_H
#define PLAYMODULE_H

#include "qmainwindow.h"

#include "GameView.h"
#include "GameEvent.h"
#include "PlayException.h"
#include "../Components/Headers/Project.h"

using projectcomponents::Project;

namespace playmodule {

	class PlayModule: public QMainWindow {

		Q_OBJECT

	public:
		virtual ~PlayModule();

		void play();
		bool eventFilter(QObject *, QEvent *);

		static PlayModule* create(QWidget *parent, Project *project) throw (PlayException);
		static bool checkPlayability(Project *project) throw (PlayException);

	protected:
		virtual void closeEvent(QCloseEvent *event);

	private:
		QVector<GameEvent*> buildGameEvents();

	private:
		PlayModule(QWidget *parent, Project *project);
		GameView *_view;
		Project *_project;
	};
}

#endif