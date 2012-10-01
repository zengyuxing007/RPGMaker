#ifndef BATTLEAIBATTLERINFO_H
#define BATTLEAIBATTLERINFO_H

#include "qpair.h"
#include "qvector.h"

#include "../Components/Headers/Element.h"
#include "../Components/Headers/Attribute.h"
#include "../PlayModule/Headers/BattleItem.h"

using playmodule::BattleItem;
using projectcomponents::Element;
using projectcomponents::Attribute;

namespace aimodule {

	/**
	*	This class is a container class for battler information.
	*	It keeps track of which information is available about the AI's opponent.
	*/
	class BattleAIBattlerInfo {
	public:

		enum ElementStatus {VULNERABLE, RESIST, NONE};

		BattleAIBattlerInfo(BattleItem *battler);
		virtual ~BattleAIBattlerInfo();

		QPair<Attribute*, int> getOffensiveStatus(QString name) const;
		QPair<Attribute*, int> getDefensiveStatus(QString name) const;
		QPair<Element*, ElementStatus> getElementStatus(QString name) const;

		void setOffensiveStatus(QPair<Attribute*, int> attribute);
		void setDefensiveStatus(QPair<Attribute*, int> attribute);
		void setElementStatus(QPair<Element*, ElementStatus> element);

	private:
		BattleItem *_battler;
		QVector<QPair<Attribute*, int>> _offensiveAttributes;
		QVector<QPair<Attribute*, int>> _defensiveAttributes;
		QVector<QPair<Element*, ElementStatus>> _elements;
	};
}

#endif