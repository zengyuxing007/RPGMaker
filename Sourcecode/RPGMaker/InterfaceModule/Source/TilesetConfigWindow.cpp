#include "qdir.h"
#include "qframe.h"
#include "qfileinfo.h"
#include "qstringlist.h"
#include "qscrollarea.h"
#include "qfiledialog.h"
#include "qmessagebox.h"

#include "../Headers/TilesetConfigWindow.h"

using namespace interfacemodule;

TilesetConfigWindow::TilesetConfigWindow(QWidget *parent, setupMode mode, Project *project)
	: QDialog(parent), _mode(mode), _project(project) {

		setWindowTitle("Tileset configuration");

		_layout = new QHBoxLayout(this);
		
		createLeftPane(mode);
		createRightPane();
}

TilesetConfigWindow::~TilesetConfigWindow() {}

QString TilesetConfigWindow::getName() const {
	return _nameframe->text();
}

QString TilesetConfigWindow::getImagename() const {
	QComboBox *imagenameframe = dynamic_cast<QComboBox*>(_imagenameframe);
	return imagenameframe->currentText();
}

Tileset::Type TilesetConfigWindow::getType() const {
	QComboBox *typeframe = dynamic_cast<QComboBox*>(_typeframe);
	if (typeframe->currentText() == QString("Static"))
		return Tileset::STATIC;
	else if (typeframe->currentText() == QString("Auto"))
		return Tileset::AUTO;
}

void TilesetConfigWindow::setupNewTileset() {
	QStringList types;
	types << "Static" << "Auto";

	QComboBox *typeframe = dynamic_cast<QComboBox*>(_typeframe);
	typeframe->addItems(types);

	typeframe->setCurrentIndex(0);
}

void TilesetConfigWindow::setupEditTileset(QString tilesetname, Tileset::Type type, QString imagename, QPixmap image) {
	_nameframe->setText(tilesetname);

	QLabel *typeframe = dynamic_cast<QLabel*>(_typeframe);
	switch (type) {
	case Tileset::STATIC:
		typeframe->setText("Static tileset");
		break;
	case Tileset::AUTO:
		typeframe->setText("Auto tileset");
		break;
	}

	QLabel *imagenameframe = dynamic_cast<QLabel*>(_imagenameframe);
	imagenameframe->setText(imagename);

	_imageframe->setPixmap(image);
}

void TilesetConfigWindow::createLeftPane(setupMode mode) {
	QFrame *leftframe = new QFrame(this);
	QVBoxLayout *leftlayout = new QVBoxLayout(leftframe);
	_layout->addWidget(leftframe);

	QFrame *nameframe = new QFrame(leftframe);
	QHBoxLayout *namelayout = new QHBoxLayout(nameframe);
	namelayout->setAlignment(Qt::AlignLeft);
	leftlayout->addWidget(nameframe);

	QFrame *typeframe = new QFrame(leftframe);
	QHBoxLayout *typelayout = new QHBoxLayout(typeframe);
	typelayout->setAlignment(Qt::AlignLeft);
	leftlayout->addWidget(typeframe);

	QFrame *imageframe = new QFrame(leftframe);
	QHBoxLayout  *imagelayout = new QHBoxLayout(imageframe);
	imagelayout->setAlignment(Qt::AlignLeft);
	leftlayout->addWidget(imageframe);

	QFrame *buttonframe = new QFrame(leftframe);
	QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
	buttonlayout->setAlignment(Qt::AlignLeft);
	leftlayout->addWidget(buttonframe);

	QLabel *label1 = new QLabel("Enter a name for this tileset:", nameframe);
	_nameframe = new QLineEdit(nameframe);
	namelayout->addWidget(label1);
	namelayout->addWidget(_nameframe);

	QLabel *label2 = new QLabel("Select a type for this tileset:", typeframe);
	switch (mode) {
	case NEW:
		_typeframe = new QComboBox(typeframe);
		connect(_typeframe, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(currentTypeChanged(const QString&)));
		break;
	case EDIT:
		_typeframe = new QLabel(typeframe);
		break;
	}
	typelayout->addWidget(label2);
	typelayout->addWidget(_typeframe);

	QLabel *label3 = new QLabel("Select an image for this tileset:", imageframe);
	switch (mode) {
	case NEW:
		_imagenameframe = new QComboBox(imageframe);
		connect(_imagenameframe, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(currentImagenameChanged(const QString&)));
		break;
	case EDIT:
		_imagenameframe = new QLabel(imageframe);
		break;
	}
	imagelayout->addWidget(label3);
	imagelayout->addWidget(_imagenameframe);

	QPushButton *acceptbutton = new QPushButton("Accept", buttonframe);
	QPushButton *cancelbutton = new QPushButton("Cancel", buttonframe);
	buttonlayout->addWidget(acceptbutton);
	buttonlayout->addWidget(cancelbutton);

	connect(acceptbutton, SIGNAL(clicked()), this, SLOT(checkInput()));
	connect(cancelbutton, SIGNAL(clicked()), this, SLOT(reject()));
}

void TilesetConfigWindow::createRightPane() {
	QScrollArea *imageframe = new QScrollArea(this);
	imageframe->setWidgetResizable(true);
	_imageframe = new QLabel("No image selected", imageframe);
	imageframe->setWidget(_imageframe);

	_layout->addWidget(imageframe);
}

void TilesetConfigWindow::checkInput() {
	if (_nameframe->text() == QString()) {
		QMessageBox::critical(this, tr("No empty name allowed"), tr("A tileset should not have an empty name.\nEnter a name for your tileset before proceeding."));
		return;
	}

	if (!!_project->getProperties()->getTilesets()->getTileset(_nameframe->text())) {
		QMessageBox::critical(this, tr("Name already exists"), tr("A tileset with the same name already exists.\nEnter a new name for your tileset before proceeding."));
		return;
	}

	if (_mode == NEW) {
		QComboBox *imagenameframe = dynamic_cast<QComboBox*>(_imagenameframe);
		if (imagenameframe->currentText() == QString()) {
			QMessageBox::critical(this, tr("No image selected"), tr("No image has been selected.\nSelect an image from the dropdown list."));
			return;
		}
	}

	accept();
}

void TilesetConfigWindow::currentTypeChanged(const QString &type) {
	if (_mode == NEW) {
		QDir tilesetdirectory;
		if (type == "Static")
			tilesetdirectory.setPath(g_staticTilesetPath);
		else if (type == "Auto")
			tilesetdirectory.setPath(g_autoTilesetPath);

		if (!tilesetdirectory.exists())
			throw InterfaceException("The directory " + tilesetdirectory.path() + " does not exist.");

		QStringList filters;
		filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp";
		tilesetdirectory.setNameFilters(filters);

		QComboBox *imagenameframe = dynamic_cast<QComboBox*>(_imagenameframe);
		imagenameframe->clear();
		imagenameframe->addItems(tilesetdirectory.entryList());
		if (imagenameframe->count() > 0)
			imagenameframe->setCurrentIndex(0);
	}
}

void TilesetConfigWindow::currentImagenameChanged(const QString &name) {
	if (_mode == NEW) {
		_imageframe->clear();
		QComboBox *typeframe = dynamic_cast<QComboBox*>(_typeframe);
		if (typeframe->currentText() == "Static")
			_imageframe->setPixmap(QPixmap(g_staticTilesetPath + name));
		else if (typeframe->currentText() == "Auto")
			_imageframe->setPixmap(QPixmap(g_autoTilesetPath + name));
	}
}