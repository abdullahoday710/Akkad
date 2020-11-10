#pragma once

namespace Akkad {

	class Time {
	public:
		static double GetTime();
		static double GetDeltaTime();
	private:
		friend class Application;

		static void Init();
		static void CalculateDeltaTime();
	};
}