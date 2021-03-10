#include <chrono>

#include "All.h"

using namespace std;

void Timer::start() {
	m_StartTime = std::chrono::steady_clock::now();
	m_bRunning = true;
}
void Timer::stop() {
	m_EndTime = std::chrono::steady_clock::now();
	m_bRunning = false;
}

double Timer::elapsedMilliseconds() {
	std::chrono::time_point<std::chrono::steady_clock> endTime;

	if (m_bRunning)
	{
		endTime = std::chrono::steady_clock::now();
	}
	else
	{
		endTime = m_EndTime;
	}

	return (double)std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
}
double Timer::elapsedSeconds() {
	return elapsedMilliseconds() / 1000.0;
}