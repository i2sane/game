#include <stdbool.h>
#include <raylib.h>
#include <stdio.h>
#include "timer.h"

Timer createTimer(double timeToExpire) {
	return GetTime() + (double)timeToExpire;
}

bool isTimerExpired(Timer t) {
	return GetTime() >= (double)t;
}

void invalidateTimer(Timer *t) {
	*t = -1.0;
}

bool isTimerInvalid(Timer t) {
	return t == -1.0;
}
