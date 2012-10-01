#ifndef TEXTEVENTHANDLER_H
#define TEXTEVENTHANDLER_H

#include "qevent.h"
#include "qobject.h"
#include "qgraphicsitem.h"

#include "../Components/Headers/TextEvent.h"

namespace playmodule {

	class TextEventHandler: public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:
		TextEventHandler(QGraphicsItem *parent);
		virtual ~TextEventHandler();

		int getIndex() const;
		TextEvent* getEvent() const;
		void setEvent(TextEvent *event);

		void nextMessage();

		void keyPressEvent(QKeyEvent *event);

	private:
		void createWindow();

		int _index;
		TextEvent *_event;
		QGraphicsTextItem *_textField;

	signals:
		void eventFinishedSignal();
	};
}
#endif