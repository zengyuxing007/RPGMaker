#include "../Headers/BattleBackgroundItem.h"

using namespace playmodule;

BattleBackgroundItem::BattleBackgroundItem(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent) {}

BattleBackgroundItem::~BattleBackgroundItem() {}

void BattleBackgroundItem::setBackground(const QPixmap &background) {
	setPixmap(background.scaled(QSize(800, 400)));
}