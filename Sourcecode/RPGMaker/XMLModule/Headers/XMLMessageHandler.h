#ifndef XMLMESSAGEHANDLER_H
#define XMLMESSAGEHANDLER_H

#include "qstring.h"
#include "qvector.h"
#include "qabstractmessagehandler.h"

namespace xmlmodule {

	class XMLMessageHandler : public QAbstractMessageHandler {
	public:
		XMLMessageHandler(QObject *parent = 0);
		virtual ~XMLMessageHandler();

		void handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const QSourceLocation &sourceLocation );
		QVector<QString> getMessages() const;
		QString toString() const;
		void clearMessages();

	private:
		QVector<QString> _messages;
	};

}

#endif