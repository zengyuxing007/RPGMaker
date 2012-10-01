#ifndef XMLTRANSLATOR_H
#define XMLTRANSLATOR_H

#include "qdom.h"
#include "qfile.h"
#include "qstring.h"
#include "XMLException.h"
#include "../Components/Headers/Project.h"

using namespace projectcomponents;

namespace xmlmodule {

	class XMLTranslator {
	public:

		XMLTranslator();
		~XMLTranslator();

		QString translateToXML(Project *project);
		Project* translateFromXML(QString projectstring) throw(XMLException);

	private:
		Project *_project;
		QDomDocument _document;

		void translateDependencies(QDomElement &root);
		void translatePropertiesDependencies(QDomElement &properties);
		void translateAttributeDependencies(QDomElement &attributes);
		void translateElementDependencies(QDomElement &elements);
		void translateAbilityDependencies(QDomElement &abilities);
		void translateClassDependencies(QDomElement &classes);
		void translateEquipmentDependencies(QDomElement &equipment);
		void translateWeaponDependencies(QDomElement &weapons);
		void translateArmorDependencies(QDomElement &armor);
		void translateAccessoryDependencies(QDomElement &accessory);
		void translateItemDependencies(QDomElement &items);
		void translateCharacterDependencies(QDomElement &characters);
		void translateEnemyDependencies(QDomElement &enemies);
		void translateBattlegroupDependencies(QDomElement &battlegroups);
		void translateEventDependencies(QDomElement &events);
		void translateEnvironmentDependencies(QDomElement &environments);
		void translateStartEventDependencies(QDomElement &startEvent);
	};

}

#endif