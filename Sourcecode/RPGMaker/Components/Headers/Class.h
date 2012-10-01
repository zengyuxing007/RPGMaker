#ifndef CLASS_H
#define CLASS_H

#include "qobject.h"
#include "qstring.h"
#include "qvector.h"
#include "Ability.h"
#include "Attribute.h"
#include "ProjectException.h"

using namespace projectcomponents;

namespace projectcomponents {

	/**
	*	The Class class defines how a character progresses physically in the game.
	*	It contains a pointer to every <i>Attribute</i> create in te game
	*	and has a list of <i>Ability</i> which the character can learn.
	*/
	class Class : public QObject {

		Q_OBJECT

	public:
		~Class();

		QString getName() const;
		QString getDescription() const;
		Attribute* getPrimaryAttribute() const;
		const QVector<QPair<Attribute*, QPair<int, double>>>& getAttributes() const;
		const QVector<QPair<Ability*, int>>& getAbilities() const;
		QPair<Attribute*, QPair<int, double>> getAttribute(QString name) const;
		QPair<Ability*, int> getAbility(QString name) const;

		void setName(QString name) throw (ProjectException);
		void setDescription(QString newDescription);
		void setPrimaryAttribute(Attribute *attribute) throw (ProjectException);

		void addAttribute(Attribute *newAttribute, int initial, double value) throw (ProjectException);
		void addAttribute(QPair<Attribute*, QPair<int, double>> newAttribute) throw (ProjectException);
		void addAbility(Ability *newAbility, int acquire) throw (ProjectException);
		void addAbility(QPair<Ability*, int> newAbility) throw (ProjectException);

		void editAttribute(QString name, int initial, double multiplier) throw (ProjectException);
		void editAbility(QString name, int acquire) throw (ProjectException);

		void removeAttribute(Attribute *oldAttribute) throw (ProjectException);
		void removeAttribute(QString oldAttribute) throw (ProjectException);
		void removeAbility(Ability *oldAbility) throw (ProjectException);
		void removeAbility(QString oldAbility) throw (ProjectException);

		void unsetPrimaryAttribute();

		static Class* create(QString name) throw (ProjectException);
		static Class* translateFromXML(QDomElement &cat)  throw (ProjectException);
		static void translateToXML(Class *cat, QDomDocument &document, QDomElement &parent);

	private:
		Class(QString name);

		QString _name;
		QString _description;
		Attribute *_primaryAttribute;
		QVector<QPair<Ability*, int>> _abilities;
		QVector<QPair<Attribute*, QPair<int, double>>> _attributes;

	signals:
		void deleteSignal(QString name);

	private slots:
		void deleteAttribute(QString name);
		void deleteAbility(QString name);
	};

}

#endif