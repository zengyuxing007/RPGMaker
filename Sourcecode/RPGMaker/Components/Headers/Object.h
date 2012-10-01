#ifndef OBJECT_H
#define OBJECT_H

#include "qdom.h"
#include "qpoint.h"
#include "qstring.h"
#include "qpixmap.h"

#include "ProjectException.h"

namespace projectcomponents {

	/**
	*	The Object class represents a small object in an environment with which interaction is possible.
	*/
	class Object : public QObject {

		Q_OBJECT

	public:
		virtual ~Object();

		QString getName() const;
		QPixmap getImage() const;
		QString getImageName() const;
		QPoint getImageFrame() const;

		void setName(QString name) throw (ProjectException);
		void setImage(QString name, QPoint frame) throw (ProjectException);

		void unsetImage();

		static Object* create(QString name);
		static Object* translateFromXML(QDomElement &element);
		static void translateToXML(Object *object, QDomDocument &document, QDomElement &parent);

	private:
		Object(QString name);

		QString _name;
		QPixmap _image;
		QString _imageName;
		QPoint _imageFrame;

	signals:
		void deleteSignal(QString name);
	};
}

#endif