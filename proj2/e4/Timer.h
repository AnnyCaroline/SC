#ifndef TIMER_H
#define TIMER_H

#ifndef NULL
#define NULL 0
#endif

#include <sys/time.h>

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
