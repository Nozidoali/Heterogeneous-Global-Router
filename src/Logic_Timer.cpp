#include "Logic_Timer.h"

chrono::time_point<chrono::steady_clock>  record;


void Tmr_Start() {
    record = steady_clock::now();
}

string Tmr_ToString() {
    auto elap = steady_clock::now() - record;
    int minutes = chrono::duration_cast<chrono::minutes>(elap).count();
	int seconds = int(chrono::duration_cast<chrono::seconds>(elap).count()) % 60;
    ostringstream ss;
    ss << minutes << "min " << seconds << "sec";
    return ss.str();
}