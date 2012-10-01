#ifndef ATTRIBUTESYSTEM_H
#define ATTRIBUTESYSTEM_H

#include "qdom.h"
#include "qpair.h"
#include "qobject.h"
#include "qvector.h"
#include "qstring.h"

#include "Attribute.h"
#include "ProjectException.h"

using namespace projectcomponents;

namespace projectcomponents {

	/**
	*	The AttributeSystem class is a management system for <i>Attribute</i>.
	*	The AttributeSystem class will always create 4 standard attributes that can only be changed in name.
	*/
	class AttributeSystem : public QObject {

		Q_OBJECT

	public:
		virtual ~AttributeSystem();

		Attribute* getAttribute(QString name) const;
		Attribute* getLifeSource() const;
		Attribute* getEnergySource() const;
		Attribute* getAgilityForce() const;
		Attribute* getEvasionForce() const;
		Attribute* getOffensiveAttribute(QString name) const;
		Attribute* getDefensiveAttribute(QString name) const;
		QVector<Attribute*> getAttributes() const;
		const QVector<Attribute*>& getOffensiveAttributes() const;
		const QVector<Attribute*>& getDefensiveAttributes() const;
		QVector<QPair<Attribute*, Attribute*>> getLinks() const;

		void addOffensiveAttribute(Attribute *attribute) throw (ProjectException);
		void addDefensiveAttribute(Attribute *attribute) throw (ProjectException);
		void addLink(Attribute *offensive, Attribute *defensive) throw (ProjectException);

		void removeOffensiveAttribute(QString name) throw (ProjectException);
		void removeDefensiveAttribute(QString name) throw (ProjectException);
		void removeLink(QString offensive, QString defensive) throw (ProjectException);

		bool isRegistered(Attribute *attribute);
		bool isRegistered(QString name);

		static AttributeSystem* create();
		static AttributeSystem* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(AttributeSystem *attributeSystem, QDomDocument &document, QDomElement &parent);

	private:
		AttributeSystem();

		Attribute *_lifeSource;
		Attribute *_energySource;
		Attribute *_agilityForce;
		Attribute *_evasionForce;
		QVector<Attribute*> _offensiveAttributes;
		QVector<Attribute*> _defensiveAttributes;

	signals:
		void addedAttribute(Attribute *attribute);
	};

}

#endif