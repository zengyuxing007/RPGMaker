#ifndef PROJECTEXCEPTION_H
#define PROJECTEXCEPTION_H

#include "qstring.h"

namespace projectcomponents {

	/**
	*	A ProjectException contains an error message.
	*	When this class is thrown, it denotes that an error has
	*	occured inside the management of the project components.
	*/
	class ProjectException {
	public:
		ProjectException(QString message) throw();
		ProjectException(const ProjectException &exception) throw();
		virtual ~ProjectException() throw();

		ProjectException& operator= (const ProjectException &exception) throw();
		virtual QString what() const throw();

	private:
		QString _message;
	};

}

#endif