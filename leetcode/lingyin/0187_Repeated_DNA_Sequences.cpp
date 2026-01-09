// https://leetcode.com/problems/repeated-dna-sequences/description/
// https://zxi.mytechroad.com/blog/hashtable/leetcode-187-repeated-dna-sequences/

/* BITMASK APPROACH EXPLANATION:
 * Instead of storing 10-character strings directly, we encode each DNA sequence as an integer.
 * Each nucleotide is represented by 2 bits:
 *   A = 00 (0), C = 01 (1), G = 10 (2), T = 11 (3)
 * A 10-letter sequence requires 10 * 2 = 20 bits total.
 * 
 * BENEFITS:
 * - Memory efficient: int (4 bytes) vs string (10+ bytes)
 * - Faster hashing: int comparison is faster than string comparison
 * - Elegant sliding window using bit shifts
 */
class Solution {
public:
  vector<string> findRepeatedDnaSequences(string s) {
    constexpr int kLen = 10;  // Length of DNA sequence we're looking for
    
    // Create a 20-bit mask (10 chars * 2 bits each)
    // (1 << 20) = 100000000000000000000 (binary)
    // Minus 1 = 011111111111111111111 (20 ones)
    // This mask keeps only the rightmost 20 bits when ANDed
    constexpr int mask = (1 << (2 * kLen)) - 1;  // 0xFFFFF in hex
    
    const int n = s.length();
    
    // Hash map: encoded sequence (int) -> count of occurrences
    unordered_map<int, int> m;
 
    // Mapping array: character -> 2-bit encoding
    // Using array for O(1) lookup (faster than map/switch)
    array<int, 128> km;  // 128 for ASCII range
    km['A'] = 0;  // 00 in binary
    km['C'] = 1;  // 01 in binary
    km['G'] = 2;  // 10 in binary
    km['T'] = 3;  // 11 in binary
    
    vector<string> ans;
    
    // Sliding window with rolling hash
    for(int i = 0, key = 0; i < n; ++i) {
      // ROLLING HASH MAGIC:
      // 1. (key << 2): Shift existing bits left by 2 to make room for new char
      //    Example: if key = ACGT (00011011), after << 2: 0001101100
      // 2. & mask: Keep only rightmost 20 bits (remove leftmost char in 10-char window)
      //    This automatically "forgets" the oldest character
      // 3. | km[s[i]]: Add the new character's 2-bit encoding to the right
      //    Example: if s[i] = 'A' (00), result: 0001101100 | 00 = 0001101100
      key = ((key << 2) & mask) | km[s[i]];
      
      // Skip until we have at least 10 characters
      if (i < kLen - 1) continue;
      
      // Increment count for this sequence
      // If this is the SECOND occurrence (count becomes 2), add to result
      // We check == 2 (not >= 2) to avoid adding duplicates
      if (++m[key] == 2)
        ans.push_back(s.substr(i - kLen + 1, kLen));
    }
    return ans;
  }
};

/* EXAMPLE WALKTHROUGH with s = "AAACCCAAAC":
 * i=0, s[0]='A': key = 00
 * i=1, s[1]='A': key = (00 << 2) | 00 = 0000
 * i=2, s[2]='A': key = (0000 << 2) | 00 = 000000
 * ...
 * i=9, s[9]='C': key = AAACCCAAAC encoded (first 10-char window complete)
 * i=10: Would process next 10-char window, mask removes leftmost 'A'
 */