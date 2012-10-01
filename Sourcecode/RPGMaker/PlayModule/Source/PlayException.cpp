#include "../Headers/PlayException.h"

using namespace playmodule;

PlayException::PlayException(QString message)
	: _message(message) {}

PlayException::PlayException(const PlayException &exception)
	: _message(exception._message) {}

PlayException::~PlayException() {}

PlayException& PlayException::operator= (const PlayException &exception) {
	this->_message = exception._message;
	return *this;
}

QString PlayException::what() const {
	return this->_message;
}