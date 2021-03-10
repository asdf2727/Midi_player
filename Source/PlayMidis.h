#pragma once

#include <vector>

#include "All.h"

bool KeySort(event num1, event num2);

options GetOptions();

double PlayEvent(event toplay, double globaltime, options opt);

void PlayKeys(std::vector <event> events, options opt);