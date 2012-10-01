#ifndef BATTLEITEM_H
#define BATTLEITEM_H

#include "qpair.h"
#include "qobject.h"
#include "qvector.h"
#include "qpixmap.h"
#include "qgraphicsitem.h"

#include "../Components/Headers/Class.h"
#include "../Components/Headers/Element.h"
#include "../Components/Headers/Attribute.h"

using projectcomponents::Class;
using projectcomponents::Element;
using projectcomponents::Attribute;

namespace playmodule {

	class BattleItem : public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:
		enum Type {CHARACTER, ENEMY};
		enum ResultType {VULNERABLE, RESIST, NONE, MISS};

		BattleItem(QGraphicsItem *parent, Type type);
		virtual ~BattleItem();

		int getLevel() const;
		int getExperience() const;
		Class* getClass() const;
		QVector<Element*> getElements() const;
		QVector<Ability*> getAbilities() const;

		QPair<Attribute*, int> getMaxHealth() const;
		QPair<Attribute*, int> getMaxEnergy() const;
		QPair<Attribute*, int> getHealth() const;
		QPair<Attribute*, int> getEnergy() const;
		QPair<Attribute*, int> getAgility() const;
		QPair<Attribute*, int> getEvasion() const;
		QPair<Attribute*, int> getOffensive(QString name) const;
		QPair<Attribute*, int> getDefensive(QString name) const;
		QPair<Attribute*, int> getPrimaryAttribute() const;
		
		QVector<QPair<Attribute*, int>> getOffensiveAttributes() const;
		QVector<QPair<Attribute*, int>> getDefensiveAttributes() const;

		Type getType() const;
		virtual QString getName() const =0;

		void setHealth(int health);
		void setEnergy(int energy);
		QPair<ResultType, int> takeHit(BattleItem *attacker, QVector<Element*> elements, QPair<Attribute*, int> offensiveAttribute);
		QPair<ResultType, int> recover(QVector<Element*> elements, QPair<Attribute*, int> offensiveAttribute);

	protected:
		int _level;
		int _experience;
		Class *_class;
		QPixmap _battleImage;
		QPixmap _environmentImage;
		QVector<Element*> _elements;
		QVector<Ability*> _abilities;
		QPair<Attribute*, int> _health;
		QPair<Attribute*, int> _energy;
		QVector<QPair<Attribute*, int>> _attributes;

	private:
		Type _type;

	signals:
		void valueChanged(QString name);

	};
}

#endif