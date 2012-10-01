#ifndef CHARCTERSYSTEM_H
#define CHARCTERSYSTEM_H

#include "qobject.h"
#include "qvector.h"

#include "Character.h"

namespace projectcomponents {

	/**
	*	The CharacterSystem is a management system for <i>Character</i>.
	*/
	class CharacterSystem : public QObject {

		Q_OBJECT

	public:
		virtual ~CharacterSystem();

		const QVector<Character*>& getCharacters() const;
		Character* getCharacter(QString name) const throw (ProjectException);

		void addCharacter(Character *character) throw (ProjectException);

		void removeCharacter(QString name) throw (ProjectException);

		static CharacterSystem* create();
		static CharacterSystem* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(CharacterSystem *system, QDomDocument &document, QDomElement &parent);

	private:
		CharacterSystem();

		QVector<Character*> _characters;
	};
}

#endif