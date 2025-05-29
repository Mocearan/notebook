/*
 * @lc app=leetcode.cn id=1 lang=cpp
 *
 * [1] 两数之和
 */

// @lc code=start
#include <unordered_map>

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> hashtable;
        int i = 0;
        for(const auto& num : nums) {
            auto it = hashtable.find(target - num);
            if(it not_eq hashtable.end())
                return { it->second, i };
            hashtable[num] = i++;
        }

        return {};
    }
};
// @lc code=end

