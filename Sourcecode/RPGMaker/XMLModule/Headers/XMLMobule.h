#ifndef XMLMODULE_H
#define XMLMODULE_H

#include "qstring.h"
#include "qxmlschema.h"
#include "XMLException.h"
#include "XMLTranslator.h"
#include "ProgramDefaults.h"
#include "XMLMessageHandler.h"

using namespace xmlmodule;

namespace xmlmodule {

	class XMLModule {
	public:
		XMLModule();
		~XMLModule();

		Project* openProject(QString path) throw(XMLException);
		void writeProject(Project *project, QString path) throw(XMLException);

	private:
		bool loadProjectSchema();
		bool validateProject(const QString &projectstring);

		QXmlSchema *_projectSchema;
		XMLTranslator *_translator;
		XMLMessageHandler *_messageHandler;
	};

}

#endif