#ifndef ENVIRONMENTCEILINGITEM_H
#define ENVIRONMENTCEILINGITEM_H

#include "qtimer.h"
#include "qgraphicsitem.h"

#include "../Components/Headers/Environment.h"

using projectcomponents::Environment;

namespace playmodule {

	class EnvironmentCeilingItem : public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:
		EnvironmentCeilingItem(QGraphicsItem *parent);
		virtual ~EnvironmentCeilingItem();

		void setEnvironment(Environment *environment);

	private:
		QTimer *_animationTimer;
		Environment *_environment;

	};
}

#endif