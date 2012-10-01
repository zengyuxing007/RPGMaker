#include "qvector.h"
#include "qpainter.h"

#include "../ProgramDefaults.h"
#include "../Headers/EnvironmentFloorItem.h"

using namespace playmodule;

EnvironmentFloorItem::EnvironmentFloorItem(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent),
	_animationTimer(new QTimer(this)),
	_environment(0) {}

EnvironmentFloorItem::~EnvironmentFloorItem() {}

void EnvironmentFloorItem::setEnvironment(Environment *environment) {
	QSize dimensions = environment->getDimensions();
	QVector<QPixmap> floor = environment->generateFloor();

	QPixmap floorImage(dimensions.width()*g_tilesize, dimensions.height()*g_tilesize);
	floorImage.fill(Qt::transparent);
	QPainter floorPainter(&floorImage);
	floorPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);

	for (int y = 0; y < dimensions.height(); y++) {
		for (int x = 0; x < dimensions.width(); x++) {
			floorPainter.drawPixmap(x *g_tilesize, y * g_tilesize, floor.at(dimensions.width() *y + x));
		}
	}

	floorPainter.end();
	setPixmap(floorImage);
	_environment = environment;
}