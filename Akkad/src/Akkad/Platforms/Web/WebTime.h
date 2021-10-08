#pragma once
#include "Akkad/Application/TimeManager.h"
namespace Akkad {

	class WebTime : public TimeManager
	{
	public:
		WebTime();
		virtual double GetTime() override;
		virtual double GetDeltaTime() override;
	protected:
		virtual void CalculateDeltaTime() override;

		double m_TimerOffset = 0;
		double m_LastFrame = 0;
		double m_NewFrame = 0;
		double m_DeltaTime = 0;
	};
}


