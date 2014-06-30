/*
C - Cheering Game

Problem id: cheergame
Time limit: 2 seconds
Memory limit: 512 Mb

One of the perhaps most important factors in deciding the winning team of a football match is the amount of cheers the teams get from their supporters. The Sportify football team is about to meet their worst opponent, Spoilify. As the manager of Sportify's cheerleading squad, you have to decide on a cheerleading tactic which will ensure you the win.

At your disposal you have n cheerleaders. Each cheerleader has energy to cheer for a total of t minutes of the 90 minute long game (the cheerleaders do not recover energy during the short half-time intermission). For each cheerleader you have to decide when in the game they should cheer. The cheerleading schedule for a match is divided into one minute intervals, and during each interval each cheerleader either cheers or does not cheer during the entire interval.

If a team gets more cheers than the other team for 5 consecutive minutes of the game, that team will score a goal. The 5 minute count is then reset, so if a team gets more cheers than the other team for, say, 17 consecutive minutes, that team will score three goals. The 5 minute interval can not run over the half time intermission, e.g., getting more cheers than the other team from the 43'rd minute to the 49'th minute is not sufficient to score a goal.

As an additional ace up your sleeve, you have spied on the Spoilify cheerleading team and know what their strategy for the game is. Based on this information, what is the best result that can be obtained?

Input

The first line of input consists of three integers 0 ≤ n ≤ 20, 1 ≤ t ≤ 90 and 0 ≤ m ≤ 1000. The meanings of n and t are described above, and m gives the number of time intervals that the Spoilify cheearleading team is active. Next follow m lines, each consisting of two integers a and b, 0 ≤ a < b ≤ 90, indicating that a cheerleader from Spoilify is cheering from time a to time b of the match. These intervals may overlap or even be equal, meaning that several cheerleaders are active simultaneously.

Output

Output two integers A and B where A is the number of goals scored by the Sportify team and B is the number of goals scored by the Spoilify team, assuming you use an optimal cheerleading tactic. If there are many possible results, you should first attempt to maximize the score difference A-B, and if there are still many possible results, maximize your score A.

Sample Input 1

1 31 2
20 60
50 90
Sample Output 1

4 3
Sample Input 2

2 5 3
0 90
0 90
3 90
Sample Output 2

0 17
*/

// Important Discovery: The contest isn't fair! I.e., Spoilify can have more
// cheering resources than Sportify.

#include <iostream>
#include <vector>
#include <utility>
using namespace std;

int main()
{
	int n, t, m;
	vector<pair<int, int>> vpi;
	cin >> n >> t >> m;
	for (int i = 0; i < m; i++) {
		// Get Spoilify's m time intervals
		int a, b;
		cin >> a >> b;
		vpi.push_back(pair<int, int>(a, b));
	}
	for (auto pr : vpi) {
		cout << pr.first << " to " << pr.second << endl;
	}

}
