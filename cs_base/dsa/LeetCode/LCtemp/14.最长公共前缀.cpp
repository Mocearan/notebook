/*
 * @lc app=leetcode.cn id=14 lang=cpp
 *
 * [14] 最长公共前缀
 *
 * https://leetcode-cn.com/problems/longest-common-prefix/description/
 *
 * algorithms
 * Easy (39.25%)
 * Likes:    1501
 * Dislikes: 0
 * Total Accepted:    467.9K
 * Total Submissions: 1.2M
 * Testcase Example:  '["flower","flow","flight"]'
 *
 * 编写一个函数来查找字符串数组中的最长公共前缀。
 * 
 * 如果不存在公共前缀，返回空字符串 ""。
 * 
 * 
 * 
 * 示例 1：
 * 
 * 
 * 输入：strs = ["flower","flow","flight"]
 * 输出："fl"
 * 
 * 
 * 示例 2：
 * 
 * 
 * 输入：strs = ["dog","racecar","car"]
 * 输出：""
 * 解释：输入不存在公共前缀。
 * 
 * 
 * 
 * 提示：
 * 
 * 
 * 0 
 * 0 
 * strs[i] 仅由小写英文字母组成
 * 
 * 
 */

// @lc code=start
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if(strs.empty()) return "";

        string ret{""};

        for(size_t i = 0; i < strs[0].length(); ++i) {
            for(const auto& str : strs) {
                if(strs[0][i] not_eq str[i]) return ret; 
            }

            ret += strs[0][i];
        }

        return ret;
    }
};
// @lc code=end

