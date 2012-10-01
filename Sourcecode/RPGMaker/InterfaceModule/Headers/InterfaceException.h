#ifndef INTERFACEEXCEPTION_H
#define INTERFACEEXCEPTION_H

#include "qstring.h"

namespace interfacemodule {

	/**
	*	The interface exception catches errors from the interface.
	*/
	class InterfaceException {
	public:
		InterfaceException(QString message) throw();
		InterfaceException(const InterfaceException &exception) throw();
		virtual ~InterfaceException() throw();

		InterfaceException& operator= (const InterfaceException &exception) throw();
		virtual QString what() const throw();

	private:
		QString _message;
	};

}

#endif