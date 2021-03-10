#include <iostream>
#include <string>
#include <vector>

#include "All.h"

int main()
{
	std::cout << "Welcome to Midi player V 3.1.0! Do you wish to play a file(0) or live(1)?" << std::endl;
	bool playmode;
	std::cin >> playmode;
	std::string filename, stri;
	std::vector <track> tracks;
	if (playmode == 0) {
		std::cout << "Please enter the name of the midi you wish to play: ";
		while (std::cin >> stri) {
			if (filename != "") {
				filename += " ";
			}
			filename += stri;
			if (stri.size() >= 4) {
				if (stri[stri.size() - 4] == '.' && stri[stri.size() - 3] == 'm' && stri[stri.size() - 2] == 'i' && stri[stri.size() - 1] == 'd') {
					break;
				}
			}
		}
		std::cout << "Reading from C:\\Users\\Cezar T\\Desktop\\Midis..." << std::endl;
		filename = "C:\\Users\\Cezar T\\Desktop\\Midis\\" + filename;
		PlayKeys(ReadMidi(ReadFile(filename), GetOptions()), GetOptions());
	}
	else {
		ReadLive(GetOptions());
	}
}
//La-Campanella.mid