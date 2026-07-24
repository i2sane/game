#pragma once

#include <stdbool.h>

typedef double Timer;

Timer createTimer(double timeToExpire);
bool isTimerExpired(Timer t);
void invalidateTimer(Timer *t); 
bool isTimerInvalid(Timer t);
