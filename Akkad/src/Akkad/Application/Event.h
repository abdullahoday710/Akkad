#pragma once

#include <functional>
namespace Akkad {

	enum class EventType {
		KeyEvent, WindowResizeEvent, WindowCloseEvent
	};

#define EVENT_TYPE(type) static EventType GetStaticType() {return EventType::type;}

	class Event {
	public:
		virtual EventType GetType() = 0;
		bool Handled = false;
	};

	typedef std::function<void(Event&)> EventFN;

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetType() == T::GetStaticType())
			{
				m_Event.Handled |= func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};
	
}