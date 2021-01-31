#include "Win32Time.h"

#include <Windows.h>

namespace Akkad {

	Win32TimeManager::Win32TimeManager()
	{
		QueryPerformanceFrequency((LARGE_INTEGER*)&m_TimerFrequency);
		QueryPerformanceCounter((LARGE_INTEGER*)&m_TimerOffset);
	}

	double Win32TimeManager::GetTime() {

		long long int value;
		QueryPerformanceCounter((LARGE_INTEGER*)&value);
		
		return (double)(value - m_TimerOffset) / m_TimerFrequency;

	}

	double Win32TimeManager::GetDeltaTime() {
		return m_deltaTime;
	}

	void Win32TimeManager::CalculateDeltaTime() {
		m_lastFrame = m_newFrame;
		
		m_newFrame = GetTime();

		m_deltaTime = m_newFrame - m_lastFrame;
	}
}