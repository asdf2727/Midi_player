#include <vector>
#include <string>
#include <algorithm>

#include "All.h"


int bintoint(std::vector <unsigned char> info, int *pos, int siz) {
	int nrf = 0;
	while (siz) {
		nrf = nrf * 256 + info[*pos];
		*pos += 1;
		siz--;
	}
	return nrf;
}
int readdyn(std::vector <unsigned char> info, int *pos) {
	int nrf = 0;
	while (1) {
		nrf = nrf * 128 + info[*pos] % 128;
		if (!(info[*pos] / 128)) {
			break;
		}
		*pos += 1;
	}
	*pos += 1;
	return nrf;
}
///Some functions to make file reading easier

int noignored = 0;
int nounknown = 0;

double tempo = -1;

std::string trackname;
std::string instrname;

//-3 = fatal error, -2 = minor error, -1 = no event, 0 = EOT, 1 = Note off, 2 = Note on, 3 = Sustain, 4 = Sostenuendo, 5 = Soft
event ReadEvent(std::vector <unsigned char> info, int *pos) {
	int evnt, len;
	static int last = -1;

	event eventf;
	eventf.time = 0;
	eventf.type = -1;
	eventf.val1 = 0;
	eventf.val2 = 0;

	evnt = info[*pos];
	*pos += 1;
	if (evnt < 128) {
		if (last >= 0) {
			evnt = last;
			*pos -= 1;
		}
		else {
			std::cout << "Invalid running event.";
			eventf.type = -3;
		}
	}

	if (128 <= evnt && evnt <= 239) {

		if (evnt / 16 == 8) {
			///Note off
			eventf.type = 1;
			eventf.val1 = info[*pos];
			*pos += 1;
			eventf.val2 = info[*pos];
			*pos += 1;
		}

		if (evnt / 16 == 9) {
			///Note on
			eventf.type = 2;
			eventf.val1 = info[*pos];
			*pos += 1;
			eventf.val2 = info[*pos];
			*pos += 1;
			if (eventf.val2 == 0) {
				eventf.type = -1;
			}
		}

		if (evnt / 16 == 10) {
			*pos += 2;
			std::cout << "Implement Polyphonic Pressure." << std::endl;
			noignored++;
		}

		if (evnt / 16 == 11) {
			if (info[*pos] == 64) {
				///Sustain
				*pos += 1;
				eventf.type = 3;
				if (info[*pos] < 64) {
					eventf.val1 = 0;
				}
				else {
					eventf.val1 = 1;
				}
				*pos += 1;
				eventf.val2 = 0;
			}
			else if (info[*pos] == 66) {
				///Sostenuendo
				*pos += 1;
				eventf.type = 4;
				if (info[*pos] < 64) {
					eventf.val1 = 0;
				}
				else {
					eventf.val1 = 1;
				}
				*pos += 1;
				eventf.val2 = 0;
			}
			else if (info[*pos] == 67) {
				///Soft
				*pos += 1;
				eventf.type = 5;
				if (info[*pos] < 64) {
					eventf.val1 = 0;
				}
				else {
					eventf.val1 = 1;
				}
				*pos += 1;
				eventf.val2 = 0;
			}
			else {
				std::cout << "Implement controller " << (int)info[*pos] << " " << (int)info[*pos + 1] << "." << std::endl;
				*pos += 2;
			}
		}

		if (evnt / 16 == 12) {
			*pos += 1;
			std::cout << "Implement program change." << std::endl;
			noignored++;
		}

		if (evnt / 16 == 13) {
			*pos += 1;
			std::cout << "Implement channel presure." << std::endl;
			noignored++;
		}

		if (evnt / 16 == 14) {
			*pos += 2;
			std::cout << "Implement pitch bend." << std::endl;
			noignored++;
		}

		last = evnt;
	}

	if (evnt == 240 || evnt == 247) {
		if (evnt == 240) {
			len = readdyn(info, pos);
			*pos += len;
			std::cout << "Implement SysEx tracks." << std::endl;
			nounknown++;
		}

		if (evnt == 247) {
			len = readdyn(info, pos);
			*pos += len;
			std::cout << "Implement SysEx tracks." << std::endl;
			nounknown++;
		}

		last = -1;
	}

	if (evnt == 255) {

		if (info[*pos] == 0 && info[*pos + 1] == 2) {
			*pos += 2;
			*pos += 2;
			std::cout << "Implement sequence number." << std::endl;
			nounknown++;
		}

		else if (info[*pos] == 1) {
			*pos += 1;
			len = readdyn(info, pos);
			for (int index = 0; index < len; index++) {
				std::cout << (char)info[*pos];
				*pos += 1;
			}
			std::cout << std::endl;
		}

		else if (info[*pos] == 2) {
			*pos += 1;
			len = readdyn(info, pos);
			std::cout << "Copyright: ";
			for (int index = 0; index < len; index++) {
				std::cout << (char)info[*pos];
				*pos += 1;
			}
			std::cout << std::endl;
		}

		else if (info[*pos] == 3) {
			*pos += 1;
			len = readdyn(info, pos);
			for (int index = 0; index < len; index++) {
				trackname += (char)info[*pos];
				*pos += 1;
			}
		}

		else if (info[*pos] == 4) {
			*pos += 1;
			len = readdyn(info, pos);
			for (int index = 0; index < len; index++) {
				instrname += (char)info[*pos];
				*pos += 1;
			}
		}

		else if (info[*pos] == 5) {
			*pos += 1;
			len = readdyn(info, pos);
			*pos += len;
			std::cout << "Implement lyric." << std::endl;
			noignored++;
		}

		else if (info[*pos] == 6) {
			*pos += 1;
			len = readdyn(info, pos);
			*pos += len;
			std::cout << "Implement marker." << std::endl;
			nounknown++;
		}

		else if (info[*pos] == 7) {
			*pos += 1;
			len = readdyn(info, pos);
			*pos += len;
			std::cout << "Implement cue point." << std::endl;
			noignored++;
		}

		else if (info[*pos] == 8) {
			*pos += 1;
			len = readdyn(info, pos);
			*pos += len;
			std::cout << "Implement program name." << std::endl;
			noignored++;
		}

		else if (info[*pos] == 9) {
			*pos += 1;
			len = readdyn(info, pos);
			*pos += len;
			std::cout << "Implement device name." << std::endl;
			noignored++;
		}

		else if (info[*pos] == 32 && info[*pos + 1] == 1) {
			*pos += 2;
			*pos += 1;
			std::cout << "Implement channel prefix." << std::endl;
			nounknown++;
		}

		else if (info[*pos] == 33 && info[*pos + 1] == 1) {
			*pos += 2;
			*pos += 1;
			std::cout << "Implement midi port." << std::endl;
			nounknown++;
		}

		else if (info[*pos] == 47 && info[*pos + 1] == 0) {
			*pos += 2;
			eventf.type = 0;
		}

		else if (info[*pos] == 81 && info[*pos + 1] == 3) {
			*pos += 2;
			tempo = bintoint(info, pos, 3);
		}

		else if (info[*pos] == 84 && info[*pos + 1] == 5) {
			*pos += 2;
			*pos += 5;
			std::cout << "Implement SMPTE Offset." << std::endl;
			nounknown++;
		}

		else if (info[*pos] == 88 && info[*pos + 1] == 4) {
			*pos += 2;
			*pos += 4;
			std::cout << "Implement time signature." << std::endl;
			noignored++;
		}

		else if (info[*pos] == 89 && info[*pos + 1] == 2) {
			*pos += 2;
			*pos += 2;
			std::cout << "Implement key signature." << std::endl;
			noignored++;
		}

		else if (info[*pos] == 127) {
			*pos += 1;
			len = readdyn(info, pos);
			*pos += len;
			std::cout << "Implement sequencer specific tracks." << std::endl;
			nounknown++;
		}

		else {
			std::cout << "Unrecognised meta event. Attempting recovery..." << std::endl;
			while (info[*pos] < 128) {
				*pos += 1;
			}
			//Jumping to next supposed event
			*pos -= 1;
			//Going back 1 byte and pray to god that its enough for deltat
			std::cout << "Recovery attempted. Integrity of read data compromised." << std::endl;
			eventf.type = -2;
		}

		last = -1;
	}
	return eventf;
}
///Returnes an event from a string 

int format, ntracks, tickdiv;

std::vector <event> ReadMidi(std::vector <unsigned char> info, options opt) {
	int len, pos = 0;
	/// Header chunk
	if (info[pos] == 'M' && info[pos + 1] == 'T' && info[pos + 2] == 'h' && info[pos + 3] == 'd') {
		pos += 4;
		len = bintoint(info, &pos, 4);
		if (len == 6) {
			format = bintoint(info, &pos, 2);
			ntracks = bintoint(info, &pos, 2);
			tickdiv = bintoint(info, &pos, 2);
			if (tickdiv >> 15 == 1) {
				std::cout << "Date and time format not implemented. Unalble to read file." << std::endl;
				exit(EXIT_FAILURE);
			}
			std::cout << "format = " << format << std::endl;
			std::cout << "ntracks = " << ntracks << std::endl;
			std::cout << "tickdiv = " << tickdiv << std::endl;
			std::cout << std::endl;
		}
		else {
			std::cout << "Header chunk length diffrent from 6. Unable to read file." << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	else {
		std::cout << "Invalid chunk format. Unable to read file." << std::endl;
		exit(EXIT_FAILURE);
	}

	int posm, deltat, trckcnt = 0, time = 0;
	std::vector <event> events;
	std::vector <event> action;
	int noignored;
	int nounknown;
	int voice;
	event eventf;
	while ((int)info.size() > pos) {
		if (info[pos] == 'M' && info[pos + 1] == 'T' && info[pos + 2] == 'r' && info[pos + 3] == 'k') {
			pos += 4;
			posm = bintoint(info, &pos, 4);
			posm += pos;
			noignored = 0;
			nounknown = 0;
			trackname = "";
			instrname = "";
			action.clear();
			if (format == 0 || format == 1) {
				time = 0;
			}
			std::cout << std::endl << "Track " << trckcnt << " notifications: " << std::endl << std::endl;
			while (1) {
				deltat = readdyn(info, &pos);
				time += deltat;
				eventf = ReadEvent(info, &pos);
				eventf.time = time;
				if (eventf.type == -3) {
					std::cout << "A fatal error was detected reading an event. Unable to read file." << std::endl;
					exit(EXIT_FAILURE);
				}
				if (eventf.type == -2) {
					std::cout << "An error was detected reading an event. Continuing to read file..." << std::endl;
					//Add error handling
				}
				if (eventf.type == 0) {
					if (pos != posm) {
						std::cout << "A faulted end track event was detected. Integrity of read data compromised. Continuing to read file..." << std::endl;
						//Add error handling
					}
					else {
						break;
					}
				}
				if (eventf.type > 0) {
					action.push_back(eventf);
				}
				if (eventf.type == 2 && opt.shortnotes != -1) {
					eventf.time += opt.shortnotes;
					eventf.type = 1;
					action.push_back(eventf);
					eventf.time -= opt.shortnotes;
				}
			}
			std::cout << std::endl;
			if (!action.empty()) {
				if (trackname != "") {
					std::cout << "	Track name: " << trackname << std::endl;
				}
				else {
					std::cout << "	Track name not defined" << std::endl;
				}
				if (instrname != "") {
					std::cout << "	Instrument name: " << instrname << std::endl;
				}
				else {
					std::cout << "	Instrument name not defined" << std::endl;
				}
				if (!action.empty()) {
					std::cout << "	Starting time: " << action[0].time << std::endl;
				}
				std::cout << "	No of significant events: " << action.size() << std::endl;
				std::cout << "	No of ignored events: " << noignored << std::endl;
				std::cout << "	No of unknown events: " << nounknown << std::endl;
				std::cout << "Do you want to include this track(1) or not(0)?" << std::endl;
				std::cin >> voice;
				if (voice != 0) {
					for (int index = 0; index < (int)action.size(); index++) {
						events.push_back(action[index]);
					}
				}
			}
			else {
				std::cout << "No significant events found. Skipping this track..." << std::endl;
			}
			trckcnt++;
		}
		else {
			std::cout << "Invalid chunk format. Unable to read file." << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	std::sort(events.begin(), events.end(), KeySort);

	tempo /= tickdiv;
	tempo /= 1000;
	double newtempo;
	if (tempo < 0) {
		std::cout << "Tempo undefined. Please insert tempo: ";
		std::cin >> newtempo;
		tempo = 60 / newtempo;
	}
	else {
		std::cout << "Tempo value is " << (double) 60 / tempo << " bpm. Please introduce a new value if you want it changed: ";
		std::cin >> newtempo;
		if (newtempo > 0 && newtempo != tempo) {
			tempo = 60 / newtempo;
		}
	}
	
	return events;
}
///Returns the events of a full midi