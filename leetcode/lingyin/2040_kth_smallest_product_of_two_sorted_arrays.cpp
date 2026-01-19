// https://leetcode.com/problems/kth-smallest-product-of-two-sorted-arrays/description/
// https://leetcode.cn/problems/kth-smallest-product-of-two-sorted-arrays/solutions/3698504/liang-ge-you-xu-shu-zu-de-di-k-xiao-chen-5qt9/

// Binary search on answer: search for kth smallest product in range [-10^10, 10^10]
// For each candidate value v, count how many products <= v using binary search

class Solution {
public:
    // Count how many products (x1 * nums2[j]) are <= v
    // When x1 >= 0: products are non-decreasing (normal binary search)
    // When x1 < 0: products are non-increasing (reversed binary search)
    int f(vector<int> &nums2, long long x1, long long v) {
        int n2 = nums2.size();
        int left = 0, right = n2 - 1;
        
        // Binary search for the cutoff point
        while (left <= right) {
            int mid = (left + right) / 2;
            // If x1 >= 0: find largest index where x1*nums2[mid] <= v
            // If x1 < 0: find largest index where x1*nums2[mid] > v (products decrease)
            if (x1 >= 0 && nums2[mid] * x1 <= v || x1 < 0 && nums2[mid] * x1 > v) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        // Return count of products <= v
        if (x1 >= 0) {
            return left;  // Products nums2[0..left-1] * x1 are all <= v
        } else {
            return n2 - left;  // Products nums2[left..n2-1] * x1 are all <= v
        }
    }

    long long kthSmallestProduct(vector<int>& nums1, vector<int>& nums2, long long k) {
        int n1 = nums1.size();
        // Binary search on the product value range
        long long left = -1e10, right = 1e10;
        
        while (left <= right) {
            long long mid = (left + right) / 2;
            
            // Count how many products are <= mid
            long long count = 0;
            for (int i = 0; i < n1; i++) {
                count += f(nums2, nums1[i], mid);
            }
            
            // Adjust search range based on count
            if (count < k) {
                left = mid + 1;  // Need larger products
            } else {
                right = mid - 1;  // Have enough, try smaller
            }
        }
        return left;
    }
};

