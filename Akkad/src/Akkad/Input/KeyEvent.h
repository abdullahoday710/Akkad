#pragma once
#include "Akkad/Application/Event.h"

namespace Akkad {

	class KeyPressEvent : public Event {
	public:
		KeyPressEvent(unsigned int keycode) {
			m_Type = EventType::KeyEvent;
			m_KeyCode = keycode;
		}
		unsigned int m_KeyCode;
		virtual EventType GetType() override { return m_Type; };
		EVENT_TYPE(KeyEvent);

	private:
		EventType m_Type;

	};
}