#ifndef ENEMY_H
#define ENEMY_H

#include "qpixmap.h"
#include "qobject.h"
#include "qstring.h"
#include "qvector.h"

#include "Class.h"
#include "Element.h"
#include "Ability.h"
#include "EnemyDrops.h"
#include "ProjectException.h"

using namespace projectcomponents;

namespace projectcomponents {

	/**
	*	The Enemy class is the computer controlled variant of the <i>Character</i> class.
	*/
	class Enemy : public QObject {

		Q_OBJECT

	public:
		virtual ~Enemy();

		QString getName() const;
		QString getDescription() const;
		int getLevel() const;
		Class* getClass() const;
		const QVector<Element*>& getElements() const;
		const QVector<Ability*>& getAbilities() const;
		QVector<QPair<Ability*, int>> getAllAbilities() const;
		QPair<QPixmap, QString> getBattleImage() const;
		QPair<QPixmap, QString> getEnvironmentImage() const;
		EnemyDrops* getDrops() const;

		void setName(QString name) throw (ProjectException);
		void setDescription(QString description);
		void setLevel(int exp);
		void setClass(Class *cat);
		void setBattleImage(QString imagename) throw (ProjectException);
		void setEnvironmentImage(QString imagename) throw (ProjectException);
		void setDrops(EnemyDrops *drops) throw (ProjectException);
		
		void addElement(Element *element) throw (ProjectException);
		void addAbility(Ability *ability) throw (ProjectException);

		void removeElement(Element *element) throw (ProjectException);
		void removeElement(QString name) throw (ProjectException);
		void removeAbility(Ability *ability) throw (ProjectException);
		void removeAbility(QString name) throw (ProjectException);

		void unsetClass();

		static Enemy* create(QString name) throw (ProjectException);
		static Enemy* translateFromXML(QDomElement &enemy)  throw (ProjectException);
		static void translateToXML(Enemy *enemy, QDomDocument &document, QDomElement &parent);

	private:
		Enemy(QString name);

		int _level;
		Class *_class;
		QString _name;
		EnemyDrops *_drops;
		QString _description;
		QVector<Element*> _elements;
		QVector<Ability*> _abilities;
		QPair<QPixmap, QString> _battleimage;
		QPair<QPixmap, QString> _environmentimage;

	signals:
		void deleteSignal(QString name);

	private slots:
		void deleteElement(QString name);
		void deleteAbility(QString name);
		void deleteClass(QString name);
	};

}

#endif