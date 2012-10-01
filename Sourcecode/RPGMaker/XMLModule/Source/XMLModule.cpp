#include "qfile.h"
#include "qfileinfo.h"
#include "../Logger.h"
#include "qxmlschema.h"
#include "qtextstream.h"
#include "qxmlschemavalidator.h"
#include "../Headers/XMLMobule.h"

using namespace xmlmodule;

XMLModule::XMLModule() 
	: _projectSchema(new QXmlSchema()),
	_translator(new XMLTranslator()),
	_messageHandler(new XMLMessageHandler()) {}

XMLModule::~XMLModule() {
	if (this->_translator)
		delete this->_translator;

	if (this->_messageHandler)
		delete this->_messageHandler;

	if (this->_projectSchema)
		delete this->_projectSchema;
}

Project* XMLModule::openProject(QString path) {
	QFile projectfile(path);
	QFileInfo projectfileinfo(projectfile);

	if (!projectfileinfo.exists())
		throw XMLException("Could not find file " + path);

	if (!projectfile.open(QIODevice::ReadOnly))
		throw XMLException("Could not read file " + path);

	if (!loadProjectSchema())
		throw XMLException("Failed to load the projectschema:\n" + _messageHandler->toString());
	_messageHandler->clearMessages();

	QString projectstring(projectfile.readAll());
	if (!validateProject(projectstring))
		throw XMLException("The projectdocument is invalid:\n" + _messageHandler->toString());
	_messageHandler->clearMessages();

	projectfile.close();
	return _translator->translateFromXML(projectstring);
}

void XMLModule::writeProject(Project *project, QString path) {
	QFile projectfile(path);

	if (!projectfile.open(QIODevice::WriteOnly | QIODevice::Text))
		throw XMLException("Could not open file " + path);

	if (!loadProjectSchema())
		throw XMLException("Failed to load the projectschema:\n" + _messageHandler->toString());
	_messageHandler->clearMessages();

	QString projectstring = _translator->translateToXML(project);
	if (!validateProject(projectstring))
		throw XMLException("The projectdocument is invalid:\n" + _messageHandler->toString());
	_messageHandler->clearMessages();

	QTextStream out(&projectfile);
	out << projectstring;
	projectfile.close();
}

bool XMLModule::loadProjectSchema() {
	QFile schemafile(g_schemaPath + "project.xsd");
	schemafile.open(QIODevice::ReadOnly);

	this->_projectSchema->setMessageHandler(this->_messageHandler);
	this->_projectSchema->load(&schemafile);

	if (!this->_projectSchema->isValid())
		return false;
	else
		return true;
}

bool XMLModule::validateProject(const QString &projectstring) {
	QXmlSchemaValidator validator(*this->_projectSchema);
	validator.setMessageHandler(this->_messageHandler);
	return validator.validate(projectstring.toUtf8());
}