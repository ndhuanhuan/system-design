// https://leetcode.com/problems/shortest-word-distance-ii/description/
class WordDistance {
public:
    unordered_map<string, vector<int>> m;
    WordDistance(vector<string>& wordsDict) {
        for (int i = 0; i < wordsDict.size(); ++i) {
            m[wordsDict[i]].push_back(i);
        }
    }
    
    int shortest(string word1, string word2) {
        int i = 0, j = 0, res = INT_MAX;

        while (i < m[word1].size() && j < m[word2].size()) {
            res = min(res, abs(m[word1][i] - m[word2][j]));
            if (m[word1][i] > m[word2][j]) {
                j++;
            } else {
                i++;
            } 
        }

        return res;
    }
};

/**
 * Your WordDistance object will be instantiated and called as such:
 * WordDistance* obj = new WordDistance(wordsDict);
 * int param_1 = obj->shortest(word1,word2);
 */