#ifndef TIMER_H
#define TIMER_H
#include <ctime>

class Timer {
public:
	Timer();
	void start_timer();
	static int get_start_time();
	int elapsed_time();
	int get_end_time();
	int now();


private:
	static time_t start_t;
	time_t end_t;



};

#endif
