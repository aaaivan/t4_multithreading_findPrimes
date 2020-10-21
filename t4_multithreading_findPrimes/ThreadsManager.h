#pragma once
#include "Thread.h"
#include <thread>
#include <vector>
#include <iostream>

class ThreadsManager
{
	static std::vector<Thread> threads;
	static std::vector<int> primes;
	static std::vector<int> upperBounds;
	static int biggestPrime;
	int subsetWithInt(int i);
public:
	ThreadsManager(int _totThreads, int _num);
	static void addPrime(int primeFound, int index);
	static void endOfThread(int index);
};

