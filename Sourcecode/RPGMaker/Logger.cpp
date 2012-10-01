#include "qfile.h"
#include "qtextstream.h"
#include "Logger.h"

Logger *Logger::_instance = 0;

/**
* Creates an instance of the Logger if necessary and returns the pointer to the caller.
*/
Logger* Logger::instance() {
	if (!Logger::_instance)
		Logger::_instance = new Logger();

	return Logger::_instance;
}

/**
* Destroys the current instance of the Logger, if any.
*/
void Logger::destroy() {
	if (Logger::_instance)
		delete Logger::_instance;

	Logger::_instance = 0;
}

/**
* Private constructor
*/
Logger::Logger() 
	: _status(false), _messages(QVector<QString>(0)) {}

/**
* Private copy constructor
*/
Logger::Logger(const Logger&) {}

/**
* Private assignment operator
*/
Logger& Logger::operator=(const Logger&) {
	return *this;
}

/**
* Private destructor
*/
Logger::~Logger() {
	this->_messages.clear();
}

/**
* Sets the status of the project.
* Status set to true means changes have been made to the project.
* Status set to false means the project remains unchanged.
*/
void Logger::setStatus(bool status) {
	if (!this->_status && status) {
		emit this->projectModified();
	}

	this->_status = status;
}

/**
* Adds message to the list of log messages.
*/
void Logger::addMessage(QString message) {
	this->_messages.append(message);
}

/**
* Clear the list of log messages.
*/
void Logger::clearmessages() {
	this->_messages.clear();
}

/**
* Writes the list of messages to a log file and clears the list.
* Messages are written to the file log.txt. The list is cleared afterwards
* and the status is set to false, meaning the project does not contain any more changes.
*/
void Logger::flush() {
	QFile logfile("log.txt");
	if (logfile.open(QIODevice::WriteOnly | QIODevice::Append)) {
		QTextStream out (&logfile);
		for (int i = 0; i < this->_messages.size(); i++) {
			out << this->_messages.at(i) << "\n";
		}
		logfile.close();
	}

	this->_messages.clear();
	_status = false;
}

/**
* Returns te status of the Logger.
* Status is true means the project has been changed.
* Status is false means no changes have been made to the project.
*/
bool Logger::getStatus() const {
	return this->_status;
}