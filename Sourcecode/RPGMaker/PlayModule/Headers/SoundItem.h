#ifndef SOUNDITEM_H
#define SOUNDITEM_H

#include "qpair.h"
#include "qobject.h"
#include "qvector.h"
#include "phonon/mediasource.h"
#include "phonon/mediaobject.h"
#include "phonon/audiooutput.h"

#include "../Components/Headers/Environment.h"

using Phonon::MediaSource;
using Phonon::MediaObject;
using Phonon::AudioOutput;
using projectcomponents::Environment;

namespace playmodule {

	class SoundItem : public QObject {

		Q_OBJECT

	public:

		SoundItem(QObject *parent);
		virtual ~SoundItem();
		
		void stop();
		MediaSource getBackgroundSource() const;
		void setBackgroundSource(MediaSource newSource);

	private:
		QPair<MediaObject*, MediaSource> _backgroundNode;
		AudioOutput *_outputNode;

	private slots:
		void backgroundPlaybackFinished();
	};
}

#endif