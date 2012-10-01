#include "qpoint.h"

#include "../ProgramDefaults.h"
#include "../Headers/BattleTargetPointer.h"

using namespace playmodule;

BattleTargetPointer::BattleTargetPointer(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent), _target(0) {

		_hostilePointer = QPixmap(g_windowskinPath + "001-Blue01.png").copy(128, 96, 32, 32);
		_friendlyPointer = QPixmap(g_windowskinPath + "001-Blue01.png").copy(160, 96, 32, 32);
}

BattleTargetPointer::~BattleTargetPointer() {}

BattleItem* BattleTargetPointer::getTarget() const {
	return _target;
}

void BattleTargetPointer::setTarget(BattleItem *target) {
	_target = target;

	if (_target->getType() == BattleItem::CHARACTER) {
		setPixmap(_friendlyPointer);
		QPointF location = target->pos();
		location.setX(location.x() + _target->boundingRect().width()/2 - boundingRect().width()/2);
		location.setY(location.y() + _target->boundingRect().height());
		setPos(location);

	} else if (_target->getType() == BattleItem::ENEMY) {
		setPixmap(_hostilePointer);
		QPointF location = target->pos();
		location.setX(location.x() + _target->boundingRect().width()/2 - boundingRect().width()/2);
		location.setY(location.y() + _target->boundingRect().height());
		setPos(location);
	}
}