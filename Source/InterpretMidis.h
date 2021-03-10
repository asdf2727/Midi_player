#pragma once

#include <vector>

#include "All.h"

event ReadEvent(std::vector <unsigned char> info, int *pos);

std::vector <event> ReadMidi(std::vector <unsigned char> info, options opt);