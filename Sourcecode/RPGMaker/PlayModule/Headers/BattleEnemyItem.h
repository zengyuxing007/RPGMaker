#ifndef BATTLEENEMYITEM_H
#define BATTLEENEMYITEM_H

#include "BattleItem.h"
#include "../Components/Headers/Enemy.h"

using projectcomponents::Enemy;

namespace playmodule {

	class BattleEnemyItem : public BattleItem {

		Q_OBJECT

	public:

		BattleEnemyItem(QGraphicsItem *parent);
		virtual ~BattleEnemyItem();

		virtual QString getName() const;

		Enemy* getEnemy() const;
		void initialize(Enemy *enemy);

	private:
		Enemy *_enemy;
	};
}

#endif