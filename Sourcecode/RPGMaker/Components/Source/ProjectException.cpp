#include "../Headers/ProjectException.h"

using namespace projectcomponents;

/**
*	Create a ProjectException with a specified message.
*/
ProjectException::ProjectException(QString message)
	: _message(message) {}

/**
*	Copy constructor.
*/
ProjectException::ProjectException(const ProjectException &exception)
	: _message(exception._message) {}

/**
*	Standard destructor.
*/
ProjectException::~ProjectException() {}

/**
*	Assignment operator...
*/
ProjectException& ProjectException::operator= (const ProjectException &exception) {
	this->_message = exception._message;
	return *this;
}

/**
*	Returns the error message.
*/
QString ProjectException::what() const {
	return this->_message;
}