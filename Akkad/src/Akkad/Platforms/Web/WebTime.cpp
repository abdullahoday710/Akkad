#include "WebTime.h"

#include <emscripten/emscripten.h>
namespace Akkad {

	WebTime::WebTime()
	{
		m_TimerOffset = emscripten_get_now();
	}

	double WebTime::GetTime()
	{
		double now = emscripten_get_now();
		return now - m_TimerOffset;
	}

	double WebTime::GetDeltaTime()
	{
		return m_DeltaTime;
	}

	void WebTime::CalculateDeltaTime()
	{
		m_LastFrame = m_NewFrame;

		m_NewFrame = GetTime();

		m_DeltaTime = (m_NewFrame - m_LastFrame) / 1000;
	}
}