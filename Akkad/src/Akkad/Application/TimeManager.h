#pragma once

namespace Akkad {

	class TimeManager {
	public:
		virtual double GetTime() = 0;
		virtual double GetDeltaTime() = 0;
	protected:
		friend class Application;
		virtual void CalculateDeltaTime() = 0;
	};
}