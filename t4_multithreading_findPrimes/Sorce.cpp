#include <vector>
#include <thread>
#include <mutex>
#include <iostream>

std::mutex addPrimeMutex;

void findPrimes(int min, int max, std::vector<int>* v, int size) {
	if (size == 0) {
		if (max < 2)
			return;
		else
		{
			v->push_back(2);
			min = 3;
		}
	}
	for (int i = min; i <= max; i++) {
		bool prime = true;
		for (int j=0; (v->at(j)) * (v->at(j)) <= i; j++) {
			if (i % (v->at(j)) == 0) {
				prime = false;
				break;
			}
		}
		if (prime)
		{
			addPrimeMutex.lock();
			v->push_back(i);
			addPrimeMutex.unlock();
		}
	}
}

int main()
{
	//get n from user
	std::cout << "Find all primes equal to or less that n.\nEnter n: ";
	int n = 0;
	std::cin >> n;
	if (n < 1)
		n = 1;
	//get number of threads from user
	std::cout << "\nHow many threads? ";
	int numOfThreads = 0;
	std::cin >> numOfThreads;
	if (numOfThreads > n)
		numOfThreads = n;
	else if (numOfThreads < 1)
		numOfThreads = 1;

	std::vector<int> primes;//vector stroring the primes found
	//the range [1 ,n] is divided into a number of subsets equal to numOfThreads
	std::vector<int> upperBounds;//last number of each subset
	int indexNextThread = 0; //indicates the next subset that should be evaluated
	const int subsetSize = n / numOfThreads;
	const int reminder = n % numOfThreads;
	for (int i = 0, min = 0, max = 0; i < numOfThreads; i++) {
		min = max + 1;
		//the first threads will check an additional number
		//to account for the reminder
		if (i < reminder)
			max = min + subsetSize;
		else
			max = min + subsetSize - 1;
		upperBounds.push_back(max);
	}

	//start first thread
	std::thread t1(findPrimes, 1, upperBounds[0], &primes, primes.size());
	indexNextThread++;
	t1.join();//wait for forst thread to finish

	while (indexNextThread < upperBounds.size())
	{
		int tempIndex = indexNextThread;
		std::vector<std::thread> threads;//store the threads running on current iteration
		//start checking next subset
		threads.push_back(std::thread(findPrimes, upperBounds[tempIndex-1] + 1, upperBounds[tempIndex], &primes, primes.size()));
		indexNextThread++;
		//start checking on all subsequent subsets whose upper boundary's square root
		//is smaller than or equal to the upper boundary of the last subset checked completely
		for (int i = indexNextThread; i < upperBounds.size(); i++) {
			if (upperBounds[tempIndex-1] * upperBounds[tempIndex-1] >= upperBounds[i]) {
				threads.push_back(std::thread(findPrimes, upperBounds[i-1] + 1, upperBounds[i], &primes, primes.size()));
				indexNextThread++;
			}
			else
				break;
		}
		//wait for all the threads in the current iteration to finish
		for (std::thread& th : threads) {
			th.join();
		}
	}

	std::cout << "\nThe primes less then or equal to "<<n<<" are:\n";
	for (const int& p : primes) {
		std::cout << p << " ";
	}

}