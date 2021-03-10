#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <winuser.h>

#include "All.h"

bool IsShift(int pos) {
	if (36 <= pos && pos <= 96) {
		return pos % 12 == 1 || pos % 12 == 3 || pos % 12 == 6 || pos % 12 == 8 || pos % 12 == 10;
	}
	else {
		return false;
	}
}
bool KeySort(event num1, event num2) {
	if (num1.time == num2.time) {
		if (num1.type == num2.type) {
			if (num1.type == 1) {
				return num1.val1 < num2.val1;
			}
			else if (num1.type == 2) {
				if (IsShift(num1.val1)) {
					if (IsShift(num2.val1)) {
						return num1.val2 < num2.val2;
					}
					else {
						return false;
					}
				}
				else {
					if (IsShift(num2.val1)) {
						return true;
					}
					else {
						return num1.val2 > num2.val2;
					}
				}
			}
			else if (num1.type == 3) {
				return num1.val1 < num2.val1;
			}
			else if (num1.type == 4) {
				return num1.val1 < num2.val1;
			}
			else if (num1.type == 5) {
				return num1.val1 < num2.val1;
			}
			else {
				///Further types
				return true;
			}
		}
		else {
			return num1.type > num2.type;
		}
	}
	else {
		return num1.time < num2.time;
	}
}
//Some functions to easily press the keys required

extern bool playmode;

options GetOptions() {
	options readopt;
	std::string getstr;
	std::ifstream fin("C:\\Users\\Cezar T\\Desktop\\Midis\\options.txt");
	//movetofit
	fin >> getstr >> getstr;
	if (getstr == "1" || getstr == "true") {
		readopt.movetofit = true;
	}
	else {
		readopt.movetofit = false;
	}
	//dodelay
	fin >> getstr >> getstr;
	if (getstr == "1" || getstr == "true") {
		readopt.dodelay = true;
	}
	else {
		readopt.dodelay = false;
	}
	//realpedal
	fin >> getstr >> getstr;
	if (getstr == "1" || getstr == "true") {
		readopt.realpedal = true;
	}
	else {
		readopt.realpedal = false;
	}
	//fullpiano
	fin >> getstr >> getstr;
	if (getstr == "1" || getstr == "true") {
		readopt.fullpiano = true;
	}
	else {
		readopt.fullpiano = false;
	}
	//dovolume
	fin >> getstr >> getstr;
	if (getstr == "1" || getstr == "true") {
		readopt.dovolume = true;
	}
	else {
		readopt.dovolume = false;
	}
	//transpose
	fin >> getstr >> getstr;
	int put = 1;
	readopt.transpose = 0;
	for (int index = getstr.size() - 1; index >= 0; index--) {
		readopt.transpose += put * (getstr[index] - '0');
		put *= 10;
	}

	//additional modifyers
	readopt.shortnotes = -1;
	while (fin >> getstr) {
		if (getstr == "shortnotes") {
			fin >> getstr;
			if (getstr == "false" || getstr == "-1") {
				readopt.shortnotes = -1;
			}
			else {
				put = 1;
				readopt.shortnotes = 0;
				for (int index = getstr.size() - 1; index >= 0; index--) {
					readopt.shortnotes += put * (getstr[index] - '0');
					put *= 10;
				}
			}
		}
		if (getstr == "end") {
			break;
		}
	}
	fin.close();
	return readopt;
}
///Reads the options for the program from a text file

bool spacestate = false;
bool shiftstate = false;
bool ctrlstate = false;
bool softstate = false;
bool pedalpress = false;

bool torelease[260];
bool pressed[260];

extern double tempo;

double PlayEvent(event toplay, double globaltime, options opt) {

	char volume[32] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z', 'X', 'C' };
	char keys[88] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'Q', 'W', 'E', 'R', 'T', '1', '1', '2', '2', '3', '4', '4', '5', '5', '6', '6', '7', '8', '8', '9', '9', '0', 'Q', 'Q', 'W', 'W', 'E', 'E', 'R', 'T', 'T', 'Y', 'Y', 'U', 'I', 'I', 'O', 'O', 'P', 'P', 'A', 'S', 'S', 'D', 'D', 'F', 'G', 'G', 'H', 'H', 'J', 'J', 'K', 'L', 'L', 'Z', 'Z', 'X', 'C', 'C', 'V', 'V', 'B', 'B', 'N', 'M', 'Y', 'U', 'I', 'O', 'P', 'A', 'S', 'D', 'F', 'G', 'H', 'J' };

	double delay = 0;

	static int curvol = -1;

	///Key processing
	if (toplay.type == 1 || toplay.type == 2) {
		toplay.val1 += opt.transpose;
		toplay.val1 -= 21;
		if (opt.movetofit == true) {
			if (opt.fullpiano == false) {
				while (toplay.val1 < 15) {
					toplay.val1 += 12;
				}
				while (toplay.val1 > 75) {
					toplay.val1 -= 12;
				}
			}
			else {
				while (toplay.val1 < 0) {
					toplay.val1 += 12;
				}
				while (toplay.val1 > 87) {
					toplay.val1 -= 12;
				}
			}
		}
		else {
			if (opt.fullpiano == false) {
				if (toplay.val1 < 15) {
					toplay.val1 += 12;
				}
				if (toplay.val1 > 75) {
					toplay.val1 -= 12;
				}
			}
			else {
				if (toplay.val1 < 0) {
					toplay.val1 += 12;
				}
				if (toplay.val1 > 87) {
					toplay.val1 -= 12;
				}
			}
		}
	}

	///Volume
	if (opt.dovolume == true && toplay.type == 2) {
		if (softstate == false) {
			if (toplay.val2 / 4 != curvol) {
				curvol = toplay.val2 / 4;
				keybd_event(VK_MENU, MapVirtualKeyA(VK_MENU, 0), 0, 0);
				keybd_event(volume[toplay.val2 / 4], MapVirtualKeyA(volume[toplay.val2 / 4], 0), 0, 0);
				keybd_event(volume[toplay.val2 / 4], MapVirtualKeyA(volume[toplay.val2 / 4], 0), KEYEVENTF_KEYUP, 0);
				keybd_event(VK_MENU, MapVirtualKeyA(VK_MENU, 0), KEYEVENTF_KEYUP, 0);
			}
		}
		else {
			if (toplay.val2 / 4 / 2 != curvol) {
				curvol = toplay.val2 / 4 / 2;
				keybd_event(VK_MENU, MapVirtualKeyA(VK_MENU, 0), 0, 0);
				keybd_event(volume[toplay.val2 / 4 / 2], MapVirtualKeyA(volume[toplay.val2 / 4 / 2], 0), 0, 0);
				keybd_event(volume[toplay.val2 / 4 / 2], MapVirtualKeyA(volume[toplay.val2 / 4 / 2], 0), KEYEVENTF_KEYUP, 0);
				keybd_event(VK_MENU, MapVirtualKeyA(VK_MENU, 0), KEYEVENTF_KEYUP, 0);
			}
		}
	}

	///Key up
	if (toplay.type == 1) {
		if (pedalpress) {
			torelease[keys[toplay.val1]] = true;
		}
		else {
			if (pressed[keys[toplay.val1]] == true) {
				keybd_event(keys[toplay.val1], MapVirtualKeyA(keys[toplay.val1], 0), KEYEVENTF_KEYUP, 0);
				pressed[keys[toplay.val1]] = false;
			}

		}
	}

	///Key down
	if (toplay.type == 2) {
		///Shift shenanigans
		if (IsShift(toplay.val1 + 21) == false) {
			if (shiftstate == true) {
				keybd_event(VK_SHIFT, MapVirtualKeyA(VK_SHIFT, 0), KEYEVENTF_KEYUP, 0);
				shiftstate = false;
			}
		}
		else {
			if (shiftstate == false) {
				keybd_event(VK_SHIFT, MapVirtualKeyA(VK_SHIFT, 0), 0, 0);
				shiftstate = true;
			}
		}

		///Ctrl shenanigans
		if (opt.fullpiano == true) {
			if (toplay.val1 < 15 || 75 < toplay.val1) {
				if (ctrlstate == false) {
					keybd_event(VK_CONTROL, MapVirtualKeyA(VK_CONTROL, 0), 0, 0);
					ctrlstate = true;
				}
			}
			else {
				if (ctrlstate == true) {
					keybd_event(VK_CONTROL, MapVirtualKeyA(VK_CONTROL, 0), KEYEVENTF_KEYUP, 0);
					ctrlstate = false;
				}
			}
		}
		if (pressed[keys[toplay.val1]] == true) {
			keybd_event(keys[toplay.val1], MapVirtualKeyA(keys[toplay.val1], 0), KEYEVENTF_KEYUP, 0);
		}
		keybd_event(keys[toplay.val1], MapVirtualKeyA(keys[toplay.val1], 0), 0, 0);
		pressed[keys[toplay.val1]] = true;
		torelease[keys[toplay.val1]] = false;
	}

	///Sustain shenanigans
	if (toplay.type == 3) {
		if (opt.realpedal == 1) {
			if (toplay.val1 == 1) {
				if (spacestate == false) {
					keybd_event(VK_SPACE, MapVirtualKeyA(VK_SPACE, 0), 0, 0);
					spacestate = true;
				}
			}
			else {
				if (spacestate == true) {
					keybd_event(VK_SPACE, MapVirtualKeyA(VK_SPACE, 0), KEYEVENTF_KEYUP, 0);
					spacestate = false;
				}
			}
		}
		else {
			if (toplay.val1 == 1) {
				pedalpress = true;
			}
			else {
				for (int index = 0; index < 260; index++) {
					if (pressed[keys[toplay.val1]] == true && torelease[keys[toplay.val1]] == true) {
						keybd_event(keys[toplay.val1], MapVirtualKeyA(keys[toplay.val1], 0), KEYEVENTF_KEYUP, 0);
						pressed[keys[toplay.val1]] = false;
					}
					torelease[keys[toplay.val1]] = false;
				}
				pedalpress = false;
			}
		}
	}

	///Soft pedal shenanigans
	if (toplay.type == 5) {
		if (toplay.val1 == 1) {
			softstate = true;
		}
		else {
			softstate = false;
		}
	}

	///Delay calc
	if (opt.dodelay == true) {
		delay = globaltime - (double)toplay.time * tempo;
	}

	return delay;
}
//Turns a cerain event to consecutive key presses

Timer deltatime;

extern int tickdiv;

extern int format;

void PlayKeys(std::vector <event> events, options opt) {
	
	int maxnote = -100;
	if (opt.transpose == 99) {
		for (int index = 0; index < (int)events.size(); index++) {
			if (maxnote < events[index].val1 && events[index].type == 1) {
				maxnote = events[index].val1;
			}
		}
		if (opt.fullpiano == false) {
			opt.transpose = 96 - maxnote;
		}
		else {
			opt.transpose = 108 - maxnote;
		}
		std::cout << "Transpose value is " << opt.transpose << "." << std::endl;
	}

	///Sorting events
	std::cout << "Sorting events..." << std::endl;
	std::sort(events.begin(), events.end(), KeySort);

	///Playing
	std::cout << "Playing in 5 seconds" << std::endl;
	Sleep(5000);
	std::cout << "Playing" << std::endl;
	int avrgcnt = 0;
	
	double delay, maxdelay = -100, avrgdelay = 0;
	int cnt = 0;

	deltatime.start();
	while (cnt < (int)events.size()) {
		if (deltatime.elapsedMilliseconds() > (double)events[cnt].time * tempo) {
			delay = PlayEvent(events[cnt], deltatime.elapsedMilliseconds(), opt);
			if (opt.dodelay == true) {
				if (maxdelay < delay) {
					maxdelay = delay;
				}
				avrgdelay += delay;
				avrgcnt++;
			}
			cnt++;
		}
	}
	if (shiftstate) {
		keybd_event(VK_SHIFT, MapVirtualKeyA(VK_SHIFT, 0), KEYEVENTF_KEYUP, 0);
	}
	if (ctrlstate) {
		keybd_event(VK_CONTROL, MapVirtualKeyA(VK_CONTROL, 0), KEYEVENTF_KEYUP, 0);
	}
	if (spacestate) {
		keybd_event(VK_SPACE, MapVirtualKeyA(VK_SPACE, 0), KEYEVENTF_KEYUP, 0);
	}

	if (opt.dodelay == true) {
		std::cout << "Maximum delay: " << maxdelay << std::endl;
		std::cout << "Averege delay: " << (double)avrgdelay / avrgcnt << std::endl;
		std::cout << "Tempo = " << tempo << std::endl;
	}
	std::cout << "Press any key to continue." << std::endl;
	std::cin.get();
	return;
}
///Turns a buffer of events to key presses at diffrent times