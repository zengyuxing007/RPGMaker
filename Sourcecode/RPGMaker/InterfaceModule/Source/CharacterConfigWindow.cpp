#include "qdir.h"
#include "qsize.h"
#include "qframe.h"
#include "qvector.h"
#include "qstring.h"
#include "qstringlist.h"
#include "qpushbutton.h"
#include "qmessagebox.h"

#include "../ProgramDefaults.h"
#include "../Headers/CharacterConfigWindow.h"

using namespace interfacemodule;

CharacterConfigWindow::CharacterConfigWindow(QWidget *parent, Project *project)
	: _project(project) {

		_layout = new QHBoxLayout(this);
		setWindowTitle("Character configuration");

		createLeftPane();
		createRightPane();
		loadWindow();
}

CharacterConfigWindow::~CharacterConfigWindow() {}

QString CharacterConfigWindow::getName() const {
	return _nameField->text();
}

int CharacterConfigWindow::getLevel() const {
	return _levelSelection->value();
}

QString CharacterConfigWindow::getClass() const {
	if (_classSelection->count() > 0)
		return _classSelection->currentText();

	return QString();
}

QString CharacterConfigWindow::getEnvironmentImage() const {
	return _environmnentSelection->currentText();
}

QString CharacterConfigWindow::getBattleImage() const {
	return _battleSelection->currentText();
}

void CharacterConfigWindow::setName(QString name) {
	_oldName = name;
	_nameField->setText(name);
}

void CharacterConfigWindow::setLevel(int level) {
	_levelSelection->setValue(level);
}

void CharacterConfigWindow::setClass(QString name) {
	for (int i = 0; i < _classSelection->count(); i++) {
		if (_classSelection->itemText(i) == name) {
			_classSelection->setCurrentIndex(i);
			return;
		}
	}
}

void CharacterConfigWindow::setEnvironmentImage(QString image) {
	for (int i = 0; i < _environmnentSelection->count(); i++) {
		if (image == _environmnentSelection->itemText(i)) {
			_environmnentSelection->setCurrentIndex(i);
			return;
		}
	}
}

void CharacterConfigWindow::setBattleImage(QString image) {
	for (int i = 0; i < _battleSelection->count(); i++) {
		if (image == _battleSelection->itemText(i)) {
			_battleSelection->setCurrentIndex(i);
			return;
		}
	}
}

void CharacterConfigWindow::loadWindow() {
	_classSelection->clear();
	_classSelection->addItem(QString());
	QVector<Class*> classes = _project->getProperties()->getClasses()->getClasses();
	for (int i = 0; i < classes.size(); i++) {
		_classSelection->addItem(classes.at(i)->getName());
	}

	if (_classSelection->count() > 0)
		_classSelection->setCurrentIndex(0);

	QStringList filters;
	filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp";

	QDir environmentDir(g_charactersPath);
	environmentDir.setNameFilters(filters);
	_environmnentSelection->clear();
	_environmnentSelection->addItems(environmentDir.entryList());

	if (_environmnentSelection->count() > 0)
		_environmnentSelection->setCurrentIndex(0);

	QDir battleDir(g_battlersPath);
	battleDir.setNameFilters(filters);
	_battleSelection->clear();
	_battleSelection->addItems(battleDir.entryList());

	if (_battleSelection->count() > 0)
		_battleSelection->setCurrentIndex(0);
}

void CharacterConfigWindow::createLeftPane() {
	QFrame *frame = new QFrame(this);
	QVBoxLayout *layout = new QVBoxLayout(frame);

	QLabel *label1 = new QLabel("Enter a name for the character:", frame);
	_nameField = new QLineEdit(frame);
	layout->addWidget(label1);
	layout->addWidget(_nameField);

	QLabel *label2 = new QLabel("Set a level for the character:", frame);
	_levelSelection = new QSpinBox(frame);
	_levelSelection->setMaximum(99);
	_levelSelection->setMinimum(0);
	_levelSelection->setSingleStep(1);
	layout->addWidget(label2);
	layout->addWidget(_levelSelection);

	QLabel *label3 = new QLabel("Select a class for the character:", frame);
	_classSelection = new QComboBox(frame);
	_classSelection->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	_classSelection->setInsertPolicy(QComboBox::InsertAlphabetically);
	layout->addWidget(label3);
	layout->addWidget(_classSelection);

	QLabel *label4 = new QLabel("Select an environmental image:", frame);
	_environmnentSelection = new QComboBox(frame);
	_environmnentSelection->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	layout->addWidget(label4);
	layout->addWidget(_environmnentSelection);

	QLabel *label5 = new QLabel("Select a battle image:", frame);
	_battleSelection = new QComboBox(frame);
	_battleSelection->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	layout->addWidget(label5);
	layout->addWidget(_battleSelection);

	QFrame *buttonFrame = new QFrame(frame);
	QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
	QPushButton *acceptButton = new QPushButton("Accept", buttonFrame);
	QPushButton *cancelButton = new QPushButton("Cancel", buttonFrame);
	buttonLayout->addWidget(acceptButton);
	buttonLayout->addWidget(cancelButton);
	layout->addWidget(buttonFrame);

	QLabel *infoLabel = new QLabel("*Images are scaled.\nOriginal sizes will be used in the game.", this);
	layout->addWidget(infoLabel);

	_layout->addWidget(frame);

	connect(acceptButton, SIGNAL(clicked()), this, SLOT(checkInput()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(_environmnentSelection, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(environmentChanged(const QString &)));
	connect(_battleSelection, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(battleChanged(const QString &)));
}

void CharacterConfigWindow::createRightPane() {
	QFrame *leftFrame = new QFrame(this);
	QVBoxLayout *leftLayout = new QVBoxLayout(leftFrame);
	leftLayout->setAlignment(Qt::AlignTop);
	_layout->addWidget(leftFrame);
	
	QFrame *rightFrame = new QFrame(this);
	QVBoxLayout *rightLayout = new QVBoxLayout(rightFrame);
	rightLayout->setAlignment(Qt::AlignTop);
	_layout->addWidget(rightFrame);

	QLabel *leftLabel = new QLabel("Environmental image*:", leftFrame);
	_environmentLabel = new QLabel(leftFrame);
	leftLayout->addWidget(leftLabel);
	leftLayout->addWidget(_environmentLabel);

	QLabel *rightLabel = new QLabel("Battle image*:", rightFrame);
	_battleLabel = new QLabel(rightFrame);
	rightLayout->addWidget(rightLabel);
	rightLayout->addWidget(_battleLabel);
}

void CharacterConfigWindow::checkInput() {
	QRegExpValidator validator(g_nameRegex);
	int pos = 0;
	if (validator.validate(_nameField->text(), pos) != QValidator::Acceptable) {
		QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
		return;
	}

	if (_nameField->text() == QString()) {
		QMessageBox::critical(this, tr("No empty name allowed"), tr("A character should not have an empty name.\nEnter a name for your character before proceeding."));
		return;
	}

	if ((_oldName != _nameField->text()) && !!_project->getCharacters()->getCharacter(_nameField->text())) {
		QMessageBox::critical(this, tr("Name already exists"), tr("A character with the same name already exists.\nEnter a new name for your character before proceeding."));
		return;
	}

	accept();
}

void CharacterConfigWindow::environmentChanged(const QString &name) {
	_environmentLabel->clear();

	QPixmap image(g_charactersPath + name);

	if ((image.size().width() > 200) || (image.size().height() > 200))
		image = image.scaled(QSize(200, 200), Qt::KeepAspectRatio);

	_environmentLabel->setPixmap(image);
}

void CharacterConfigWindow::battleChanged(const QString &name) {
	_battleLabel->clear();

	QPixmap image(g_battlersPath + name);

	if ((image.size().width() > 200) || (image.size().height() > 200))
		image = image.scaled(QSize(200, 200), Qt::KeepAspectRatio);

	_battleLabel->setPixmap(image);
}