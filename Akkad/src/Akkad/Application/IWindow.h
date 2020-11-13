#pragma once
#include <functional>
#include "Event.h"
#include "Akkad/Graphics/RenderPlatform.h"
namespace Akkad {
	
	struct WindowSettings {
		const char* title;
		unsigned int width;
		unsigned int height;
	};

	class Window {

	public:
		virtual int Init(WindowSettings settings) = 0;
		virtual void OnUpdate() = 0;
		virtual bool IsCloseRequested() = 0;
		virtual void SetEventCallback(std::function<void(Event&)> func) = 0;
		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;
		virtual void* GetNativeWindow() = 0;
		virtual void SetVsync(bool status) = 0;
	};

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) {
			m_Type = EventType::WindowEvent;
			m_Width = width;
			m_Height = height;
		}
		unsigned int m_Width;
		unsigned int m_Height;

		virtual EventType GetType() override { return m_Type; };
		EVENT_TYPE(WindowEvent);

	private:
		EventType m_Type;

	};
}
