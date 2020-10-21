#pragma once
#include <thread>
#include <vector>
class Thread
{
	static int counter;
	int index;
	std::thread t;
	int lowBound;
	int upBound;
	bool terminated;
public:
	Thread(int _min, int _max, std::vector<int>::iterator it);
	void terminate();
	bool isTerminated();
	int getLowerBound();
	int getUpperBound();
	void join();
};

