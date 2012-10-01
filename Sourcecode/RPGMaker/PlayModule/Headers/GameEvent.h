#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include "../Components/Headers/Event.h"

using projectcomponents::Event;

namespace playmodule {

	class GameEvent {
	public:

		GameEvent(Event *event);
		virtual ~GameEvent();

		Event* getEvent() const;
		Event::Type getType() const;
		Event::Trigger getTrigger() const;
		int getExecutionLimit() const;
		GameEvent* getFollowupEvent() const;

		void setFollowupEvent(GameEvent *event);
		void addRequirementEvent(GameEvent *event);

		void execute();
		bool isExecuted() const;
		bool executionPermission() const;

	private:

		Event *_event;
		bool _executed;
		int _executionsRemaining;

		GameEvent *_followup;
		QVector<GameEvent*> _requirements;
	};
}

#endif