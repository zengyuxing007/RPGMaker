#include "qdir.h"
#include "qframe.h"
#include "qgroupbox.h"
#include "qboxlayout.h"
#include "qpushbutton.h"
#include "qmessagebox.h"
#include "phonon/mediasource.h"

#include "../ProgramDefaults.h"
#include "../Headers/EnvironmentConfigWindow.h"

using namespace interfacemodule;

EnvironmentConfigWindow::EnvironmentConfigWindow(QWidget *parent, Project *project)
	: QDialog(parent), _project(project), _oldname(QString()) {

		setWindowTitle("Environment configuration");
		createWindow();
		loadWindow();
}

EnvironmentConfigWindow::~EnvironmentConfigWindow() {}

void EnvironmentConfigWindow::createWindow() {

	QHBoxLayout *layout = new QHBoxLayout(this);

	QFrame *leftFrame = new QFrame(this);
	QVBoxLayout *leftLayout = new QVBoxLayout(leftFrame);
	layout->addWidget(leftFrame);

	{
		QGroupBox *nameFrame = new QGroupBox("Name", leftFrame);
		QLabel *label1 = new QLabel("Enter a name for your environment:", nameFrame);
		_nameInput = new QLineEdit(nameFrame);
		QVBoxLayout *nameLayout = new QVBoxLayout(nameFrame);
		nameLayout->setAlignment(Qt::AlignLeft);
		nameLayout->addWidget(label1);
		nameLayout->addWidget(_nameInput);

		leftLayout->addWidget(nameFrame);
	}
	
	{
		QGroupBox *dimensionFrame = new QGroupBox("Dimensions", leftFrame);

		QFrame *widthFrame = new QFrame(dimensionFrame);
		QLabel *label2 = new QLabel("Width:\n(Max 50)", widthFrame);
		_widthInput = new QSpinBox(widthFrame);
		_widthInput->setMinimum(1);
		_widthInput->setMaximum(50);
		_widthInput->setValue(25);
		QHBoxLayout *widthLayout = new QHBoxLayout(widthFrame);
		widthLayout->setAlignment(Qt::AlignLeft);
		widthLayout->addWidget(label2);
		widthLayout->addWidget(_widthInput);

		QFrame *heightFrame = new QFrame(dimensionFrame);
		QLabel *label3 = new QLabel("Height:\n(Max 50)", heightFrame);
		_heightInput = new QSpinBox(heightFrame);
		_heightInput->setMinimum(1);
		_heightInput->setMaximum(50);
		_heightInput->setValue(25);
		QHBoxLayout *heightLayout = new QHBoxLayout(heightFrame);
		heightLayout->setAlignment(Qt::AlignLeft);
		heightLayout->addWidget(label3);
		heightLayout->addWidget(_heightInput);

		QHBoxLayout *dimensionLayout = new QHBoxLayout(dimensionFrame);
		dimensionLayout->addWidget(widthFrame);
		dimensionLayout->addWidget(heightFrame);

		leftLayout->addWidget(dimensionFrame);
	}

	{
		QGroupBox *musicFrame = new QGroupBox("Background music", leftFrame);
		QHBoxLayout *musicLayout = new QHBoxLayout(musicFrame);

		_musicSelection = new QComboBox(musicFrame);
		_musicSelection->setSizeAdjustPolicy(QComboBox::AdjustToContents);

		_audioOutputObject = new Phonon::AudioOutput(Phonon::MusicCategory, musicFrame);
		_backgroundMusicObject = new Phonon::MediaObject(this);
		Phonon::createPath(_backgroundMusicObject, _audioOutputObject);

		QPushButton *playButton = new QPushButton(QIcon(g_iconPath + "playback_play_icon&24.png"), "", musicFrame);
		playButton->setToolTip("Play the selected music");
		playButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *stopButton = new QPushButton(QIcon(g_iconPath + "playback_stop_icon&24.png"), "", musicFrame);
		stopButton->setToolTip("Stop playing the current music");
		stopButton->setMaximumSize(g_buttonsize, g_buttonsize);

		musicLayout->addWidget(_musicSelection);
		musicLayout->addSpacing(20);
		musicLayout->addWidget(playButton);
		musicLayout->addWidget(stopButton);

		connect(playButton, SIGNAL(clicked()), this, SLOT(playMusic()));
		connect(stopButton, SIGNAL(clicked()), this, SLOT(stopMusic()));

		leftLayout->addWidget(musicFrame);
	}
	
	{
		QFrame *buttonFrame = new QFrame(leftFrame);
		QPushButton *acceptButton = new QPushButton("Accept", buttonFrame);
		acceptButton->setDefault(true);
		QPushButton *cancelButton = new QPushButton("Cancel", buttonFrame);
		QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
		buttonLayout->setAlignment(Qt::AlignRight);
		buttonLayout->addWidget(acceptButton);
		buttonLayout->addWidget(cancelButton);

		leftLayout->addWidget(buttonFrame);

		connect(acceptButton, SIGNAL(clicked()), this, SLOT(checkInput()));
		connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	}
	
	{
		QGroupBox *battlebackFrame = new QGroupBox("Battleback", this);
		QVBoxLayout *battlebackLayout = new QVBoxLayout(battlebackFrame);
		battlebackLayout->setAlignment(Qt::AlignTop);

		QFrame *selectionFrame = new QFrame(battlebackFrame);
		QHBoxLayout *selectionLayout = new QHBoxLayout(selectionFrame);

		QLabel *label = new QLabel("Select a background that is used for battles in this environment:", selectionFrame);
		_battlebackSelection = new QComboBox(battlebackFrame);
		_battlebackSelection->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		selectionLayout->addWidget(label);
		selectionLayout->addWidget(_battlebackSelection);

		_battlebackLabel = new QLabel("No battleback selected", battlebackFrame);

		battlebackLayout->addWidget(selectionFrame);
		battlebackLayout->addWidget(_battlebackLabel);

		layout->addWidget(battlebackFrame);

		connect(_battlebackSelection, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(battlebackChanged(const QString &)));
	}
}

void EnvironmentConfigWindow::loadWindow() {
	QStringList musicFilters;
	musicFilters << "*.mp3";

	QDir backgroundDir(g_backgroundMusicPath);
	backgroundDir.setNameFilters(musicFilters);
	_musicSelection->clear();
	_musicSelection->addItem(QString());
	_musicSelection->addItems(backgroundDir.entryList());

	if (_musicSelection->count() > 0)
		_musicSelection->setCurrentIndex(0);

	QStringList imageFilter;
	imageFilter << "*.jpg";
	
	QDir battlebackDir(g_battlebackPath);
	battlebackDir.setNameFilters(imageFilter);
	_battlebackSelection->clear();
	_battlebackSelection->addItem(QString());
	_battlebackSelection->addItems(battlebackDir.entryList());

	if (_battlebackSelection->count() > 0)
		_battlebackSelection->setCurrentIndex(0);
}

QString EnvironmentConfigWindow::getName() const {
	return _nameInput->text();
}

int EnvironmentConfigWindow::getWidth() const {
	return _widthInput->value();
}

int EnvironmentConfigWindow::getHeight() const {
	return _heightInput->value();
}

QString EnvironmentConfigWindow::getBackgroundMusic() const {
	return _musicSelection->currentText();
}

QString EnvironmentConfigWindow::getBattleback() const {
	return _battlebackSelection->currentText();
}

void EnvironmentConfigWindow::setName(QString name) {
	_nameInput->setText(name);
	_oldname = name;
}

void EnvironmentConfigWindow::setWidth(int width) {
	_widthInput->setValue(width);
}

void EnvironmentConfigWindow::setHeight(int height) {
	_heightInput->setValue(height);
}

void EnvironmentConfigWindow::setBackgroundMusic(QString title) {
	for (int i = 0; i < _musicSelection->count(); i++) {
		if (_musicSelection->itemText(i) == title) {
			_musicSelection->setCurrentIndex(i);
			return;
		}
	}
}

void EnvironmentConfigWindow::setBattleback(QString name) {
	for (int i = 0; i < _battlebackSelection->count(); i++) {
		if (_battlebackSelection->itemText(i) == name) {
			_battlebackSelection->setCurrentIndex(i);
			return;
		}
	}
}

void EnvironmentConfigWindow::playMusic() {
	if ((_musicSelection->count() > 0) && (_musicSelection->currentText() != QString())) {
		_backgroundMusicObject->stop();
		_backgroundMusicObject->clearQueue();
		_backgroundMusicObject->setCurrentSource(Phonon::MediaSource(g_backgroundMusicPath + _musicSelection->currentText()));
		_backgroundMusicObject->play();
	}
}

void EnvironmentConfigWindow::stopMusic() {
	_backgroundMusicObject->stop();
	_backgroundMusicObject->clearQueue();
}

void EnvironmentConfigWindow::battlebackChanged(const QString &name) {
	if (_battlebackSelection->count() > 0) {
		_battlebackLabel->clear();
		if (name == QString()) {
			_battlebackLabel->setText("No battleback selected");
		} else {
			_battlebackLabel->setPixmap(QPixmap(g_battlebackPath + name));
		}
	}
}

void EnvironmentConfigWindow::checkInput() {
	if (_nameInput->text() == QString()) {
		QMessageBox::critical(this, tr("No empty name allowed"), tr("An environment should not have an empty name.\nEnter a name for your environment before proceeding."));
		return;
	}

	if (_oldname == QString()) {
		if (!!_project->getEnvironments()->getEnvironment(_nameInput->text())) {
			QMessageBox::critical(this, tr("Name already exists"), tr("An environment with the same name already exists.\nEnter a new name for your environment before proceeding."));
			return;
		}
	} else {
		if (!!_project->getEnvironments()->getEnvironment(_nameInput->text()) && (_nameInput->text() != _oldname)) {
			QMessageBox::critical(this, tr("Name already exists"), tr("An environment with the same name already exists.\nEnter a new name for your environment before proceeding."));
			return;
		}
	}

	accept();
}