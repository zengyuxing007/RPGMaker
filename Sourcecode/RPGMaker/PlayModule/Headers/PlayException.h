#ifndef PLAYEXCEPTION_H
#define PLAYEXCEPTION_H

#include "qstring.h"

namespace playmodule {

	class PlayException {
	public:
		PlayException(QString message) throw();
		PlayException(const PlayException &exception) throw();
		virtual ~PlayException() throw();

		PlayException& operator= (const PlayException &exception) throw();
		virtual QString what() const throw();

	private:
		QString _message;
	};

}

#endif