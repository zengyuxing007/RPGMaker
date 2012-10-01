#ifndef CONTROLLOGGER_H
#define CONTROLLOGGER_H

#include "qstring.h"
#include "qvector.h"
#include "qobject.h"

/**
* Used to log system messages.
* There can only be one instance of this class at a time.
* The logger will record actions and save the status of the project.
*/
class Logger : private QObject {

	Q_OBJECT

public:
	void setStatus(bool status);
	void addMessage(QString message);
	void clearmessages();
	void flush();
	bool getStatus() const;

	static Logger* instance();
	static void destroy();

signals:
	void projectModified();

private:
	Logger();
	Logger(const Logger&);
	Logger& operator=(const Logger&);
	virtual ~Logger();

	bool _status;
	QVector<QString> _messages;
	static Logger *_instance;

};


#endif