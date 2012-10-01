#include "../Headers/InterfaceException.h"

using namespace interfacemodule;

InterfaceException::InterfaceException(QString message)
	: _message(message) {}

InterfaceException::InterfaceException(const InterfaceException &exception)
	: _message(exception._message) {}

InterfaceException::~InterfaceException() {}

InterfaceException& InterfaceException::operator= (const InterfaceException &exception) {
	this->_message = exception._message;
	return *this;
}

QString InterfaceException::what() const {
	return this->_message;
}