#include "qfileinfo.h"

#include "../Logger.h"
#include "../ProgramDefaults.h"
#include "../Headers/Object.h"

using namespace projectcomponents;

/**
*	Create an object with name <i>name</i>.
*/
Object::Object(QString name)
	: _name(name), _image(), _imageName(), _imageFrame(0, 0) {

		Logger *logger = Logger::instance();
		logger->addMessage("Created environment object " + _name);
		logger->setStatus(true);
}

/**
*	Standard destructor.
*/
Object::~Object() {
	emit deleteSignal(_name);
}

/**
*	Return the name of the object.
*/
QString Object::getName() const {
	return _name;
}

/**
*	Return the entire image associated with the object.
*/
QPixmap Object::getImage() const {
	return _image;
}

/**
*	Return the imagename of the image of the object.
*/
QString Object::getImageName() const {
	return _imageName;
}

/**
*	Return the image that represents the object in de gameworld.
*/
QPoint Object::getImageFrame() const {
	return _imageFrame;
}

/**
*	Set the name of the object.
*/
void Object::setName(QString name) {
	if (name == QString())
		throw ProjectException("An environment object should not have an empty name.");

	_name = name;
	Logger *logger = Logger::instance();
	logger->addMessage("Changed name of environment object to " + _name);
	logger->setStatus(true);
}

/**
*	Set the entire object image and select which frame represents the object in the image.
*/
void Object::setImage(QString name, QPoint frame) {
	if (name == QString())
		throw ProjectException("To remove the image, use the appropriate method.");

	QFileInfo imageinfo(g_charactersPath + name);
	if (!imageinfo.exists())
		throw ProjectException("The image " + name + " does not exist in the path " + g_staticTilesetPath + ".");

	if ((frame.x() < 0) || (frame.x() > 3) || (frame.y() < 0) || (frame.y() > 3))
		throw ProjectException("The frame of the object does not point to a valid location");

	QPixmap image(g_charactersPath + name);
	QSize dimensions(image.width()/4, image.height()/4);

	_image = image.copy(dimensions.width() * frame.x(), dimensions.height() * frame.y(), dimensions.width(), dimensions.height());
	_imageName = name;
	_imageFrame = frame;

	Logger *logger = Logger::instance();
	logger->addMessage("Changed image of environment object " + _name);
	logger->setStatus(true);
}

/**
*	Removes image from object.
*/
void Object::unsetImage() {
	_image = QPixmap();
	_imageName = QString();
	_imageFrame = QPoint(0, 0);

	Logger *logger = Logger::instance();
	logger->addMessage("Removed image from environment object " + _name);
	logger->setStatus(true);
}

/**
*	Creates an object with name <i>name</i> and returns a pointer to it.
*/
Object* Object::create(QString name) {
	if (name == QString())
		throw ProjectException("An environment object should not have an empty name.");

	return new Object(name);
}

/**
*	Create and translate an object from XML and return a pointer to it.
*/
Object* Object::translateFromXML(QDomElement &element) {
	QString name = element.attributeNode("name").value();
	Object *object = create(name);

	QDomElement imageElement = element.firstChildElement("image");
	if (!imageElement.isNull()) {
		QString imagename = imageElement.attributeNode("name").value();
		int row = imageElement.attributeNode("row").value().toInt();
		int column = imageElement.attributeNode("column").value().toInt();

		object->setImage(imagename, QPoint(row, column));
	}

	return object;
}

/**
*	Translate the object to XML and append it to the XML-document.
*/
void Object::translateToXML(Object *object, QDomDocument &document, QDomElement &parent) {
	QDomElement objectElement = document.createElement("object");

	objectElement.setAttribute("name", object->getName());

	if (object->getImageName() != QString()) {
		QDomElement imageElement = document.createElement("image");
		imageElement.setAttribute("name", object->getImageName());
		imageElement.setAttribute("row", object->getImageFrame().y());
		imageElement.setAttribute("column", object->getImageFrame().x());

		objectElement.appendChild(imageElement);
	}

	parent.appendChild(objectElement);
}