#include "qdir.h"
#include "qlabel.h"
#include "qvector.h"
#include "qgroupbox.h"
#include "qboxlayout.h"
#include "qgridlayout.h"
#include "qpushbutton.h"
#include "qmessagebox.h"
#include "qheaderview.h"
#include "qradiobutton.h"
#include "qinputdialog.h"

#include "../ProgramDefaults.h"
#include "../Headers/ObjectWindow.h"

using namespace interfacemodule;

ObjectWindow::ObjectWindow(QWidget *parent, Project *project)
	: QFrame(parent), _project(project) {

		createWindow();
		loadObjects();

		if (_objectsList->count() > 0)
			_objectsList->setCurrentRow(0);
}

ObjectWindow::~ObjectWindow() {}

void ObjectWindow::createWindow() {
	QHBoxLayout *layout = new QHBoxLayout(this);

	{	// Object selection
		QGroupBox *objectFrame = new QGroupBox("Objects", this);
		QVBoxLayout *objectsLayout = new QVBoxLayout(objectFrame);

		QLabel *label = new QLabel("Select an object:", objectFrame);

		QFrame *buttonFrame = new QFrame(this);
		QHBoxLayout *buttonlayout = new QHBoxLayout(buttonFrame);
		buttonlayout->setAlignment(Qt::AlignCenter);
		QPushButton *newObjectButton = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", buttonFrame);
		newObjectButton->setToolTip("Create a new object");
		newObjectButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *editObjectButton = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", buttonFrame);
		editObjectButton->setToolTip("Edit the current object");
		editObjectButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *deleteObjectButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", buttonFrame);
		deleteObjectButton->setToolTip("Delete the current object");
		deleteObjectButton->setMaximumSize(g_buttonsize, g_buttonsize);
		buttonlayout->addWidget(newObjectButton);
		buttonlayout->addWidget(editObjectButton);
		buttonlayout->addWidget(deleteObjectButton);
		buttonlayout->addSpacing(20);

		_objectsList = new QListWidget(this);

		objectsLayout->addWidget(label);
		objectsLayout->addWidget(_objectsList);
		objectsLayout->addWidget(buttonFrame);

		layout->addWidget(objectFrame);

		connect(newObjectButton, SIGNAL(clicked()), this, SLOT(newObject()));
		connect(editObjectButton, SIGNAL(clicked()), this, SLOT(editObject()));
		connect(deleteObjectButton, SIGNAL(clicked()), this, SLOT(deleteObject()));
		connect(_objectsList, SIGNAL(currentTextChanged(const QString &)), this, SLOT(objectsSelected(const QString &)));
	}

	{	// Image selection
		QGroupBox *imageFrame = new QGroupBox("Image", this);
		QVBoxLayout *imageLayout = new QVBoxLayout(imageFrame);

		QFrame *imageSelectionFrame = new QFrame(this);
		QHBoxLayout *imageSelectionLayout = new QHBoxLayout(imageSelectionFrame);
		imageSelectionLayout->setAlignment(Qt::AlignLeft);

		QLabel *label1 = new QLabel("Select an image:", imageSelectionFrame);
		_imagesList = new QComboBox(imageSelectionFrame);
		_imagesList->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		QLabel *label2 = new QLabel("Currently selected frame:", imageSelectionFrame);
		_currentImage = new QLabel(imageSelectionFrame);

		imageSelectionLayout->addWidget(label1);
		imageSelectionLayout->addWidget(_imagesList);
		imageSelectionLayout->addSpacing(20);
		imageSelectionLayout->addWidget(label2);
		imageSelectionLayout->addWidget(_currentImage);

		_imageTable = new QTableWidget(this);
		_imageTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
		_imageTable->setSelectionBehavior(QAbstractItemView::SelectItems);
		_imageTable->setSelectionMode(QAbstractItemView::SingleSelection);
		_imageTable->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
		_imageTable->verticalHeader()->setResizeMode(QHeaderView::Fixed);
		_imageTable->horizontalHeader()->hide();
		_imageTable->verticalHeader()->hide();

		imageLayout->addWidget(imageSelectionFrame);
		imageLayout->addWidget(_imageTable);

		layout->addWidget(imageFrame);

		connect(_imagesList, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(imageSelected(const QString &)));
		connect(_imageTable, SIGNAL(cellClicked(int, int)), this, SLOT(frameClicked(int, int)));
	}
}

void ObjectWindow::loadObjects() {
	_currentImage->clear();
	_imagesList->clear();
	_objectsList->clear();

	_imageTable->clear();
	_imageTable->setRowCount(0);
	_imageTable->setColumnCount(0);

	QVector<Object*> objects = _project->getObjects()->getObjects();
	for (int i = 0; i < objects.size(); i++) {
		Object *it = objects.at(i);
		if (!!it)
			_objectsList->addItem(it->getName());
	}
}

void ObjectWindow::objectsSelected(const QString &name) {
	if ((_objectsList->count() > 0) && (name != QString())) {
		_imagesList->clear();
		_imageTable->clear();
		_imageTable->setRowCount(0);
		_imageTable->setColumnCount(0);
		_currentImage->clear();


		Object *object = _project->getObjects()->getObject(name);
		if (!!object) {

			QStringList filters;
			filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp";

			QDir objectDir(g_charactersPath);
			objectDir.setNameFilters(filters);
			_imagesList->addItems(objectDir.entryList());

			if (object->getImageName() != QString()) {
				for (int i = 0; i < _imagesList->count(); i++) {
					if (_imagesList->itemText(i) == object->getImageName()) {
						_imagesList->setCurrentIndex(i);
						break;
					}
				}

				_currentImage->setPixmap(object->getImage());
			} else {
				if (_imagesList->count() > 0)
					_imagesList->setCurrentIndex(0);
			}
		}
	}
}

void ObjectWindow::newObject() {
	bool accepted;
	QString name = QInputDialog::getText(this, tr("New object"),
		tr("Enter a name for the object.\nThis name should be unique across all objects."),
		QLineEdit::Normal, "", &accepted);

	if (accepted) {
		if (name == QString()) {
			QMessageBox::critical(this, tr("No empty name allowed"), tr("An object should not have an empty name."));
			newObject();
			return;
		}

		if (!!_project->getObjects()->getObject(name)) {
			QMessageBox::critical(this, tr("Name already in use"), tr("An object with the same name already exists."));
			newObject();
			return;
		}

		Object *object = Object::create(name);
		try {
			_project->getObjects()->addObject(object);
		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error has occurred"), e.what());
			delete object;
			return;
		}

		loadObjects();
		_objectsList->setCurrentRow(_objectsList->count() -1);
	}
}

void ObjectWindow::editObject() {
	if ((_objectsList->count() > 0) && (!!_objectsList->currentItem())) {
		Object *object = _project->getObjects()->getObject(_objectsList->currentItem()->text());
		if (!!object) {
			bool accepted;
			QString name = QInputDialog::getText(this, tr("Edit object"),
				tr("Enter a new name for the object:"),
				QLineEdit::Normal, object->getName(), &accepted);

			if (accepted && (name != object->getName())) {
				if (name == QString()) {
					QMessageBox::critical(this, tr("No empty name allowed"), tr("An object should not have an empty name."));
					editObject();
					return;
				}

				if (!!_project->getObjects()->getObject(name)) {
					QMessageBox::critical(this, tr("Name already in use"), tr("An object with the same name already exists."));
					editObject();
					return;
				}

				object->setName(name);
				_objectsList->currentItem()->setText(name);
			}
		}
	}
}

void ObjectWindow::deleteObject() {
	if ((_objectsList->count() > 0) && !!_objectsList->currentItem()) {

		QString currentObject = _objectsList->currentItem()->text();
		Object *object = _project->getObjects()->getObject(currentObject);

		if (!!object) {

			QMessageBox::StandardButton response = QMessageBox::warning(this,"Delete object",
				"Are you sure you want to delete the object?\nDeleting an object will delete all references to it, and cannot be undone.",
				QMessageBox::Ok | QMessageBox::Cancel);

			if (response == QMessageBox::Ok) {

				_project->getObjects()->removeObject(currentObject);

				loadObjects();
				if (_objectsList->count() > 0)
					_objectsList->setCurrentRow(0);

				emit updateSignal();
			}
		}
	}
}

void ObjectWindow::imageSelected(const QString &name) {
	if ((_imagesList->count() > 0) && (name != QString())) {
		QPixmap image(g_charactersPath + name);
		QSize dimensions = image.size();

		_imageTable->clear();
		_imageTable->setRowCount(4);
		_imageTable->setColumnCount(4);
		_imageTable->horizontalHeader()->setDefaultSectionSize(dimensions.width()/4);
		_imageTable->verticalHeader()->setDefaultSectionSize(dimensions.height()/4);

		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				QPixmap frame = image.copy(x * dimensions.width()/4, y * dimensions.height()/4, dimensions.width()/4, dimensions.height()/4);
				QLabel *label = new QLabel();
				label->setPixmap(frame);
				_imageTable->setCellWidget(y, x, label);
			}
		}
	}
}

void ObjectWindow::frameClicked(int row, int column) {
	if ((_objectsList->count() > 0) && !!_objectsList->currentItem() && (_imagesList->count() > 0)) {

		Object *object = _project->getObjects()->getObject(_objectsList->currentItem()->text());

		if (!!object) {
			object->setImage(_imagesList->currentText(), QPoint(column, row));
			_currentImage->setPixmap(object->getImage());
		}
	}
}