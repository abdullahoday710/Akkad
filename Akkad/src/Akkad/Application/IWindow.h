#pragma once
#include "Event.h"
#include "Akkad/Graphics/RenderPlatform.h"

#include <glm/glm.hpp>
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
		virtual void SetEventCallback(std::function<void(Event&)> func) = 0;
		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;
		virtual glm::vec2 GetWindowRectMin() = 0;
		virtual glm::vec2 GetWindowRectMax() = 0;
		virtual void* GetNativeWindow() = 0;
	};

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) {
			m_Type = EventType::WindowResizeEvent;
			m_Width = width;
			m_Height = height;
		}
		unsigned int m_Width;
		unsigned int m_Height;

		virtual EventType GetType() override { return m_Type; };
		EVENT_TYPE(WindowResizeEvent);

	private:
		EventType m_Type;

	};

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() { m_Type = EventType::WindowCloseEvent; };
		virtual EventType GetType() override { return m_Type; };

		EVENT_TYPE(WindowCloseEvent);

	private:
		EventType m_Type;
	};
}
