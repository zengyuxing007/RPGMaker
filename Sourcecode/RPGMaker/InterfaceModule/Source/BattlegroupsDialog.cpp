#include "qdir.h"
#include "qframe.h"
#include "qlabel.h"
#include "qgroupbox.h"
#include "qboxlayout.h"
#include "qstringlist.h"
#include "qpushbutton.h"
#include "qheaderview.h"
#include "qmessagebox.h"
#include "qgridlayout.h"
#include "qinputdialog.h"
#include "phonon/mediasource.h"

#include "../ProgramDefaults.h"
#include "../Headers/BattlegroupsDialog.h"

using namespace interfacemodule;

BattlegroupsDialog::BattlegroupsDialog(QWidget *parent, Project *project)
	: QDialog(parent), _project(project) {
		createWindow();
		setWindowModality(Qt::ApplicationModal);
		setWindowTitle("Battlegroup configuration");

		loadBattlegroups();

		if (_groupsList->count() > 0)
			_groupsList->setCurrentRow(0);
}

BattlegroupsDialog::~BattlegroupsDialog() {}

void BattlegroupsDialog::createWindow() {
	QGridLayout *layout = new QGridLayout(this);

	{
		QGroupBox *groupsFrame = new QGroupBox("Battlegroups", this);
		QVBoxLayout *groupsLayout = new QVBoxLayout(groupsFrame);

		QFrame *buttonFrame = new QFrame(groupsFrame);
		QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
		QPushButton *newGroupButton = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", buttonFrame);
		newGroupButton->setToolTip("Add a new battlegroup");
		newGroupButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *editGroupButton = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", buttonFrame);
		editGroupButton->setToolTip(tr("Edit the current battlegroup"));
		editGroupButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *deleteGroupButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", buttonFrame);
		deleteGroupButton->setToolTip(tr("Delete the current battlegroup"));
		deleteGroupButton->setMaximumSize(g_buttonsize, g_buttonsize);
		buttonLayout->addWidget(newGroupButton);
		buttonLayout->addWidget(editGroupButton);
		buttonLayout->addWidget(deleteGroupButton);
		buttonLayout->addSpacing(20);

		QLabel *label = new QLabel("Select a battlegroup:", buttonFrame);
		buttonLayout->addWidget(label);

		_groupsList = new QListWidget(groupsFrame);

		groupsLayout->addWidget(buttonFrame);
		groupsLayout->addWidget(_groupsList);

		layout->addWidget(groupsFrame, 0, 0, 2, 1);

		connect(_groupsList, SIGNAL(currentTextChanged(const QString &)), this, SLOT(groupSelected(const QString &)));
		connect(newGroupButton, SIGNAL(clicked()), this, SLOT(newBattlegroup()));
		connect(editGroupButton, SIGNAL(clicked()), this, SLOT(editBattlegroup()));
		connect(deleteGroupButton, SIGNAL(clicked()), this, SLOT(deleteBattlegroup()));
	}

	{
		QGroupBox *membersFrame = new QGroupBox("Enemies", this);
		QVBoxLayout *membersLayout = new QVBoxLayout(membersFrame);

		QFrame *buttonFrame = new QFrame(membersFrame);
		QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
		QPushButton *addEnemyButton = new QPushButton(QIcon(g_iconPath + "sq_plus_icon&24.png"), "", buttonFrame);
		addEnemyButton->setToolTip("Add an enemy to the battlegroup");
		addEnemyButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *removeEnemyButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", buttonFrame);
		removeEnemyButton->setToolTip(tr("Remove the selected enemy from the battlegroup"));
		removeEnemyButton->setMaximumSize(g_buttonsize, g_buttonsize);
		buttonLayout->addWidget(addEnemyButton);
		buttonLayout->addWidget(removeEnemyButton);
		buttonLayout->addSpacing(20);

		QLabel *label = new QLabel("Add or remove enemies from the battlegroup:", buttonFrame);
		buttonLayout->addWidget(label);

		_membersList = new QTableWidget(membersFrame);
		_membersList->clear();
		_membersList->setEditTriggers(QAbstractItemView::NoEditTriggers);
		_membersList->setSelectionBehavior(QAbstractItemView::SelectItems);
		_membersList->setSelectionMode(QAbstractItemView::SingleSelection);
		_membersList->verticalHeader()->setResizeMode(QHeaderView::Fixed);
		_membersList->horizontalHeader()->hide();
		_membersList->setColumnCount(1);
		_membersList->setRowCount(0);

		membersLayout->addWidget(buttonFrame);
		membersLayout->addWidget(_membersList);

		layout->addWidget(membersFrame, 0, 1, 2, 1);

		connect(addEnemyButton, SIGNAL(clicked()), this, SLOT(addEnemy()));
		connect(removeEnemyButton, SIGNAL(clicked()), this, SLOT(removeEnemy()));
	}

	{
		QGroupBox *musicFrame = new QGroupBox("Battle music", this);
		QVBoxLayout *musicLayout = new QVBoxLayout(musicFrame);

		QLabel *musicLabel = new QLabel("Select the background music when encountering this battlegroup:", musicFrame);
		_backgroundmusicSelection = new QComboBox(musicFrame);
		_backgroundmusicSelection->setSizeAdjustPolicy(QComboBox::AdjustToContents);

		_audioOutputObject = new Phonon::AudioOutput(Phonon::MusicCategory, musicFrame);
		_backgroundMusicObject  = new Phonon::MediaObject(this);
		Phonon::createPath(_backgroundMusicObject, _audioOutputObject);

		QFrame *musicButtonFrame = new QFrame(musicFrame);
		QHBoxLayout *musicButtonLayout = new QHBoxLayout(musicButtonFrame);
		musicButtonLayout->setAlignment(Qt::AlignRight);
		QPushButton *playButton = new QPushButton(QIcon(g_iconPath + "playback_play_icon&24.png"), "", musicFrame);
		playButton->setToolTip("Play the selected music");
		playButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *stopButton = new QPushButton(QIcon(g_iconPath + "playback_stop_icon&24.png"), "", musicFrame);
		stopButton->setToolTip("Stop playing the current music");
		stopButton->setMaximumSize(g_buttonsize, g_buttonsize);
		musicButtonLayout->addWidget(playButton);
		musicButtonLayout->addWidget(stopButton);

		musicLayout->addWidget(musicLabel);
		musicLayout->addWidget(_backgroundmusicSelection);
		musicLayout->addWidget(musicButtonFrame);

		layout->addWidget(musicFrame, 0, 2, 1, 1);

		connect(playButton, SIGNAL(clicked()), this, SLOT(playBackgroundMusic()));
		connect(stopButton, SIGNAL(clicked()), this, SLOT(stopBackgroundMusic()));
		connect(_backgroundmusicSelection, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(backgroundMusicSelected(const QString&)));
	}

	{
		QGroupBox *musicFrame = new QGroupBox("Victory music", this);
		QVBoxLayout *musicLayout = new QVBoxLayout(musicFrame);

		QLabel *musicLabel = new QLabel("Select the victory music when triumphing over this battlegroup:", musicFrame);
		_victorymusicSelection = new QComboBox(musicFrame);
		_victorymusicSelection->setSizeAdjustPolicy(QComboBox::AdjustToContents);

		QFrame *musicButtonFrame = new QFrame(musicFrame);
		QHBoxLayout *musicButtonLayout = new QHBoxLayout(musicButtonFrame);
		musicButtonLayout->setAlignment(Qt::AlignRight);
		QPushButton *playButton = new QPushButton(QIcon(g_iconPath + "playback_play_icon&24.png"), "", musicFrame);
		playButton->setToolTip("Play the selected music");
		playButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *stopButton = new QPushButton(QIcon(g_iconPath + "playback_stop_icon&24.png"), "", musicFrame);
		stopButton->setToolTip("Stop playing the current music");
		stopButton->setMaximumSize(g_buttonsize, g_buttonsize);
		musicButtonLayout->addWidget(playButton);
		musicButtonLayout->addWidget(stopButton);

		musicLayout->addWidget(musicLabel);
		musicLayout->addWidget(_victorymusicSelection);
		musicLayout->addWidget(musicButtonFrame);

		layout->addWidget(musicFrame, 1, 2, 1, 1);

		connect(playButton, SIGNAL(clicked()), this, SLOT(playVictoryMusic()));
		connect(stopButton, SIGNAL(clicked()), this, SLOT(stopVictoryMusic()));
		connect(_victorymusicSelection, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(victoryMusicSelected(const QString&)));
	}
}

void BattlegroupsDialog::loadBattlegroups() {
	_groupsList->clear();
	_backgroundmusicSelection->clear();
	_victorymusicSelection->clear();
	_membersList->clear();
	_membersList->setRowCount(0);
	_backgroundMusicObject->stop();
	_backgroundMusicObject->clearQueue();

	// Load battlegoups
	QVector<Battlegroup*> groups = _project->getBattlegroups()->getBattlegroups();
	for (int i = 0; i < groups.size(); i++) {
		Battlegroup *it = groups.at(i);
		if (!!it)
			_groupsList->addItem(it->getName());
	}

	// Load music
	QStringList musicFilters;
	musicFilters << "*.mp3";

	QDir backgroundDir(g_backgroundMusicPath);
	backgroundDir.setNameFilters(musicFilters);
	_backgroundmusicSelection->addItem(QString());
	_backgroundmusicSelection->addItems(backgroundDir.entryList());
	_victorymusicSelection->addItem(QString());
	_victorymusicSelection->addItems(backgroundDir.entryList());

	if (_backgroundmusicSelection->count() > 0)
		_backgroundmusicSelection->setCurrentIndex(0);

	if (_victorymusicSelection->count() > 0)
		_victorymusicSelection->setCurrentIndex(0);
}

void BattlegroupsDialog::groupSelected(const QString &name) {
	if ((_groupsList->count() > 0) && !!_groupsList->currentItem()) {
		Battlegroup *group = _project->getBattlegroups()->getBattlegroup(name);
		if (!!group) {
			_membersList->clear();
			_membersList->setRowCount(g_maxBattlegroupsize);
			_backgroundMusicObject->stop();
			_backgroundMusicObject->clearQueue();

			QVector<Enemy*> members = group->getEnemies();
			for (int i = 0; i < members.size(); i++) {
				Enemy *it = members.at(i);
				if (!!it)
					_membersList->setItem(i, 0, new QTableWidgetItem(it->getName()));
			}

			
			if (group->getBackgroundMusic() != QString()) {
				for (int i = 0; i < _backgroundmusicSelection->count(); i++) {
					if (_backgroundmusicSelection->itemText(i) == group->getBackgroundMusic()) {
						_backgroundmusicSelection->setCurrentIndex(i);
						break;
					}
				}
			} else
				_backgroundmusicSelection->setCurrentIndex(0);

			if (group->getVictoryMusic() != QString()) {
				for (int i = 0; i < _victorymusicSelection->count(); i++) {
					if (_victorymusicSelection->itemText(i) == group->getVictoryMusic()) {
						_victorymusicSelection->setCurrentIndex(i);
						break;
					}
				}
			} else
				_victorymusicSelection->setCurrentIndex(0);
		}
	}
}

void BattlegroupsDialog::newBattlegroup() {
	bool accepted;
	QString name = QInputDialog::getText(this, tr("New battlegroup"),
		tr("Enter a name for the battlegroup.\nThis name should be unique for all battlegroups."),
		QLineEdit::Normal, "", &accepted);

	if (accepted) {
		QRegExpValidator validator(g_nameRegex);
		int pos = 0;
		if (validator.validate(name, pos) != QValidator::Acceptable) {
			QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
			newBattlegroup();
			return;
		}

		if (name == QString()) {
			QMessageBox::critical(this, tr("No empty name allowed"), tr("A battlegroup should not have an empty name."));
			newBattlegroup();
			return;
		}

		if (!!_project->getBattlegroups()->getBattlegroup(name)) {
			QMessageBox::critical(this, tr("Name already in use"), tr("A battlegroup with the same name already exists."));
			newBattlegroup();
			return;
		}

		Battlegroup *group = Battlegroup::create(name);
		try {
			_project->getBattlegroups()->addBattlegroup(group);
		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error has occurred"), e.what());
			delete group;
			return;
		}

		loadBattlegroups();
		_groupsList->setCurrentRow(_groupsList->count() -1);
	}
}

void BattlegroupsDialog::editBattlegroup() {
	if ((_groupsList->count() > 0) && !!_groupsList->currentItem()) {
		Battlegroup *group = _project->getBattlegroups()->getBattlegroup(_groupsList->currentItem()->text());
		if (!!group) {
			bool accepted;
			QString name = QInputDialog::getText(this, tr("Enter battlegroup"),
				tr("Enter a new name for the battlegroup:"),
				QLineEdit::Normal, group->getName(), &accepted);

			if (accepted && (name != group->getName())) {
				QRegExpValidator validator(g_nameRegex);
				int pos = 0;
				if (validator.validate(name, pos) != QValidator::Acceptable) {
					QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
					editBattlegroup();
					return;
				}

				if (name == QString()) {
					QMessageBox::critical(this, tr("No empty name allowed"), tr("A battlegroup should not have an empty name."));
					editBattlegroup();
					return;
				}

				if (!!_project->getBattlegroups()->getBattlegroup(name)) {
					QMessageBox::critical(this, tr("Name already in use"), tr("A battlegroup with the same name already exists."));
					editBattlegroup();
					return;
				}

				group->setName(name);
				_groupsList->currentItem()->setText(name);
			}
		}
	}
}

void BattlegroupsDialog::deleteBattlegroup() {
	if ((_groupsList->count() > 0) && !!_groupsList->currentItem()) {

		QString groupName = _groupsList->currentItem()->text();
		Battlegroup *group = _project->getBattlegroups()->getBattlegroup(groupName);

		if (!!group) {
			QMessageBox::StandardButton response = QMessageBox::warning(this, tr("Delete battlegroup"),
				tr("Are you sure you want to delete the battlegroup?\nDeleting a battlegroup will delete all references to it and cannot be undone."),
				QMessageBox::Ok | QMessageBox::Cancel);

			if (response == QMessageBox::Ok) {
				_project->getBattlegroups()->removeBattlegroup(groupName);
				loadBattlegroups();
				if (_groupsList->count() > 0)
					_groupsList->setCurrentRow(0);
			}
		}
	}
}

void BattlegroupsDialog::addEnemy() {
	if ((_groupsList->count() > 0) && !!_groupsList->currentItem()) {
		Battlegroup *group = _project->getBattlegroups()->getBattlegroup(_groupsList->currentItem()->text());
		if (!!group) {

			if (group->getEnemies().size() == g_maxBattlegroupsize) {
				QMessageBox::information(this, tr("Battlegroup is full"),
					tr("The battlegroup is full. No more enemies can join the battlegroup."));
				return;
			}

			QStringList enemiesList;
			QVector<Enemy*> enemies = _project->getEnemies()->getEnemies();
			for (int i =0; i < enemies.size(); i++) {
				Enemy *it = enemies.at(i);
				if (!!it)
					enemiesList << it->getName();
			}

			if (enemiesList.size() == 0) {
				QMessageBox::information(this, tr("No enemies available"),
					tr("There are no enemies available to choose from."));
				return;
			}

			bool accepted;
			QString name = QInputDialog::getItem(this, tr("Add enemy to battlegroup"),
				tr("Select an enemy to add to the battlegroup:"), enemiesList, 0, false, &accepted);

			if (accepted) {
				Enemy *enemy = _project->getEnemies()->getEnemy(name);
				if (!!enemy) {
					try {
						group->addEnemy(enemy);
					} catch (ProjectException &e) {
						QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
						return;
					}

					groupSelected(group->getName());
				}
			}
		}
	}
}

void BattlegroupsDialog::removeEnemy() {
	if ((_groupsList->count() > 0) && !!_groupsList->currentItem() && !!_membersList->currentItem()) {
		Battlegroup *group = _project->getBattlegroups()->getBattlegroup(_groupsList->currentItem()->text());
		if (!!group) {

			int enemyIndex = _membersList->currentRow();
			try {
				group->removeEnemy(enemyIndex);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			groupSelected(group->getName());
		}
	}
}

void BattlegroupsDialog::backgroundMusicSelected(const QString &name) {
	if ((_groupsList->count() > 0) && !!_groupsList->currentItem()) {
		Battlegroup *group = _project->getBattlegroups()->getBattlegroup(_groupsList->currentItem()->text());
		if (!!group) {
			try {
				group->setBackgroundMusic(_backgroundmusicSelection->currentText());
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An error has occurred"), e.what());
			}
		}
	}
}

void BattlegroupsDialog::playBackgroundMusic() {
	if ((_backgroundmusicSelection->count() > 0) && (_backgroundmusicSelection->currentText() != QString())) {
		_backgroundMusicObject->stop();
		_backgroundMusicObject->clearQueue();
		_backgroundMusicObject->setCurrentSource(Phonon::MediaSource(g_backgroundMusicPath + _backgroundmusicSelection->currentText()));
		_backgroundMusicObject->play();
	}
}

void BattlegroupsDialog::stopBackgroundMusic() {
	_backgroundMusicObject->stop();
	_backgroundMusicObject->clearQueue();
}

void BattlegroupsDialog::victoryMusicSelected(const QString &name) {
	if ((_groupsList->count() > 0) && !!_groupsList->currentItem()) {
		Battlegroup *group = _project->getBattlegroups()->getBattlegroup(_groupsList->currentItem()->text());
		if (!!group) {
			try {
				group->setVictoryMusic(_victorymusicSelection->currentText());
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An error has occurred"), e.what());
			}
		}
	}
}

void BattlegroupsDialog::playVictoryMusic() {
	if ((_victorymusicSelection->count() > 0) && (_victorymusicSelection->currentText() != QString())) {
		_backgroundMusicObject->stop();
		_backgroundMusicObject->clearQueue();
		_backgroundMusicObject->setCurrentSource(Phonon::MediaSource(g_backgroundMusicPath + _victorymusicSelection->currentText()));
		_backgroundMusicObject->play();
	}
}

void BattlegroupsDialog::stopVictoryMusic() {
	_backgroundMusicObject->stop();
	_backgroundMusicObject->clearQueue();
}