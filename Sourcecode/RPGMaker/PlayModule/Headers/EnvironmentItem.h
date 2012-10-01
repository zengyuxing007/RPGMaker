#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include "qpoint.h"
#include "qobject.h"
#include "qgraphicsitem.h"

#include "../Components/Headers/Object.h"
#include "../Components/Headers/Environment.h"

using projectcomponents::Object;
using projectcomponents::Environment;

namespace playmodule {

	class EnvironmentItem : public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:
		EnvironmentItem(QGraphicsItem *parent);
		virtual ~EnvironmentItem();

		void setObject(Object *object);
		void setEnvironment(Environment *environment);
		void setLocation(QPoint location);

	private:
		Object *_object;
		Environment *_environment;

	};
}

#endif