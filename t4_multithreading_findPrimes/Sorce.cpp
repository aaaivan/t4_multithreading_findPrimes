#include <vector>
#include <list>
#include <thread>
#include <mutex>
#include <iostream>

std::mutex pushPrimeMutex;

void findPrimes(int min, int max, std::list<int>* primes, std::list<int>* temp) {
	for (int i = min; i <= max; i++) {
		bool prime = true;
		for (std::list<int>::iterator it = primes->begin(); (*it) * (*it) <= i; it++) {
			if (i % (*it) == 0) {
				//one of the primes already found devides i
				//so i is NOT prime
				prime = false;
				break;
			}
		}
		if (prime)
		{
			pushPrimeMutex.lock();
			temp->push_back(i);
			pushPrimeMutex.unlock();
		}
	}
}

int main()
{
	std::list<int> primes={};//vector storing all the primes

	//get n from user
	std::cout << "Find all primes equal to or less that n.\nEnter n: ";
	int n = 0;
	std::cin >> n;
	//take care of cases that would break the code
	if (n < 0)
		n = 0;
	if (n < 2) {
		std::cout << "\nThere are no primes less that or equal to " << n << ".";
		return 0;
	}
	else if (n == 2) {
		std::cout << "\nThe primes less then or equal to " << n << " are:\n2";
		return 0;
	}
	else {//the first prime is added manualli to the primes list
		//hence only the number in the interval [3, n] need to be checked
		primes.push_back(2);
	}

	//get number of threads from user
	std::cout << "\nHow many threads? ";
	int numOfThreads = 0;
	std::cin >> numOfThreads;
	//take care of cases that would break the code
	if (numOfThreads >= n - 1) {
		//there can be at most as many threads as
		//there are numbers in the interval [3, n]
		numOfThreads = n - 2;
	}
	else if (numOfThreads < 1)
		numOfThreads = 1;

	//*****************************************************************************************
	//the range [3 ,n] is divided into a number of subsets equal to numOfThreads
	std::vector<int> upperBounds;//last number of each subset
	std::vector<std::list<int>> tempLists(numOfThreads, std::list<int>());// list of primes found in each subset
	int indexNextThread = 0; //indicates the next subset that should be evaluated
	const int subsetSize = (n-2) / numOfThreads;
	const int reminder = (n-2) % numOfThreads;
	for (int i = 0, min = 2, max = 2; i < numOfThreads; i++) {
		min = max + 1;
		//the first threads will check an additional number
		//to account for the reminder
		if (i < reminder)
			max = min + subsetSize;
		else
			max = min + subsetSize - 1;
		upperBounds.push_back(max);
	}

	//start first thread: it adds the primes it finds directly to the primes list
	std::thread t1(findPrimes, 3, upperBounds[indexNextThread], &primes, &primes);
	indexNextThread++;
	t1.join();//wait for forst thread to finish
	
	//each the following threads will add the primes it finds to one of the lists in the tempLists vector.
	//when a threads ends, the corresponding list is merged to the primes list.
	while (indexNextThread < upperBounds.size()){
		int tempIndex = indexNextThread;
		std::vector<std::thread> threads = {};//store the threads running on current iteration
		for (int i = indexNextThread; i < upperBounds.size(); i++) {
			//start thread for any subsequent subsets whose upper boundary's square root
			//is no grater than the upper boundary of the last subset checked completely
			if (upperBounds[tempIndex-1] * upperBounds[tempIndex-1] >= upperBounds[i]) {
				threads.push_back(std::thread(findPrimes, upperBounds[i-1] + 1, upperBounds[i], &primes, &(tempLists[i])));
				indexNextThread++;
			}
			else
				break;
		}
		//wait for all the threads in the current iteration to finish
		for (int i = 0; i < threads.size(); i++) {
			threads[i].join();
			//merge primes found by this thread to the primes list
			primes.merge(tempLists[tempIndex+i]);
		}
	}
	//*******************************************************************************************

	//print all primes
	std::cout << "\nThe primes less then or equal to "<<n<<" are:\n";
	for (const int& p : primes) {
		std::cout << p << " ";
	}
}