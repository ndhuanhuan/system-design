// https://leetcode.com/problems/word-ladder/description/
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> dict(wordList.begin(), wordList.end());
        
        if (!dict.count(endWord)) return 0;
        const int l = beginWord.size();

        unordered_set<string> q1({beginWord});
        unordered_set<string> q2({endWord});

        int step = 0;

        while(!q1.empty() && !q2.empty()) {
            step++;

            if (q1.size() > q2.size()) {
                swap(q1, q2);
            }

            unordered_set<string> q;

            for(string w: q1) {
                for (int i = 0; i < l; ++i) {
                    char cur = w[i];
                    for (int j = 'a'; j <= 'z'; ++j) {
                        w[i] = j;
                        if (q2.count(w)) return step+1;
                        if (!dict.count(w)) continue;
                        q.insert(w);
                        dict.erase(w);
                    }

                    w[i] = cur;
                }
            }

            swap(q1, q);
        }

        return 0;
    }
};