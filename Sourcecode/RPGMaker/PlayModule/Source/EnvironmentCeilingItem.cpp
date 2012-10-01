#include "qvector.h"
#include "qpainter.h"

#include "../ProgramDefaults.h"
#include "../Headers/EnvironmentCeilingItem.h"

using namespace playmodule;

EnvironmentCeilingItem::EnvironmentCeilingItem(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent),
	_animationTimer(new QTimer(this)),
	_environment(0) {}

EnvironmentCeilingItem::~EnvironmentCeilingItem() {}

void EnvironmentCeilingItem::setEnvironment(Environment *environment) {
	QSize dimensions = environment->getDimensions();
	QVector<QPixmap> ceiling = environment->generateCeiling();

	QPixmap ceilingImage(dimensions.width()*g_tilesize, dimensions.height()*g_tilesize);
	ceilingImage.fill(Qt::transparent);
	QPainter ceilingPainter(&ceilingImage);
	ceilingPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);

	for (int y = 0; y < dimensions.height(); y++) {
		for (int x = 0; x < dimensions.width(); x++) {
			ceilingPainter.drawPixmap(x *g_tilesize, y * g_tilesize, ceiling.at(dimensions.width() *y + x));
		}
	}

	ceilingPainter.end();
	setPixmap(ceilingImage);
	_environment = environment;
}