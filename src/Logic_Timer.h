#pragma once

#include <chrono>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <string>
using std::chrono::steady_clock;
using namespace std;

typedef std::chrono::high_resolution_clock Clock;

void Tmr_Start();
string Tmr_ToString();
int Tmr_TimeLeft();