/*
 * @lc app=leetcode.cn id=9 lang=cpp
 *
 * [9] 回文数
 *
 * https://leetcode-cn.com/problems/palindrome-number/description/
 *
 * algorithms
 * Easy (58.71%)
 * Likes:    1416
 * Dislikes: 0
 * Total Accepted:    581.7K
 * Total Submissions: 990.8K
 * Testcase Example:  '121'
 *
 * 给你一个整数 x ，如果 x 是一个回文整数，返回 true ；否则，返回 false 。
 * 
 * 回文数是指正序（从左向右）和倒序（从右向左）读都是一样的整数。例如，121 是回文，而 123 不是。
 * 
 * 
 * 
 * 示例 1：
 * 
 * 
 * 输入：x = 121
 * 输出：true
 * 
 * 
 * 示例 2：
 * 
 * 
 * 输入：x = -121
 * 输出：false
 * 解释：从左向右读, 为 -121 。 从右向左读, 为 121- 。因此它不是一个回文数。
 * 
 * 
 * 示例 3：
 * 
 * 
 * 输入：x = 10
 * 输出：false
 * 解释：从右向左读, 为 01 。因此它不是一个回文数。
 * 
 * 
 * 示例 4：
 * 
 * 
 * 输入：x = -101
 * 输出：false
 * 
 * 
 * 
 * 
 * 提示：
 * 
 * 
 * -2^31 
 * 
 * 
 * 
 * 
 * 进阶：你能不将整数转为字符串来解决这个问题吗？
 * 
 */

// @lc code=start
// class Solution {
// public:
//     bool isPalindrome(int x) {
//         if ( x < 0 or ( x not_eq 0 and x % 10 == 0 ) ) {
//             return false;
//         }

//         int x2{0};
//         while(x > x2) {
//             x2 = x2 * 10 + x % 10;
//             x /= 10;
//         }

//         // x2 / 10 remove midlle shared number
//         //  ex: 12321 -> 12/123 -> remove 3 -> 12/12
//         return x == x2 or x == x2 / 10;
//     }
// };

class Solution
{
public:
    bool isPalindrome(int x)
    {
        if (x < 0 or ( x not_eq 0 and x % 10 == 0 ) ) {
            return false;
        }

        long x2{0};
        int cx = x;
        while(x) {
            x2 = x2 * 10 + x % 10;
            x /= 10;
        }

        // x2 / 10 remove midlle shared number
        //  ex: 12321 -> 12/123 -> remove 3 -> 12/12
        return cx == x2 and x2 >= 0;
    }
};
// @lc code=end

