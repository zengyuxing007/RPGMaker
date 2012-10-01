#include "../ProgramDefaults.h"
#include "../Headers/SoundItem.h"

using namespace playmodule;

SoundItem::SoundItem(QObject *parent)
	: QObject(parent),
	_backgroundNode(new MediaObject(this), MediaSource(QString())),
	_outputNode(new AudioOutput(this))
{
	Phonon::createPath(_backgroundNode.first, _outputNode);
	connect(_backgroundNode.first, SIGNAL(finished()), this, SLOT(backgroundPlaybackFinished()));
}

SoundItem::~SoundItem() {}

void SoundItem::stop() {
	_backgroundNode.first->stop();
	_backgroundNode.first->clear();
}

MediaSource SoundItem::getBackgroundSource() const {
	return _backgroundNode.first->currentSource();
}

void SoundItem::setBackgroundSource(MediaSource newSource) {
	if (newSource.type() != MediaSource::LocalFile)
		return;

	if (_backgroundNode.first->state() == Phonon::ErrorState) {
		stop();
	}

	if ((_backgroundNode.first->state() != Phonon::PlayingState)
		|| (_backgroundNode.first->currentSource().fileName() != newSource.fileName())) {
		stop();
		_backgroundNode.first->setCurrentSource(newSource);
		_backgroundNode.first->play();
		_backgroundNode.second = newSource;
	}
}

void SoundItem::backgroundPlaybackFinished() {
	stop();
	_backgroundNode.first->setCurrentSource(_backgroundNode.second);
	_backgroundNode.first->play();
}