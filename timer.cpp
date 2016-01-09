#include "timer.h"

time_t Timer::start_t = 0;

Timer::Timer(){
}

void Timer::start_timer() {
	time(&start_t);
}

int Timer::get_start_time() {
	return start_t;
}

int Timer::elapsed_time() {
	int elapsed_t = time(&end_t) - start_t;
	return elapsed_t;
}

int Timer::get_end_time() {
	return end_t;
}

int Timer::now() {
	return time(&end_t);
}
