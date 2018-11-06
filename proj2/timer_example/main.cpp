#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include "Timer.h"

int main() {
	
	Timer t;

	t.click();
	sleep(2);
	t.clock();

	printf("Execution time %fs\n", t.interval_s());
	printf("Execution time %fms\n", t.interval_ms());
	return (0);
}
