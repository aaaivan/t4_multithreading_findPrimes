#include "ThreadsManager.h"
#include "Functions.h"
#include <thread>

std::vector<Thread> ThreadsManager::threads;
std::vector<int> ThreadsManager::primes;
std::vector<int> ThreadsManager::upperBounds;
int ThreadsManager::biggestPrime=0;

ThreadsManager::ThreadsManager(int _totThreads, int _num) {
	const int subdivisionSize = _num / _totThreads;
	const int reminder = _num % _totThreads;
	for (int i = 0, min = 0, max = 0; i < _totThreads; i++) {
		min = max + 1;
		//the first threads will compute one additional square
		//to account for the reminder
		if (i < reminder)
			max = min + subdivisionSize;
		else
			max = min + subdivisionSize - 1;
		upperBounds.push_back(max);
	}
	threads.push_back(Thread(1, upperBounds[0], primes.begin()));
}

void ThreadsManager::addPrime(int prime, int index) {
	primes.push_back(prime);
}

void ThreadsManager::endOfThread(int index) {

}


