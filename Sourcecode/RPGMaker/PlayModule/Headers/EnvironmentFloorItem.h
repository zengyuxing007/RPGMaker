#ifndef ENVIRONMENTFLOORITEM_H
#define ENVIRONMENTFLOORITEM_H

#include "qtimer.h"
#include "qgraphicsitem.h"

#include "../Components/Headers/Environment.h"

using projectcomponents::Environment;

namespace playmodule {

	class EnvironmentFloorItem : public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:
		EnvironmentFloorItem(QGraphicsItem *parent);
		virtual ~EnvironmentFloorItem();

		void setEnvironment(Environment *environment);

	private:
		QTimer *_animationTimer;
		Environment *_environment;

	};
}

#endif