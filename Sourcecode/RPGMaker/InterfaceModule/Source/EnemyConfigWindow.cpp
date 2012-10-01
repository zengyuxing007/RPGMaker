#include "qdir.h"
#include "qsize.h"
#include "qframe.h"
#include "qvector.h"
#include "qstring.h"
#include "qstringlist.h"
#include "qpushbutton.h"
#include "qmessagebox.h"

#include "../ProgramDefaults.h"
#include "../Headers/EnemyConfigWindow.h"

using namespace interfacemodule;

EnemyConfigWindow::EnemyConfigWindow(QWidget *parent, Project *project)
	: _project(project) {

		_layout = new QHBoxLayout(this);
		setWindowTitle("Enemy configuration");

		createLeftPane();
		createRightPane();
		loadWindow();
}

EnemyConfigWindow::~EnemyConfigWindow() {}

QString EnemyConfigWindow::getName() const {
	return _nameField->text();
}

int EnemyConfigWindow::getLevel() const {
	return _levelSelection->value();
}

QString EnemyConfigWindow::getClass() const {
	if (_classSelection->count() > 0)
		return _classSelection->currentText();

	return QString();
}

int EnemyConfigWindow::getExperience() const {
	return _experienceSelection->value();
}

QPair<int, int> EnemyConfigWindow::getCurrency() const {
	return QPair<int, int>(_minCurrencySelection->value(), _maxCurrencySelection->value());
}

QString EnemyConfigWindow::getEnvironmentImage() const {
	return _environmnentSelection->currentText();
}

QString EnemyConfigWindow::getBattleImage() const {
	return _battleSelection->currentText();
}

void EnemyConfigWindow::setName(QString name) {
	_oldName = name;
	_nameField->setText(name);
}

void EnemyConfigWindow::setLevel(int level) {
	_levelSelection->setValue(level);
}

void EnemyConfigWindow::setClass(QString name) {
	for (int i = 0; i < _classSelection->count(); i++) {
		if (_classSelection->itemText(i) == name) {
			_classSelection->setCurrentIndex(i);
			return;
		}
	}
}

void EnemyConfigWindow::setExperience(int experience) {
	_experienceSelection->setValue(experience);
}

void EnemyConfigWindow::setCurrency(QPair<int, int> currency) {
	_minCurrencySelection->setValue(currency.first);
	_maxCurrencySelection->setValue(currency.second);
}

void EnemyConfigWindow::setEnvironmentImage(QString image) {
	for (int i = 0; i < _environmnentSelection->count(); i++) {
		if (image == _environmnentSelection->itemText(i)) {
			_environmnentSelection->setCurrentIndex(i);
			return;
		}
	}
}

void EnemyConfigWindow::setBattleImage(QString image) {
	for (int i = 0; i < _battleSelection->count(); i++) {
		if (image == _battleSelection->itemText(i)) {
			_battleSelection->setCurrentIndex(i);
			return;
		}
	}
}

void EnemyConfigWindow::loadWindow() {
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

void EnemyConfigWindow::createLeftPane() {
	QFrame *frame = new QFrame(this);
	QVBoxLayout *layout = new QVBoxLayout(frame);

	QLabel *label1 = new QLabel("Enter a name for the enemy:", frame);
	_nameField = new QLineEdit(frame);
	layout->addWidget(label1);
	layout->addWidget(_nameField);

	QLabel *label2 = new QLabel("Set a level for the enemy:", frame);
	_levelSelection = new QSpinBox(frame);
	_levelSelection->setMaximum(99);
	_levelSelection->setMinimum(0);
	_levelSelection->setSingleStep(1);
	layout->addWidget(label2);
	layout->addWidget(_levelSelection);

	QLabel *label3 = new QLabel("Select a class for the enemy:", frame);
	_classSelection = new QComboBox(frame);
	_classSelection->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	_classSelection->setInsertPolicy(QComboBox::InsertAlphabetically);
	layout->addWidget(label3);
	layout->addWidget(_classSelection);

	QLabel *label4 = new QLabel("Set the amount of experience this enemy is worth:", frame);
	_experienceSelection = new QSpinBox(frame);
	_experienceSelection->setMinimum(0);
	_experienceSelection->setMaximum(9999);
	_experienceSelection->setSingleStep(1);
	layout->addWidget(label4);
	layout->addWidget(_experienceSelection);

	QLabel *label5 = new QLabel("Set the boundaries of currency this enemy drops:", frame);
	QFrame *currencyFrame = new QFrame(frame);
	QHBoxLayout *currencyLayout = new QHBoxLayout(currencyFrame);
	QLabel *minLabel = new QLabel("min.", currencyFrame);
	_minCurrencySelection = new QSpinBox(currencyFrame);
	_minCurrencySelection->setMinimum(0);
	_minCurrencySelection->setMaximum(9999);
	QLabel *maxLabel = new QLabel("max.", currencyFrame);
	_maxCurrencySelection = new QSpinBox(currencyFrame);
	_maxCurrencySelection->setMinimum(0);
	_maxCurrencySelection->setMaximum(9999);
	currencyLayout->addWidget(minLabel);
	currencyLayout->addWidget(_minCurrencySelection);
	currencyLayout->addWidget(maxLabel);
	currencyLayout->addWidget(_maxCurrencySelection);
	layout->addWidget(label5);
	layout->addWidget(currencyFrame);

	QLabel *label6 = new QLabel("Select an environmental image:", frame);
	_environmnentSelection = new QComboBox(frame);
	_environmnentSelection->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	layout->addWidget(label6);
	layout->addWidget(_environmnentSelection);

	QLabel *label7 = new QLabel("Select a battle image:", frame);
	_battleSelection = new QComboBox(frame);
	_battleSelection->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	layout->addWidget(label7);
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

void EnemyConfigWindow::createRightPane() {
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

void EnemyConfigWindow::checkInput() {
	QRegExpValidator validator(g_nameRegex);
	int pos = 0;
	if (validator.validate(_nameField->text(), pos) != QValidator::Acceptable) {
		QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
		return;
	}

	if (_nameField->text() == QString()) {
		QMessageBox::critical(this, tr("No empty name allowed"), tr("An enemy should not have an empty name.\nEnter a name for your tileset before proceeding."));
		return;
	}

	if ((_oldName != _nameField->text()) && !!_project->getEnemies()->getEnemy(_nameField->text())) {
		QMessageBox::critical(this, tr("Name already exists"), tr("An enemy with the same name already exists.\nEnter a new name for your tileset before proceeding."));
		return;
	}

	if (_minCurrencySelection->value() > _maxCurrencySelection->value()) {
		int temp = _minCurrencySelection->value();
		_minCurrencySelection->setValue(_maxCurrencySelection->value());
		_maxCurrencySelection->setValue(temp);
	}

	accept();
}

void EnemyConfigWindow::environmentChanged(const QString &name) {
	_environmentLabel->clear();

	QPixmap image(g_charactersPath + name);

	if ((image.size().width() > 200) || (image.size().height() > 200))
		image = image.scaled(QSize(200, 200), Qt::KeepAspectRatio);

	_environmentLabel->setPixmap(image);
}

void EnemyConfigWindow::battleChanged(const QString &name) {
	_battleLabel->clear();

	QPixmap image(g_battlersPath + name);

	if ((image.size().width() > 200) || (image.size().height() > 200))
		image = image.scaled(QSize(200, 200), Qt::KeepAspectRatio);

	_battleLabel->setPixmap(image);
}