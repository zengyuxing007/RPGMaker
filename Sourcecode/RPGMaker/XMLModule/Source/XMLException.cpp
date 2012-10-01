#include "../Headers/XMLException.h"

using namespace xmlmodule;

XMLException::XMLException(QString message) 
	: _message(message) {}

XMLException::XMLException(const XMLException &exception)
	: _message(exception.what()) {}

XMLException::~XMLException() {}

XMLException& XMLException::operator=(const XMLException &exception) {
	this->_message = exception._message;
	return *this;
}

QString XMLException::what() const {
	return this->_message;
}