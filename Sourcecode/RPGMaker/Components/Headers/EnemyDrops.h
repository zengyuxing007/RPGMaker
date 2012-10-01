#ifndef ENEMYDROPS_H
#define ENEMYDROPS_H

#include "qdom.h"
#include "qpair.h"
#include "qobject.h"
#include "qvector.h"

#include "Armor.h"
#include "Weapon.h"
#include "Accessory.h"
#include "ProjectException.h"

using namespace projectcomponents;

namespace projectcomponents {

	/**
	*	The EnemyDrops class contains the currency, experience and items that the enemy it belongs to can drop.
	*/
	class EnemyDrops : public QObject {

		Q_OBJECT

	public:
		virtual ~EnemyDrops();

		int getExperience() const;
		QPair<int, int> getCurrency() const;
		const QVector<QPair<EquipmentPiece*, int>>& getItems() const;
		QPair<EquipmentPiece*, int> getItem(int position) const;

		void setExperience(int experience) throw (ProjectException);
		void setCurrency(QPair<int, int> currency) throw (ProjectException);

		void addItem(EquipmentPiece *item, int chance) throw (ProjectException);
		void editItem(int position, int chance) throw (ProjectException);
		void removeItem(int position) throw (ProjectException);

		static EnemyDrops* create() throw (ProjectException);
		static EnemyDrops* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(EnemyDrops *drops, QDomDocument &document, QDomElement &parent);

	private:
		EnemyDrops();
		int _experience;
		QPair<int, int> _currency;
		QVector<QPair<EquipmentPiece*, int>> _drops;

	private slots:
		void deleteItem(QString name);

	};
}

#endif