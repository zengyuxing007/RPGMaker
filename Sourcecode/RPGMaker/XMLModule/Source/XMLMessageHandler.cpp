#include "../Headers/XMLMessageHandler.h"

using namespace xmlmodule;

XMLMessageHandler::XMLMessageHandler(QObject *parent)
	: QAbstractMessageHandler(parent), _messages(QVector<QString>(0)) {}

XMLMessageHandler::~XMLMessageHandler() {
	this->_messages.clear();
}

void XMLMessageHandler::handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const QSourceLocation &sourceLocation ) {
	QString newMsg;
	if (type == QtDebugMsg)
		newMsg.append("Debug: ");
	else if (type == QtWarningMsg)
		newMsg.append("Warning: ");
	else if (type == QtCriticalMsg)
		newMsg.append("Critical: ");
	else if (type == QtFatalMsg)
		newMsg.append("Fatal: ");

	newMsg.append(description);
	newMsg.append(" at location: line " + QString::number(sourceLocation.line()) + QString(", column ") + QString::number(sourceLocation.column()));

	this->_messages.append(newMsg);
}

QVector<QString> XMLMessageHandler::getMessages() const {
	return this->_messages;
}

QString XMLMessageHandler::toString() const {
	QString msg;

	if (!this->_messages.empty()) {
		for (int i = 0; i < this->_messages.size(); i++) {
			msg.append(this->_messages.at(i) + "\n");
		}
	}

	return msg;
}

void XMLMessageHandler::clearMessages() {
	this->_messages.clear();
}