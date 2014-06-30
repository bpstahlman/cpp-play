/*
B - A List Game

Problem id: listgame
Time limit: 2 seconds
Memory limit: 512 Mb


You are playing the following simple game with a friend:

The first player picks a positive integer X.
The second player gives a list of k positive integers Y1, ..., Yk such that (Y1+1)·(Y2+1)·...·(Yk+1) = X, and gets k points.
Write a program that plays the second player.

Input

The input consists of a single integer X satisfying 10^3 â‰¤ X â‰¤ 10^9, giving the number picked by the first player.

Output

Write a single integer k, giving the number of points obtained by the second player, assuming she plays as good as possible.
Sample Input 1

65536
Sample Output 1

16
Sample Input 2

127381
Sample Output 2

3
*/

// DONE: Correctly on 26Jun2014

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Return ordered (increasing) list of prime factors
void primes(unsigned int n, vector<unsigned int>& vi)
{
	int max = static_cast<unsigned int>(sqrt(n));
	for (int i = 2; i <= max; i++) {
		if (n % i == 0) {
			vi.push_back(i);
			primes(n / i, vi);
			return;
		}
	}
	// Can't factor input any further.
	// Assumption: Constraint i<=max ensures input n will never fall below 2
	vi.push_back(n);
}

int main()
{
	unsigned int x;
	cin >> x;
	vector<unsigned int> ps {};
	primes(x, ps);
	cout << "K = " << ps.size() << endl;
	cout << "Primes: ";
	for (auto i : ps) {
		cout << i << " " << endl;
	}
	return 0;
}
