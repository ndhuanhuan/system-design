// https://leetcode.com/problems/reverse-words-in-a-string/description/
// https://zxi.mytechroad.com/blog/string/leetcode-151-reverse-words-in-a-string/
class Solution {
public:
    // The algorithm is essentially building a new string in-place by moving words forward and removing extra spaces. l tracks the write position, i tracks the read position.
    // "...hello.world"
    // "hello...world"
    string reverseWords(string s) {
        reverse(s.begin(), s.end());
        //cout << s << endl;
        int l = 0;

        for (int i = 0, j = 0; i < s.length(); ++i) {
            if (s[i] == ' ') continue;
            if (l) ++l;
            j = i;
            while(j < s.length() && s[j] != ' ') ++j;
            reverse(begin(s) + l, begin(s) + j);
            //cout << s << endl;
            //cout << "I: " << i << " J: " << j << " l: " << l << endl;
            l += (j-i);
            i = j;
        }

        s.resize(l);
        return s;
    }
};

class Solution {
public:
    string reverseWords(string s) {
        reverse(s.begin(), s.end());
        int n = s.size();
        int left = 0;
        int right = 0;
        int i = 0;
        while (i < n) {
            while (i < n && s[i] == ' ')
                i++;
            if (i == n)
                break;
            while (i < n && s[i] != ' ') {
                s[right++] = s[i++];
            }
            reverse(s.begin() + left, s.begin() + right);
            s[right++] = ' ';
            left = right;
            i++;
        }
        s.resize(right - 1);
        return s;
    }
};