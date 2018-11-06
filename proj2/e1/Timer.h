#ifndef TIMER_H
#define TIMER_H

#include <time.h>

class Timer {
   //private:
      struct timeval tv;
      double begin;
      double end;

	public:
			void click();
			void clock();
			double interval_ms();
			double interval_s();
};

#endif
