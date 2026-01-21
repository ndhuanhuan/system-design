// 65. Valid Number
// https://leetcode.com/problems/valid-number/
// Author: Huahua

/*
PROBLEM-SOLVING MIND MAP:

1. VALID NUMBER FORMATS:
   ✓ Integer: "123", "-456", "+789"
   ✓ Decimal: "1.5", ".5", "5.", "-1.5"
   ✓ Scientific: "1e10", "1.5e-10", ".5e10", "5.e10"
   ✗ Invalid: ".", "e", "1e", "1.2.3", "1e2e3", "+-1", "e10"

2. APPROACH: Single Pass with State Tracking
   - Track what we've seen: digits, dot, exponent
   - Validate rules as we scan each character
   
3. TRACKING VARIABLES:
   bool digit  → Have we seen at least one digit? (MUST be true at end)
   bool dot    → Have we seen a decimal point?
   bool e      → Have we seen an exponent 'e'?

4. VALIDATION RULES (in order of checking):
   a) DIGIT: Always valid, mark digit = true
   
   b) DOT '.':
      - Can't have dot after 'e' (e.g., "1e2.3" invalid)
      - Can't have two dots (e.g., "1.2.3" invalid)
      
   c) EXPONENT 'e'/'E':
      - Must have digit before 'e' (e.g., "e10" invalid)
      - Can't have two 'e's (e.g., "1e2e3" invalid)
      - Reset digit flag (need digit after 'e')
      
   d) SIGN '+'/'-':
      - Can only appear at START or RIGHT AFTER 'e'
      - Examples: "+1", "1e-10" valid; "1+2", "1e10+2" invalid
      
   e) ANYTHING ELSE: Invalid character

5. DON'T FORGET:
   - Trim leading/trailing spaces first
   - Convert to lowercase for 'e'/'E' handling
   - Return digit flag (must have at least one digit)

INTERVIEW TIP: Mention that a DFA (Deterministic Finite Automaton) is the 
formal way to solve this, but the boolean flag approach is more intuitive.
*/

class Solution {
public:
  bool isNumber(string s) {
    // Remove leading spaces
    for (int i = 0; i < s.length(); ++i) {
      if (s[i] != ' ') {
        s = s.substr(i); // Trim the string from the first non-space character
        break;
      }
    }

    // Remove trailing spaces
    while (!s.empty() && s.back() == ' ') {
      s.pop_back(); // Remove spaces from the end of the string
    }

    bool dot = false;
    bool e = false;
    bool digit = false;
    for (int i = 0; i < s.length(); ++i) {
      s[i] = tolower(s[i]);
      if (isdigit(s[i])) {
        digit = true;
      } else if (s[i] == '.') {
        if (e || dot) return false;
        dot = true;
      } else if (s[i] == 'e') {
        if (e || !digit) return false;
        e = true;
        digit = false;
      } else if (s[i] == '-' || s[i] == '+') {
        if (i != 0 && s[i - 1] != 'e')
          return false;
      } else {
        return false;
      }
    }
    return digit;
  }
};