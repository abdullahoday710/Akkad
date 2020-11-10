#include "Akkad/Application/Time.h"
#include <Windows.h>

namespace Akkad {

	unsigned long long int TimerOffset;
	unsigned long long int TimerFrequency;

	double newFrame;
	double lastFrame = 0;

	double deltaTime;

	void Time::Init() {
		QueryPerformanceFrequency((LARGE_INTEGER*)&TimerFrequency);
		QueryPerformanceCounter((LARGE_INTEGER*)&TimerOffset);
	}

	double Time::GetTime() {

		long long int value;
		QueryPerformanceCounter((LARGE_INTEGER*)&value);
		
		return (double)(value - TimerOffset) / TimerFrequency;

	}

	double Time::GetDeltaTime() {
		return deltaTime;
	}

	void Time::CalculateDeltaTime() {
		lastFrame = newFrame;
		
		newFrame = GetTime();

		deltaTime = newFrame - lastFrame;
	}
}