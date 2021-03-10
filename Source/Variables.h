#pragma once

#include <vector>
#include <string>
#include <chrono>

#include "All.h"

struct event {
	double time;
	int type;
	int val1;
	int val2;
};

struct track {
	std::vector <event> action;
	std::string instrname;
	std::string trackname;
	int noignored;
	int nounknown;
};

struct options {
	bool movetofit;
	bool dodelay;
	bool realpedal;
	bool fullpiano;
	bool dovolume;
	int transpose;

	int shortnotes;
};

class Timer {
public:
	void start();
	void stop();

	double elapsedMilliseconds();
	double elapsedSeconds();

private:
	std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
	std::chrono::time_point<std::chrono::steady_clock> m_EndTime;
	bool                                               m_bRunning = false;
};