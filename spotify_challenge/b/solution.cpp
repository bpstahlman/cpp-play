
/* This is an example solution to the "Cheering Game" problem from the
 * Spotify Programming Challenge, held on May 29, 2010.
 *
 * The solution is provided as-is, is not documented, and may be
 * generally difficult to read.
 *
 * This work by Scrool (http://scrool.se/) is licensed under a
 * Creative Commons Attribution-Share Alike 2.5 Sweden License
 * (http://creativecommons.org/licenses/by-sa/2.5/se/deed.en).  You
 * are free to redistribute it as you like as long as this license
 * information is not removed.
 */
#include <cassert>
#include <algorithm>
#include <cstring>
#include <cstdio>

using namespace std;

typedef pair<int,int> pii;

int n, t, m;
// Initialization: Represents opponent cheer units alloted by minute, with index 0 corresponding to 90th minute.
int bound[100];

// dim 1: complement of current time (i.e. t=90 is start of game)
// dim 2: cheer units remaining at time t.
// dim 3: current d value with center shifted from 0 to 4
//        -4 <= d <= 4 with 5 being point at which we score, and -5 the point at which they score
//        Note: d is forced to 0 at halftime
// Initialization: Memset initialized all bytes in score to FF, so a negative score means unscored.
pii score[100][2000][10];

// Compares 2 pairs representing scores, returning true if the first is better than the second.
bool better(pii &S1, pii &S2) {
  int d = (S1.first-S1.second)-(S2.first-S2.second);
  if (d) return d > 0;
  return S1.first > S2.first;
}

pii Score(int t, int e, int d) {
  // t==0 is 90th minute, but due to the recursive, depth-first nature of the returns from Score, we're essentially
  // working backwards from the end, so there's never any score at minute 90.
  if (!t) return pii(0,0);
  if (t == 45) d = 0;
  assert(0 <= e && e <= 1800);
  assert(-4 <= d && d <= 4);
  // Initilize score before loop that looks for better outcomes.
  // Shift d to center at 4, thereby avoiding negative indices.
  pii &r = score[t][e][d+4];
  // Do we have a score for this permutation yet?
  // Explanation: Due to the recursive nature of the calls to Score, the permutations are not unique: consider that
  // cheer units could be expended in different ways such that units remaining at a given point in time are still equal.
  // Thus, this test is a form of result caching.
  if (r.first < 0) {
    // This loop tries every allotment of cheer units possible for current time; i represents units for current try.
    // For each try, we call Score recursively for the subsequent minute, saving return in next for comparison with
    // current best outcome, updating current best if appropriate.
    // e is cheer units remaining; n is # of cheerleaders.
    // Explanation: A single cheerleader can't use more than one of his cheer units per minute.
    for (int i = 0; i <= min(e, n); ++i) {
      // When dd reaches 5, we score; when dd reaches -5, they score.
      // Note: When a team wins consecutive minutes, dd incs/decs continuously by 1; when a new team wins, however, dd
      // jumps to -1 or 1, nullifying up to 4 wins of the team that had been winning.
      int dd = 0;
      if (i < bound[t]) dd = min(d, 0)-1;
      if (i > bound[t]) dd = max(d, 0)+1;
      // Recursive self call returns the best outcome at the subsequent minute, given the current try. Note that for
      // each try, cheer units allotted for this minute and all earlier minutes has been fixed. We then update the
      // returned score if a goal would be scored in this minute.
      // The dd % 5 resets the counter after a goal
      pii next = Score(t-1, e-i, dd % 5);
      // Counter's been effectively reset after score, but update the pairs to reflect any goal scored.
      next.first += dd == 5;
      next.second += dd == -5;
      if (!i || better(next, r)) r = next;
    }
  }
  // Return the pair representing best possible outcome for this t.
  cout << "@t=" << t << " score: " << r.first << " - " << r.second << endl;
  return r;
}

int main(void) {
  scanf("%d%d%d", &n, &t, &m);
  memset(bound, 0, sizeof(bound));
  memset(score, -1, sizeof(score));
  assert(0 <= n && n <= 20);
  assert(1 <= t && t <= 90);
  assert(0 <= m && m <= 1000);
  for (int i = 0; i < m; ++i) {
    int a, b;
    assert(scanf("%d%d", &a, &b) == 2);
    assert(0 <= a && a < b && b <= 90);
    for (int x = a; x < b; ++x)
      ++bound[90-x];
  }
  pii r = Score(90, n*t, 0);
  printf("%d %d\n", r.first, r.second);
  return 0;
}

// ts=2:sw=2:et:tw=120
