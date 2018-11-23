#include "Timer.h"


void Timer::click(){
	gettimeofday(&tv, NULL);
	begin = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
}

void Timer::clock(){
	gettimeofday(&tv, NULL);
	end = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

double Timer::interval_ms(){
	return end-begin;
}

double Timer::interval_s(){
	return (end-begin)/1000;
}

