#pragma once
#include "Akkad/Application/TimeManager.h"

namespace Akkad {
	class Win32TimeManager : public TimeManager
	{
	public:
		Win32TimeManager();
		~Win32TimeManager() {};

		virtual double GetTime() override;
		virtual double GetDeltaTime() override;

	private:
		virtual void CalculateDeltaTime() override;

		unsigned long long int m_TimerOffset = 0;
		unsigned long long int m_TimerFrequency = 0;

		double m_newFrame = 0;
		double m_lastFrame = 0;

		double m_deltaTime = 0;
		
	};
}