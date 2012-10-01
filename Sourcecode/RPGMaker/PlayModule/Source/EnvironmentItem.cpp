#include "../ProgramDefaults.h"
#include "../Headers/EnvironmentItem.h"

using namespace playmodule;

EnvironmentItem::EnvironmentItem(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent), _object(0), _environment(0) {}

EnvironmentItem::~EnvironmentItem() {}

void EnvironmentItem::setObject(Object *object) {
	_object = object;
	setPixmap(object->getImage());
}

void EnvironmentItem::setEnvironment(Environment *environment) {
	_environment = environment;
}

void EnvironmentItem::setLocation(QPoint location) {
	QPixmap objectImage = _object->getImage();
	QSize dimensions = objectImage.size();

	QPoint newPos;
	newPos.setX(location.x() - dimensions.width()/2);
	newPos.setY(location.y() - dimensions.height() + g_tilesize/2);

	setPos(newPos);
}