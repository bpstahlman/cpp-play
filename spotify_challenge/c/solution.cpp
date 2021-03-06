
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
#include <stack>
#include <list>
#include <functional>
#include <unordered_map>
#include <memory>
#include <cstring>
#include <cstdio>
#include <iostream>

using namespace std;

typedef pair<int,int> pii;

struct Info {
  int units, opp_units;
  int t, e, d;
};
struct ScoreRet {
  ScoreRet(const ScoreRet& o) : scores {o.scores}, li {o.li} {}
  ScoreRet(pii scores_arg = pii(-1,-1), list<Info> li_arg = list<Info> {}) : scores {scores_arg}, li {li_arg} {}
  ScoreRet& operator=(const ScoreRet& o) {
    this->scores = o.scores;
    this->li = o.li;
    return *this;
  }
  pii scores;
  list<Info> li;
  //tuple<int,int,int> id;
};
// Struct used as hash key for ScoreRet
struct ScoreRetId {
  ScoreRetId(int t_arg, int e_arg, int d_arg) : t{t_arg}, e{e_arg}, d{d_arg} {}
  int t, e, d;
};

struct ScoreRetIdHash {
  ScoreRetIdHash() {}
  size_t operator()(const ScoreRetId& id) const {
    return hash<int>()(id.t) ^ hash<int>()(id.e) ^ hash<int>()(id.d);
  }
};
struct ScoreRetIdEq {
  ScoreRetIdEq() {}
  bool operator()(const ScoreRetId& id1, const ScoreRetId& id2) const {
    return id1.t == id2.t && id1.e == id2.e && id1.d == id2.d;
  }
};

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

// This unordered map is intended to parallel score[][][]; it's used for caching.
using srmap_type = unordered_map<ScoreRetId, ScoreRet, ScoreRetIdHash, ScoreRetIdEq>;
srmap_type smap {1000, ScoreRetIdHash{}, ScoreRetIdEq{}};

// Compares 2 pairs representing scores, returning true if the first is better than the second.
bool better(pii &S1, pii &S2) {
  int d = (S1.first-S1.second)-(S2.first-S2.second);
  if (d) return d > 0;
  return S1.first > S2.first;
}

ScoreRet Score(int t, int e, int d)
{
  // t==0 is end of 90th minute, but due to the recursive, depth-first nature of the returns from Score, we're
  // essentially working backwards from the end, so there's never any score at t==0.
  if (!t) {
    // Create a ScoreRet with an empty list.
    return ScoreRet {pii(0,0), list<Info> {}};
  }
  if (t == 45) d = 0;
  assert(0 <= e && e <= 1800);
  assert(-4 <= d && d <= 4);
  // Initilize score (from cache) before loop that looks for better outcomes: value of -1 means nothing in cache.
  // Shift input d (-4<=d<=4) to center at 4, thereby avoiding negative indices.
  pii &r = score[t][e][d+4];
  // Also check cache for appropriate ScoreRet, bearing in mind that the 2 caches should hit or miss together...
  // Explanation: There are many ways to arrive at a given t with a specific combination of t, e and d, and since those
  // 3 parameters completely determine the result of the recursion, caching the result can save a lot of time.
  auto sr_it = smap.find(ScoreRetId(t,e,d+4));
  if (sr_it != smap.end()) {
    // Make sure the 2 caches are in sync.
    assert(r.first >= 0);
    ScoreRet sr_r {sr_it->second};
    return sr_r;
  }
  // If we get here, nothing in cache. We don't have a score for this permutation yet.
  int i_r = -1;
  ScoreRet sr_r {};
  // Note: This cache test is no longer necessary because of the earlier test on the smap cache.
  //if (r.first < 0) {
    // This loop tries every allotment of cheer units possible for current time; i represents units for current try.
    // For each try, we call Score recursively for the subsequent minute, saving return in next for comparison with
    // current best outcome, updating current best if appropriate.
    // e is cheer units remaining; n is # of cheerleaders.
    // Constraint: A single cheerleader can't use more than one of his cheer units per minute: hence, the min(e, n)
    for (int i = 0; i <= min(e, n); ++i) {
      // When dd reaches 5, we score; when dd reaches -5, they score.
      // Note: When a team wins consecutive minutes, dd incs/decs continuously by 1; when a new team wins a minute,
      // however, dd jumps to -1 or 1, nullifying up to 4 wins of the team that had been winning.
      int dd = 0;
      if (i < bound[t]) dd = min(d, 0)-1;
      if (i > bound[t]) dd = max(d, 0)+1;
      // Recursive self call returns the best outcome at the subsequent minute, given the current try. Note that for
      // each try, cheer units allotted for this minute and all earlier minutes has been fixed. We then update the
      // returned score if a goal would be scored in this minute.
      // The dd % 5 resets the counter after a goal
      auto sr = Score(t-1, e-i, dd % 5);
      pii next = sr.scores;
      // If there was a score this minute, the modulo 5 in the Score argument has effectively reset the count; need to
      // update the pairs as well.
      next.first += dd == 5;
      next.second += dd == -5;
      // Is this the current best?
      if (!i || better(next, r)) {
        r = next;
        i_r = i;   // # of cheer units used
        sr_r = sr;
      }
    }
    // Assumption: sr_r is one the values returned by recursive calls to Score in loop above.
    // Rationale: We'll always get into the loop because of the <= 0 test.
    // Update the ScoreRet to reflect best outcome for this level, then cache.
    sr_r.scores = r;
    sr_r.li.push_front(Info {i_r, bound[t], t, e, d});
    // Note: std::make_pair doesn't work because I don't have rvalue references... Could probably use std::move()
    smap.insert(pair<ScoreRetId, ScoreRet>(ScoreRetId(t,e,d+4), sr_r));
  //}
  // Return the pair representing best possible outcome for this t.
  return sr_r;
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
  ScoreRet sr = Score(90, n*t, 0);

  int idx = 1;
  for (auto it = sr.li.begin(); it != sr.li.end(); ++it, idx++) {
    cout << "minute=" << idx << "\t t=" << it->t << "\t e=" << it->e << "\t d=" << it->d << "\t " << it->units << "\t " << it->opp_units << endl;
  }
  cout << "-- Final Score --" << endl;
  printf("%d %d\n", sr.scores.first, sr.scores.second);
  return 0;
}

// vim:ts=2:sw=2:et:tw=120
