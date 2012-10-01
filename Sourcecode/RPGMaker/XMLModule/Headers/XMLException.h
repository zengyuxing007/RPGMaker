#ifndef XMLEXCEPTION_H
#define XMLEXCEPTION_H

#include "qstring.h"

namespace xmlmodule {

	class XMLException {
	public:

		XMLException(QString message) throw();
		XMLException(const XMLException &exception) throw();
		virtual ~XMLException() throw();
		
		XMLException& operator= (const XMLException &exception) throw();
		virtual QString what() const throw();

	private:
		QString _message;
	};

}

#endif