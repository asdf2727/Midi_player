#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <signal.h>
#include <windows.h>

#include "All.h"

bool ReadLive(options opt) {
	bool canread;

	RtMidiIn *midiin = 0;

	std::string portName;
	int noPort = 0;

	std::vector <unsigned char> eventmessage;

	int pos;

	double delay;

	try {
		midiin = new RtMidiIn();
	}
	catch (RtMidiError &error) {
		error.printMessage();
		exit(EXIT_FAILURE);
	}

	int nPorts = midiin->getPortCount();
	if (nPorts == 0) {
		canread = false;
		std::cout << "There are no ports available. Closing program..." << std::endl;
	}
	else {
		canread = true;
		std::cout << std::endl << "There are " << nPorts << " MIDI input sources available." << std::endl;
		for (int index = 0; index < nPorts; index++) {
			try {
				portName = midiin->getPortName(index);
			}
			catch (RtMidiError& error) {
				error.printMessage();
				exit(EXIT_FAILURE);
			}
			std::cout << "	Input Port #" << index << ": " << portName << std::endl;
		}

		midiin->openPort(0);
		midiin->ignoreTypes(true, false, false);

		std::cout << "Reading MIDI from port 0..." << std::endl;
		while (1) {
			midiin->getMessage(&eventmessage);
			/*
			for (int index = 0; index < (int)eventmessage.size(); index++) {
				if (eventmessage.size() != 1 || eventmessage[0] != 254) {
					if ((int)eventmessage[index] / 16 < 10) {
						std::cout << (int)eventmessage[index] / 16;
					}
					else {
						std::cout << (char)('A' + (int)eventmessage[index] / 16 - 10);
					}
					if ((int)eventmessage[index] % 16 < 10) {
						std::cout << (int)eventmessage[index] % 16;
					}
					else {
						std::cout << (char)('A' + (int)eventmessage[index] % 16 - 10);
					}
					std::cout << " ";
				}
			}
			if (eventmessage.size() != 0 && (eventmessage.size() != 1 || eventmessage[0] != 254)) {
				std::cout << "| ";
			}
			*/
			if ((eventmessage.size() != 1 || eventmessage[0] != 254) && eventmessage.size() != 0) {
				pos = 0;
				delay = PlayEvent(ReadEvent(eventmessage, &pos), 0, opt);
				if (delay != 0) {
					std::cout << delay << std::endl;
				}
			}
		}
	}

	delete midiin;
	return canread;
}
///Reads in realtime midi events as bytes

std::vector <unsigned char> ReadFile(std::string filename) {
	std::streampos size;
	char* memblock;
	std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char[(unsigned int)size];
		file.seekg(0, std::ios::beg);
		file.read(memblock, size);
		file.close();
		std::cout << "File in memory" << std::endl << std::endl;
	}
	else {
		memblock = new char[0];
		std::cout << "Unable to open file" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::vector <unsigned char> ans;
	for (int index = 0; index < size; index++) {
		ans.push_back((unsigned char)memblock[index]);
		/*
		int val = (unsigned char)memblock[index];
		std::cout << (unsigned char)memblock[index] << " " << val << " ";
		if (val / 16 < 10) {
			std::cout << val / 16;
		}
		else {
			std::cout << (unsigned char)('A' + val / 16 - 10);
		}
		if (val % 16 < 10) {
			std::cout << val % 16;
		}
		else {
			std::cout << (unsigned char)('A' + val % 16 - 10);
		}
		std::cout << " | ";
		*/
	}
	return ans;
}
///Reads from a file midi events as bytes