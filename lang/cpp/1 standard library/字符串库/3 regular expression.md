# regular expression

​		正则表达式提供了一种简单而简洁地在文本中描述模式的方法。

​		``<regex>``

---

> For a more exhaustive presentation of regular expressions, see [[Friedl,1997](ch19.xhtml#bib19)].
>
> *[Friedl,1997]*  Jeffrey E. F. Friedl: *Mastering Regular Expressions*. O’Reilly Media. Sebastopol, California. 1997. ISBN 978-1565922570.

```c++
regex pat {R"(\w{2}\s*\d{5}(-\d{4})?)"};   // U.S. postal code pattern: XXddddd-dddd and variants
regex pat {"\\w{2}\\s*\\d{5}(-\\d{4})?"};    // U.S. postal code pattern
```

​		以``R"()"``开头的原始字符串文字，允许在字符串中直接使用 `\`和 `""`。原始字符串特别适合于正则表达式，因为它们往往包含大量的反斜杠。

​		在<regex>中，标准库以``std::regex``类及其支持函数的形式提供了对正则表达式的支持:

- `regex_match()`:将正则表达式与已知大小的字符串匹配
- `regex_search()`:在(任意长)数据流中搜索匹配正则表达式的字符串
- `regex_replace()`:在(任意长)数据流中搜索匹配正则表达式的字符串并替换它们
- `regex_iterator`:遍历匹配和子匹配
- `regex_token_iterator`:遍历不匹配的对象

​		正则表达式的语法和语义被设计成，可以将正则表达式编译成状态机以实现高效执行，``regex``类型在运行时执行此编译结果。



## searching

```c++
int lineno = 0;
for (string line; getline(cin,line); ) {                  // read into line buffer
    ++lineno;
    smatch matches;                                       // matched strings go here
	if (regex_search(line,matches,pat))         // search for pat in line
        cout << lineno << ": " << matches[0] << '\n';
}
```

- 在``line``中搜索与``pat``中存储的正则表达式匹配的任何内容

- 如果找到任何匹配项，则将它们存储在匹配项中

- 如果没有找到匹配，``regex_search(line,matches,pat)``返回``false``

- `smatch`，是正则表达式结果的容器

  - `“s”`代表“子”或“字符串”，``smatch`是字符串类型的子匹配向量。

  - 第一个元素 `match[0]`，是完全匹配，是整个模式。`match[1]`是可选的四位数子模式``(-\d{4})?``

  - `\n`可以是模式的一部分，因此可以搜索多行模式。如果这样做，不应该一次读一行。

  - `regex_search()`的结果是一个匹配的集合，通常表示为``smatch``:

    ```c++
    void use()
    {
    	ifstream in("file.txt");        // input file
        if (!in) {                               // check that the file was opened
            cerr << "no file\n";
            return;
        }
    
        regex pat {R"(\w{2}\s*\d{5}(-\d{4})?)"};   // U.S. postal code pattern
    
        int lineno = 0;
        for (string line; getline(in,line); ) {
        ++lineno;
        smatch matches;     // matched strings go here
        if (regex_search(line, matches, pat)) {
            cout << lineno << ": " << matches[0] << '\n';           // the complete match
            if (1<matches.size() && matches[1].matched)        // if there is a sub-pattern
                                                               // and if it is matched
                cout << "\t: " << matches[1] << '\n';                 // submatch
          }
      }
    }                  
    ```



##	正则表达式符号

​		`regex`库可以识别正则表达式表示法的几种变体。这里，我使用默认表示法，这是用于ECMAScript(通常称为JavaScript)的ECMA标准的一种变体。正则表达式的语法是基于特殊字符的:

​										**Regular Expression Special Characters**

|       |                                     |       |                                       |
| :---- | ----------------------------------- | ----- | ------------------------------------- |
| ``.`` | Any single character (a “wildcard”) | **\** | Next character has a special meaning  |
| ``[`` | Begin character class               | ``*`` | 0 or more (suffix operation)          |
| ``]`` | End character class                 | ``+`` | 1 or more (suffix operation)          |
| `{`   | Begin count                         | ``?`` | Optional (0 or 1)  (suffix operation) |
| ``}`` | End count                           | ``|`` | Alternative (or)                      |
| ``(`` | Begin grouping                      | ``^`` | Start of line; negation               |
| ``)`` | End grouping                        | ``$`` | End of line                           |

```js
^A*B+C?$ // 一行以0个或多个A开头，后面跟着一个或多个B，后随一个可选的C

// match:
// AAAAAAAAAAAABBBBBBBBBC
// BC
// B

// not match:
// AAAAA                    // no B
//    AAAABC             // initial space
// AABBCC                 // too many Cs
```

- 圆括号中，被认为是子模式，可以从``smatch``中单独提取

  ```js
  \d+-\d+            // no subpatterns
  \d+(-\d+)          // one subpattern
  (\d+)(-\d+)        // two subpatterns
  ```

- 通过添加后缀，模式可以是可选的或重复的(默认是一次):

  ```c++
  { n } // Exactly n times
  { n, } // n or more times
  {n,m} // At least n and at most m times
  * // Zero or more, that is, {0,}
  + // One or more, that is, {1,}
  ? // Optional (zero or one), that is {0,1}
      
      
  // A{3}B{2,4}C*
      // AAABBC
      // AAABBB
  ```

- ``?``后缀，在任何重复符号(``? / * / + / {}``) 使模式匹配器寻找最短的匹配，而不是最长的匹配。

  > 默认情况下，模式匹配器总是寻找最长的匹配。这被称为马克斯·蒙克法则。

  ```c++
  ababab
  // (ab)+  matches all of ababab
  // (ab)+? matches only the first ab.
  ```

- 在正则表达式中，字符类名必须用``[::]``括起来

  ​	此外，它们必须在定义字符类的``[]``对中使用

  ```c++
    // Character Classes
  alnumAny alphanumeric character
  alpha // Any alphabetic character
  blank // Any whitespace character that is not a line separator
  cntrl // Any control character
  d // Any decimal digit
  digit // Any decimal digit
  graph // Any graphical character
  lower // Any lowercase character
  print // Any printable character
  punct // Any punctuation character
  s // Any whitespace character
  space // Any whitespace character
  upper // Any uppercase character
  w // Any word character (alphanumeric characters plus the underscore)
  xdigit // Any hexadecimal digit character
      
      // [:digit:] matches a decimal digit.
  ```

  - **Character Class Abbreviations**

    | **\d** | A decimal digit                                              | **[[:digit:]]**  |
    | ------ | ------------------------------------------------------------ | ---------------- |
    | **\s** | A space (space, tab, etc.)                                   | **[[:space:]]**  |
    | **\w** | A letter (**a**-**z**) or digit (**0**-**9**) or underscore (**_**) | **[_[:alnum:]]** |

    | **\D** | Not **\d** | **[^[:digit:]]**  |
    | ------ | ---------- | ----------------- |
    | **\S** | Not **\s** | **[^[:space:]]**  |
    | **\W** | Not **\w** | **[^_[:alnum:]]** |
  
  **Nonstandard (but Common) Character Class Abbreviations**
  
  | **\l** | A lowercase character  | **[[:lower:]]**  |
  | ------ | ---------------------- | ---------------- |
  | **\u** | An uppercase character | **[[:upper:]]**  |
  | **\L** | Not **\l**             | **[^[:lower:]]** |
  | **\U** | Not **\u**             | **[^[:upper:]]** |
  
  > 为了完全的可移植性，使用字符类名而不是这些缩写。
  >
  > 描述c++标识符的模式:下划线或字母后面可能是空白的字母、数字或下划线序列
  >
  > ```c++
  > [:alpha:][:alnum:]*                  // wrong: characters from the set ":alpha" followed by ...
  > [[:alpha:]][[:alnum:]]*             // wrong: doesn't accept underscore ( _ is not alpha)
  > ([[:alpha:]]|_)[[:alnum:]]*        // wrong: underscore is not part of alnum either
  > 
  > ([[:alpha:]]|_)([[:alnum:]]|_)*           // OK, but clumsy
  > [[:alpha:]_][[:alnum:]_]*                  // OK: include the underscore in the character classes
  > [_[:alpha:]][_[:alnum:]]*                  // also OK
  > [_[:alpha:]]\w*                                 // \w is equivalent to [_[:alnum:]]
  >     
  >     
  > bool is_identifier(const string& s)
  > {
  >        // regex pat {"[_[:alpha:]]\\w*"};  // underscore or letter
  >      regex pat {R"([_[:alpha:]]\w*)"};
  >       // followed by zero or more underscores, letters, or digits
  >  
  >     return regex_match(s,pat);
  > }
  > 
  > /* example
  > Ax*                  // A, Ax, Axxxx
  > Ax+                 // Ax, Axxx         Not A
  > \d-?\d              // 1-2, 12            Not 1--2
  > \w{2}-\d{4,5}   // Ab-1234, XX-54321, 22-5432         Digits are in \w
  > (\d*:)?(\d+)      // 12:3, 1:23, 123, :123       Not 123:
  > (bs|BS)           // bs, BS             Not bS
  > [aeiouy]          // a, o, u              An English vowel, not x
  > [^aeiouy]        // x, k                  Not an English vowel, not e
  > [a^eiouy]        // a, ^, o, u           An English vowel or ^
  > */
  > ```
  
- 可能由``sub_match``表示的组(子模式)由`()`分隔

  如果需要不应该定义子模式的括号，请使用``(?``而不是平淡的。

  ```c++
  (\s|:|,)*(\d*)      // optional spaces, colons, and/or commas followed by an optional number
  ```

  假设我们对数字之前的字符不感兴趣(假设是分隔符)，我们可以这样写:

  ```c++
  (?:\s|:|,)*(\d*)    // optional spaces, colons, and/or commas followed by an optional number
  ```

  这将使正则表达式引擎不必存储第一个字符``(?:`` variant只有一个子模式

  ```c++
  // Regular Expression Grouping Examples
  \d*\s\w+ // No groups (subpatterns)
  (\d*)\s(\w+) // Two groups
  (\d*)(\s(\w+))+ // Two groups (groups do not nest)
  (\s*\w*)+ // One group; one or more subpatterns;
  		// only the last subpattern is saved as a sub_match
  <(.*?)>(.*?)</\1> // Three groups; the \1 means “same as group 1”
  ```

  > ​		最后一个模式对于解析XML很有用。它查找标签/标签结束标记。注意，我使用了一个非贪婪匹配(一个懒惰匹配)，.*?，表示标记和结束标记之间的子模式。如果我使用plain .*，这个输入会导致一个问题吗
  >
  > ```xml
  > Always look on the <b>bright</b> side of <b>life</b>.
  > ```
  >
  > ​		第一个子模式的贪婪匹配将匹配第一个<和最后一个>。这是正确的行为，但不太可能是程序员想要的。

​		

## Iterators

​		我们可以定义一个regex_iterator，用于遍历字符序列以查找匹配的模式。例如，我们可以使用``sregex_iterator (a regex_iterator<string>)``来输出字符串中所有以空格分隔的单词:

```c++
void test()
{
    string input = "aa as; asd ++e^asdf asdfg";
    regex pat {R"(\s+(\w+))"};
    for (sregex_iterator p(input.begin(),input.end(),pat); p!=sregex_iterator{}; ++p)
        cout << (*p)[1] << '\n';
}

/*
This outputs:

    as
    asd
    asdfg
*/
```

​		我们漏掉了第一个单词aa，因为它前面没有空格。如果我们将模式简化为R"((\w+))"，我们得到

```c++
aa
as
asd
e
asdf
asdfg
```

​		`regex_iterator`是一个双向迭代器，因此不能直接迭代``istream``(它只提供输入迭代器)。此外，我们不能通过``regex_iterator``进行写操作，默认的``regex_iterator (regex_iterator{})``是唯一可能的序列结束符
