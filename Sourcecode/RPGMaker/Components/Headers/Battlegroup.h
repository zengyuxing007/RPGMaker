#ifndef BATTLEGROUP_H
#define BATTLEGROUP_H

#include "qstring.h"
#include "qvector.h"
#include "qobject.h"

#include "Enemy.h"
#include "ProjectException.h"

namespace projectcomponents {

	/**
	*	The Battlegroup class holds an amount of enemies.
	*	They form a group that can be fought in the game.
	*	Every Battlegroup can have its own battle and victory music.
	*/
	class Battlegroup : public QObject {

		Q_OBJECT

	public:
		virtual ~Battlegroup();

		QString getName() const;
		QString getVictoryMusic() const;
		QString getBackgroundMusic() const;
		QVector<Enemy*> getEnemies() const;
		Enemy* getEnemy(QString name) const throw (ProjectException);
		Enemy* getEnemy(int position) const throw (ProjectException);

		void addEnemy(Enemy *enemy) throw (ProjectException);

		void setName(QString name) throw (ProjectException);
		void setVictoryMusic(QString name) throw (ProjectException);
		void setBackgroundMusic(QString name) throw (ProjectException);

		void removeEnemy(QString name) throw (ProjectException);
		void removeEnemy(int position) throw (ProjectException);

		static Battlegroup* create(QString name) throw (ProjectException);
		static Battlegroup* translateFromXML(QDomElement element) throw (ProjectException);
		static void translateToXML(Battlegroup *group, QDomDocument &document, QDomElement &parent);

	private:
		Battlegroup(QString name);

		QString _name;
		QString _backgroundMusic;
		QString _victoryMusic;
		QVector<Enemy*> _enemies;

	signals:
		void deleteSignal(QString name);

	private slots:
		void enemyDeleted(QString name);
	};
}

#endif