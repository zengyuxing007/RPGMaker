#ifndef ENEMYSYSTEM_H
#define ENEMYSYSTEM_H

#include "qobject.h"
#include "qvector.h"

#include "Enemy.h"

namespace projectcomponents {

	/**
	*	The EnemySystem class is a management system for <i>Enemy</i>.
	*/
	class EnemySystem : public QObject {

		Q_OBJECT

	public:
		virtual ~EnemySystem();

		const QVector<Enemy*>& getEnemies() const;
		Enemy* getEnemy(QString name) const throw (ProjectException);

		void addEnemy(Enemy *enemy) throw (ProjectException);

		void removeEnemy(QString name) throw (ProjectException);

		static EnemySystem* create();
		static EnemySystem* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(EnemySystem *system, QDomDocument &document, QDomElement &parent);

	private:
		EnemySystem();

		QVector<Enemy*> _enemies;
	};
}

#endif