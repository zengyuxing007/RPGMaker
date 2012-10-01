#ifndef CHARACTER_H
#define CHARACTER_H

#include "qpair.h"
#include "qobject.h"
#include "qstring.h"
#include "qvector.h"
#include "qpixmap.h"

#include "Class.h"
#include "Element.h"
#include "Ability.h"
#include "ProjectException.h"
#include "CharacterEquipment.h"

namespace projectcomponents {

	/**
	*	A Character is the enitity in the game over which the player has direct control.
	*	A Character is of a certain class, can be attuned to elements, can learn abilities and wear equipment.
	*/
	class Character : public QObject {

		Q_OBJECT

	public:
		~Character();

		QString getName() const;
		QString getBiography() const;
		Class* getClass() const;
		int getLevel() const;
		Element* getElement(QString name) const;
		QPair<Ability*, int> getAbility(QString name) const;
		const QVector<Element*>& getElements() const;
		const QVector<QPair<Ability*, int>>& getAbilities() const;
		QVector<QPair<Ability*, int>> getAllAbilities() const;
		QPair<QPixmap, QString> getBattleImage() const;
		QPair<QPixmap, QString> getEnvironmentImage() const;
		CharacterEquipment* getEquipment() const;

		void setName(QString name) throw (ProjectException);
		void setBiography(QString newBiography);
		void setClass(Class *newClass);
		void setLevel(int level);
		void setBattleImage(QString imagename) throw (ProjectException);
		void setEnvironmentImage(QString imagename) throw (ProjectException);
		void setEquipment(CharacterEquipment *equipment) throw (ProjectException);

		void addAbility(Ability *newAbility, int acquire) throw (ProjectException);
		void addAbility(QPair<Ability*, int> newAbility) throw (ProjectException);
		void addElement(Element *newElement) throw (ProjectException);

		void editAbility(QString ability, int acquire) throw (ProjectException);

		void removeAbility(Ability *oldAbility) throw (ProjectException);
		void removeAbility(QString oldAbility) throw (ProjectException);
		void removeElement(Element *oldElement) throw (ProjectException);
		void removeElement(QString oldElement) throw (ProjectException);

		void unsetClass();

		static Character* create(QString name) throw (ProjectException);
		static Character* translateFromXML(QDomElement &character)  throw (ProjectException);
		static void translateToXML(Character *character, QDomDocument &document, QDomElement &parent);

	private:
		Character(QString name);

		int _level;
		Class *_class;
		QString _name;
		QString _biography;
		QVector<Element*> _elements;
		CharacterEquipment *_equipment;
		QPair<QPixmap, QString> _battleimage;
		QVector<QPair<Ability*, int>> _abilities;
		QPair<QPixmap, QString> _environmentimage;

	signals:
		void deleteSignal(QString name);

	private slots:
		void deleteAbility(QString name);
		void deleteElement(QString name);
		void deleteClass(QString name);
	};
}

#endif