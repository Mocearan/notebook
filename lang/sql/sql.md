# SQL

​		SQL 是一种为数不多的声明性语言

---

## 理解

### 声明性语言

​		SQL 语言是为计算机声明从原始数据中获得结果的范例，而不是告诉计算机如何能够得到结果。

​		SQL 语言声明的是结果集的属性，计算机会根据 SQL 所声明的内容来从数据库中挑选出符合声明的数据，而不是像传统编程思维去指示计算机如何操作。

### 顺序

​		SQL 语句的执行顺序跟其语句的语法顺序并不一致。SQL 语句的语法顺序是：

- SELECT[DISTINCT]
- FROM
- WHERE
- GROUP BY
- HAVING
- UNION
- ORDER BY

​		执行顺序为：

- FROM
- WHERE
- GROUP BY
- HAVING
- SELECT
- DISTINCT
- UNION
- ORDER BY

​		关于 SQL 语句的执行顺序，有三个值得我们注意的地方：

1. `FROM `才是 `SQL `语句执行的第一步，并非 `SELECT`

   ​	数据库在执行 SQL 语句的第一步是将数据从硬盘加载到数据缓冲区中，以便对这些数据进行操作。

2.  `SELECT `是在大部分语句执行了之后才执行的，严格的说是在 `FROM `和 `GROUP BY` 之后执行的。

   ​	这是`SELECT `中设定别名的字段，不能在 `WHERE `中作为判断条件使用的原因。

   ```sql
   SELECT A.x + A.y AS z
   FROM A
   WHERE z = 10 -- z 在此处不可用，因为SELECT是最后执行的语句！
   
   如果你想重用别名z，你有两个选择。要么就重新写一遍 z 所代表的表达式：
   
   SELECT A.x + A.y AS z
   FROM A
   WHERE (A.x + A.y) = 10
   
   …或者求助于衍生表、通用数据表达式或者视图，以避免别名重用。
   ```

3.  无论在语法上还是在执行顺序上， `UNION `总是排在在 `ORDER BY` 之前。

   ​		尽管某些数据库允许 `SQL `语句对子查询（subqueries）或者派生表（derived tables）进行排序，但是这并不说明这个排序在 `UNION `操作过后仍保持排序后的顺序。

   > ​		并非所有的数据库对 SQL 语句使用相同的解析方式。如 MySQL、PostgreSQL和 SQLite 中就不会按照上面第二点中所说的方式执行。



​		不是所有的数据库都按照上述方式执行 SQL 预计，我们永远要记得： SQL 语句的语法顺序和其执行顺序并不一致，这样我们就能避免一般性的错误。如果你能记住 SQL 语句语法顺序和执行顺序的差异，你就能很容易的理解一些很常见的 SQL 问题。

​		当然，如果一种语言被设计成语法顺序直接反应其语句的执行顺序，那么这种语言对程序员是十分友好的，这种编程语言层面的设计理念已经被微软应用到了 LINQ 语言中。

### 核心是对表的引用（table references）

​		 SQL 语句语法顺序和执行顺序的不同，很多同学会认为`SELECT `中的字段信息是 `SQL `语句的核心。其实真正的核心在于对表的引用。

​		如果我们从集合论（关系代数）的角度来看，一张数据库的表就是一组数据元的关系，而每个 SQL 语句会改变一种或数种关系，从而产生出新的数据元的关系（即产生新的表）。

​		根据 SQL 标准，`FROM `语句被定义为：

```sql
<from clause> ::= FROM <table reference> [ { <comma> <table reference> }... ]
```

​		`FROM `语句的“输出”是一张联合表，来自于所有引用的表在某一维度上的联合。

> `FROM a, b`
>
> - 如果 a 表有三个字段， b 表有 5 个字段，那么这个“输出表”就有 8 （ =5+3）个字段。
>
> - 这个联合表里的数据是 `a*b`，即 a 和 b 的笛卡尔积。
>
>   ​	换句话说，也就是 a 表中的每一条数据都要跟 b 表中的每一条数据配对。
>
>   ​	如果 a 表有3 条数据， b 表有 5 条数据，那么联合表就会有 15 （ =5*3）条数据。

​		`FROM `输出的结果被 `WHERE `语句筛选后要经过 `GROUP BY` 语句处理，从而形成新的输出结果。



### 灵活引用表

​		灵活引用表能使 SQL 语句变得更强大。

​		一个简单的例子就是 `JOIN `的使用。**严格的说 `JOIN `语句并非是 `SELECT `中的一部分，而是一种特殊的表引用语句。 **SQL 语言标准中表的连接定义如下：

```sql
<table reference> ::=
    <table name>
  | <derived table>
  | <joined table>
```

> `FROM a, b`
>
> - a 可能输如下表的连接：
>
>   ```sql
>   a1 JOIN a2 ON a1.id = a2.id
>   
>   将它放到之前的例子中就变成了：
>   FROM a1 JOIN a2 ON a1.id = a2.id, b
>   ```
>
> - 尽管将一个连接表用逗号跟另一张表联合在一起并不是常用作法，但是你的确可以这么做。结果就是，最终输出的表就有了 a1+a2+b 个字段了。
>
>   > 原文这里用词为 `degree `，译为维度。如果把一张表视图化，我们可以想象每一张表都是由横纵两个维度组成的，横向维度即我们所说的字段或者列，英文为`columns`；纵向维度即代表了每条数据，英文为 `record `，根据上下文，作者这里所指的应该是字段数。

​		在 SQL 语句中派生表的应用甚至比表连接更加强大。

​		思考问题时，要从表引用的角度出发，这样就很容易理解数据是怎样被 `SQL `语句处理的，并且能够帮助你理解那些复杂的表引用是做什么的。

​		更重要的是，要理解 `JOIN `是构建连接表的关键词，并不是 `SELECT `语句的一部分。有一些数据库允许在 `INSERT `、 `UPDATE `、 `DELETE `中使用 `JOIN `。



### 推荐使用表连接

​		尽量不要使用逗号来代替 `JOIN `进行表的连接，这样会提高你的 `SQL `语句的可读性，并且可以避免一些错误。利用逗号来简化 `SQL `语句有时候会造成思维上的混乱

```sql
FROM a, b, c, d, e, f, g, h
WHERE a.a1 = b.bx
AND a.a2 = c.c1
AND d.d1 = b.bc
-- etc...
```

​		`JOIN `语句的好处在于：

- 安全。 `JOIN `和要连接的表离得非常近，这样就能避免错误。
- 更多连接的方式，`JOIN `语句能去区分出来外连接和内连接等。

​		要尽量使用 `JOIN `进行表的连接，永远不要在 `FROM `后面使用逗号连接表。

### 不同的连接操作

​		`SQL `语句中，表连接的方式从根本上分为五种：

- EQUI JOIN
- SEMI JOIN
- ANTI JOIN
- CROSS JOIN
- DIVISION

​		 `SQL `是对表的引用， `JOIN `则是一种引用表的复杂方式。但是 `SQL `语言的表达方式和实际我们所需要的逻辑关系之间是有区别的，并非所有的逻辑关系都能找到对应的 `JOIN `操作，所以这就要我们在平时多积累和学习关系逻辑，这样你就能在以后编写 `SQL `语句中选择适当的 `JOIN `操作了。

#### EQUI JOIN

​		最普通的 `JOIN `操作，它包含两种连接方式：

- `INNER JOIN`（或者是 `JOIN `）
- `OUTER JOIN`（包括： `LEFT `、 `RIGHT`、 `FULL OUTER JOIN`）

用例子最容易说明其中区别：

```sql
-- This table reference contains authors and their books.
-- There is one record for each book and its author.
-- authors without books are NOT included
author JOIN book ON author.id = book.author_id
 
-- This table reference contains authors and their books
-- There is one record for each book and its author.
-- ... OR there is an "empty" record for authors without books
-- ("empty" meaning that all book columns are NULL)
author LEFT OUTER JOIN book ON author.id = book.author_id
```



#### SEMI JOIN

​		在 `SQL `中有两种表现方式：使用 IN，或者使用 EXISTS。

​		“ SEMI ”在拉丁文中是“半”的意思。这种连接方式是只连接目标表的一部分。

​		这是什么意思呢？再想一下上面关于作者和书名的连接。我们想象一下这样的情况：我们不需要作者 / 书名这样的组合，只是需要那些在书名表中的书的作者信息。那我们就能这么写：

```sql
-- Using IN
FROM author
WHERE author.id IN (SELECT book.author_id FROM book)
 
-- Using EXISTS
FROM author
WHERE EXISTS (SELECT 1 FROM book WHERE book.author_id = author.id)
```

​		尽管没有严格的规定说明你何时应该使用 IN ，何时应该使用 EXISTS ，但是这些事情你还是应该知道的：

- `IN`比 `EXISTS `的可读性更好
- `EXISTS `比`IN `的表达性更好（更适合复杂的语句）
- 二者之间性能没有差异（但对于某些数据库来说性能差异会非常大）

​		因为使用` INNER JOIN `也能得到书名表中书所对应的作者信息，所以很多初学者机会认为可以通过 DISTINCT 进行去重，然后将 `SEMI JOIN `语句写成这样：

```sql
-- Find only those authors who also have books
SELECT DISTINCT first_name, last_name
FROM author
JOIN book ON author.id = book.author_id
```

​		这是一种很糟糕的写法，原因如下：

- `SQL `语句性能低下：因为去重操作（ `DISTINCT `）需要数据库重复从硬盘中读取数据到内存中。（译者注： `DISTINCT `的确是一种很耗费资源的操作，但是每种数据库对于 DISTINCT 的操作方式可能不同）。
- 这么写并非完全正确：尽管也许现在这么写不会出现问题，但是随着 SQL 语句变得越来越复杂，你想要去重得到正确的结果就变得十分困难。

​		更多的关于滥用 `DISTINCT `的危害可以参考这篇博文：http://blog.jooq.org/2013/07/30/10-common-mistakes-java-developers-make-when-writing-sql/。



#### ANTI JOIN

​		这种连接的关系跟 SEMI JOIN 刚好相反。在 IN 或者 EXISTS 前加一个 NOT 关键字就能使用这种连接。举个例子来说，我们列出书名表里没有书的作者：

```sql
-- Using IN
FROM author
WHERE author.id NOT IN (SELECT book.author_id FROM book)
 
-- Using EXISTS
FROM author
WHERE NOT EXISTS (SELECT 1 FROM book WHERE book.author_id = author.id)
```

​		关于性能、可读性、表达性等特性也完全可以参考 SEMI JOIN。

​		这篇博文介绍了在使用` NOT IN `时遇到 `NULL `应该怎么办，因为有一点背离本篇主题，就不详细介绍，有兴趣的同学可以读一下：http://blog.jooq.org/2012/01/27/sql-incompatibilities-not-in-and-null-values/。



#### CROSS JOIN

​		这个连接过程就是两个连接的表的乘积：即将第一张表的每一条数据分别对应第二张表的每条数据。

​		我们之前见过，这就是逗号在 `FROM `语句中的用法。在实际的应用中，很少有地方能用到 `CROSS JOIN`，但是一旦用上了，你就可以用这样的 `SQL`语句表达：

```sql
-- Combine every author with every book
author CROSS JOIN book
```



#### DIVISION

​		`DIVISION `的确是一个怪胎。简而言之，如果 `JOIN `是一个乘法运算，那么 `DIVISION `就是 `JOIN `的逆过程。

​		`DIVISION `的关系很难用 `SQL `表达出来，介于这是一个新手指南，解释 `DIVISION `已经超出了我们的目的。但是有兴趣的同学还是可以来看看这三篇文章：

- http://blog.jooq.org/2012/03/30/advanced-sql-relational-division-in-jooq/
- http://en.wikipedia.org/wiki/Relational_algebra#Division
- https://www.simple-talk.com/sql/t-sql-programming/divided-we-stand-the-sql-of-relational-division/。



### 派生表

​		`SQL `是一种声明性的语言，并且 `SQL `语句中不能包含变量。但是你能写出类似于变量的语句，这些就叫做派生表：

```sql
-- A derived table
FROM (SELECT * FROM author)
```

> 说白了，所谓的派生表就是在括号之中的子查询

​		需要注意的是有些时候我们可以给派生表定义一个相关名（即我们所说的别名）。

```sql
-- A derived table with an alias
FROM (SELECT * FROM author) a
```

​		派生表可以有效的避免由于 `SQL `逻辑而产生的问题。举例来说：如果你想重用一个用 `SELECT `和 `WHERE `语句查询出的结果，这样写就可以（以 Oracle 为例）：

```sql
-- Get authors' first and last names, and their age in days
SELECT first_name, last_name, age
FROM (
  SELECT first_name, last_name, current_date - date_of_birth age
  FROM author
)
-- If the age is greater than 10000 days
WHERE age > 10000
```

​		需要我们注意的是：在有些数据库，以及 `SQL:1990` 标准中，派生表被归为下一级——通用表语句（ common table experssion）。这就允许你在一个 `SELECT `语句中对派生表多次重用。上面的例子就（几乎）等价于下面的语句：

```sql
WITH a AS (
  SELECT first_name, last_name, current_date - date_of_birth age
  FROM author
)
SELECT *
FROM a
WHERE age > 10000
```

​		当然了，你也可以给“ a ”创建一个单独的视图，这样你就可以在更广泛的范围内重用这个派生表了。更多信息可以阅读下面的文章（http://en.wikipedia.org/wiki/View_%28SQL%29）。

​		大体上来说 SQL 语句就是对表的引用，而并非对字段的引用。要好好利用这一点，不要害怕使用派生表或者其他更复杂的语句。

### `GROUP BY `是对表的引用进行的操作

​		`GROUP BY`，是在表的引用上进行了操作，将其转换为一种新的引用方式。

​		当你应用 `GROUP BY `的时候， `SELECT `后没有使用聚合函数的列，都要出现在 `GROUP BY `后面。

> 译者注：原文大意为“当你是用 `GROUP BY `的时候，你能够对其进行下一级逻辑操作的列会减少，包括在 `SELECT `中的列”）。

​		需要注意的是：其他字段能够使用聚合函数：

```sql
SELECT A.x, A.y, SUM(A.z)
FROM A
GROUP BY A.x, A.y
```

​		还有一点值得留意的是： `MySQL `并不坚持这个标准，这的确是令人很困惑的地方。（译者注：这并不是说 `MySQL `没有` GROUP BY `的功能）但是不要被 `MySQL `所迷惑。` GROUP BY `改变了对表引用的方式。你可以像这样既在 `SELECT `中引用某一字段，也在 `GROUP BY `中对其进行分组。



### `SELECT `实质上是对关系的映射

​		一旦你建立起来了表的引用，经过修改、变形，你能够一步一步的将其映射到另一个模型中。 SELECT 语句就像一个“投影仪”，我们可以将其理解成一个将源表中的数据按照一定的逻辑转换成目标表数据的函数。

​		通过 `SELECT`语句，你能对每一个字段进行操作，通过复杂的表达式生成所需要的数据。

​		`SELECT `语句有很多特殊的规则，至少你应该熟悉以下几条：

1. 你仅能够使用那些能通过表引用而得来的字段；

2. 如果你有 GROUP BY 语句，你只能够使用 GROUP BY 语句后面的字段或者聚合函数；

3. 当你的语句中没有 GROUP BY 的时候，可以使用开窗函数代替聚合函数；

4. 当你的语句中没有 GROUP BY 的时候，你不能同时使用聚合函数和其它函数；

5. 有一些方法可以将普通函数封装在聚合函数中；

   ……

​		一些更复杂的规则多到足够写出另一篇文章了。比如：为何你不能在一个没有 GROUP BY 的 SELECT 语句中同时使用普通函数和聚合函数？（上面的第 4 条）

​		原因如下：

- 凭直觉，这种做法从逻辑上就讲不通。
- 如果直觉不能够说服你，那么语法肯定能。 SQL : 1999 标准引入了 GROUPING SETS，SQL： 2003 标准引入了 group sets : GROUP BY() 。无论什么时候，只要你的语句中出现了聚合函数，而且并没有明确的 GROUP BY 语句，这时一个不明确的、空的 GROUPING SET 就会被应用到这段 SQL 中。因此，原始的逻辑顺序的规则就被打破了，映射（即 SELECT ）关系首先会影响到逻辑关系，其次就是语法关系。（译者注：这段话原文就比较艰涩，可以简单理解如下：在既有聚合函数又有普通函数的 SQL 语句中，如果没有 GROUP BY 进行分组，SQL 语句默认视整张表为一个分组，当聚合函数对某一字段进行聚合统计的时候，引用的表中的每一条 record 就失去了意义，全部的数据都聚合为一个统计值，你此时对每一条 record 使用其它函数是没有意义的）。

​		`SELECT `语句可能是 `SQL `语句中最难的部分了，尽管他看上去很简单。其他语句的作用其实就是对表的不同形式的引用。而 `SELECT `语句则把这些引用整合在了一起，通过逻辑规则将源表映射到目标表，而且这个过程是可逆的，我们可以清楚的知道目标表的数据是怎么来的。

想要学习好 `SQL `语言，就要在使用 `SELECT `语句之前弄懂其他的语句，虽然 `SELECT `是语法结构中的第一个关键词，但它应该是我们最后一个掌握的。



### 几个简单的关键词： DISTINCT ， UNION ， ORDER BY 和 OFFSET

- 集合运算（ DISTINCT 和 UNION ）
- 排序运算（ ORDER BY，OFFSET…FETCH）

**集合运算（ set operation）：**

集合运算主要操作在于集合上，事实上指的就是对表的一种操作。从概念上来说，他们很好理解：



- DISTINCT 在映射之后对数据进行去重
- UNION 将两个子查询拼接起来并去重
- UNION ALL 将两个子查询拼接起来但不去重
- EXCEPT 将第二个字查询中的结果从第一个子查询中去掉
- INTERSECT 保留两个子查询中都有的结果并去重



**排序运算（ ordering operation）：**

排序运算跟逻辑关系无关。这是一个 SQL 特有的功能。排序运算不仅在 SQL 语句的最后，而且在 SQL 语句运行的过程中也是最后执行的。使用 ORDER BY 和 OFFSET…FETCH 是保证数据能够按照顺序排列的最有效的方式。其他所有的排序方式都有一定随机性，尽管它们得到的排序结果是可重现的。


OFFSET…SET是一个没有统一确定语法的语句，不同的数据库有不同的表达方式，如 MySQL 和 PostgreSQL 的 LIMIT…OFFSET、SQL Server 和 Sybase 的 TOP…START AT 等。具体关于 OFFSET..FETCH 的不同语法可以参考这篇文章：
http://www.jooq.org/doc/3.1/manual/sql-building/sql-statements/select-statement/limit-clause/。

让我们在工作中尽情的使用 SQL！

正如其他语言一样，想要学好 SQL 语言就要大量的练习。上面的 10 个简单的步骤能够帮助你对你每天所写的 SQL 语句有更好的理解。另一方面来讲，从平时常见的错误中也能积累到很多经验。下面的两篇文章就是介绍一些 JAVA 和其他开发者所犯的一些常见的 SQL 错误：



- 10 Common Mistakes Java Developers Make when Writing SQL
- 10 More Common Mistakes Java Developers Make when Writing SQL



## 思想：一切皆关系

​		在计算机领域有许多伟大的设计理念和思想，例如：

- 在 Unix 中，一切皆文件。
- 在面向对象的编程语言中，一切皆对象。

​		关系数据库同样也有自己的设计思想：在 SQL 中，一切皆关系。



### 关系模型

​		关系模型（Relational model）由 E.F.Codd 博士于 1970 年提出，以集合论中的关系概念为基础；无论是现实世界中的实体对象还是它们之间的联系都使用关系表示。

​		数据库系统中看到的关系就是二维表（Table），由行（Row）和列（Column）组成。因此，也可以说关系表是由数据行构成的集合。

​		关系模型由**数据结构**、**关系操作**、**完整性约束**三部分组成。

- 关系模型中的数据结构就是关系表，包括**基础表**、**派生表**（查询结果）和**虚拟表**（视图）。
- 常用的关系操作包括**增加**、**删除**、**修改**和**查询**（CRUD），使用的就是 SQL 语言。其中查询操作最为复杂，包括选择（Selection）、投影（Projection）、并集（Union）、交集（Intersection）、差集（Exception）以及笛卡儿积（Cartesian product）等。
- 完整性约束用于维护数据的完整性或者满足业务约束的需求，包括**实体完整性**（主键约束）、**参照完整性**（外键约束）以及**用户定义的完整性**（非空约束、唯一约束、检查约束和默认值）。

​		关系操作语言，也就是 SQL



### 面向集合

​		SQL（结构化查询语言）是操作关系数据库的标准语言。SQL 非常接近英语，使用起来非常简单。它在设计之初就考虑了非技术人员的使用需求，我们通常只需说明想要的结果（What），而将数据处理的过程（How）交给数据库管理系统。

​		 **SQL 是一种面向集合的编程语言，它的操作对象是集合，操作的结果也是集合**。

> 在关系数据库中，关系、表、集合三者通常表示相同的概念。



#### SELECT

```sql
SELECT employee_id, first_name, last_name, hire_date
  FROM employees;
```

- `FROM `之后是一个表（关系、集合）。
- 整个查询语句的结果也是一个表。

​		可以将上面的查询作为表使用：

```sql
SELECT *
  FROM (SELECT employee_id, first_name, last_name, hire_date
          FROM employees) t;
```

​		括号内的查询语句被称为派生表，我们给它指定了一个别名叫做 t.

​		PostgreSQL 中upper() 是一个大写转换的函数。它出现再 FROM 子句中，意味着它的结果也是一个表，只不过是 1 行 1 列的特殊表。

```sql
-- PostgreSQL
SELECT *
  FROM upper('sql');
| upper |
|-------|
|   SQL |
```

​		SELECT 子句用于指定需要查询的字段，可以包含表达式、函数值等。SELECT 在关系操作中被称为投影（Projection），看下面的示意图应该就比较好理解了。

![图片](https://mmbiz.qpic.cn/mmbiz_png/Pn4Sm0RsAujocXlZZM2qFMbuZtEcgicDVYbHibXuFRibyjk3Jp5E2wa7CnzeJQ1dQt8VKBgeYCgcpdRedKsmY95Mg/640?wx_fmt=png&wxfrom=13&tp=wxpic)

​		WHERE 用于指定数据过滤的条件，在关系运算中被称为选择（Selection）

![图片](https://mmbiz.qpic.cn/mmbiz_png/Pn4Sm0RsAujocXlZZM2qFMbuZtEcgicDVKib109KD7Y1Yicbl5E6nz3wOPVsdSh5Er7UtS2djyVCZ5vuVhaVA6p7Q/640?wx_fmt=png&wxfrom=13&tp=wxpic)

​		ORDER BY 用于对查询的结果进行排序

![图片](https://mmbiz.qpic.cn/mmbiz_png/Pn4Sm0RsAujocXlZZM2qFMbuZtEcgicDVegicmvPAokib9t3Uqztd4obX9FNXnUJ2ddvFeFEobVviaoc9zGKvkyf2w/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

​		总之，SQL 可以完成各种数据操作，例如过滤、分组、排序、限定数量等；所有这些操作的对象都是关系表，结果也是关系表。

![图片](https://mmbiz.qpic.cn/mmbiz_png/Pn4Sm0RsAujocXlZZM2qFMbuZtEcgicDV9hvhjh7bxQb6uNfFcqCQEZEr5O7oZHrvxfjBUHicic5cEZGJ41zt0gbw/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

​		在这些关系操作中，有一个比较特殊，就是分组。

#### GROUP BY

​		分组（ GROUP BY）操作和其他的关系操作不同，因为它改变了关系的结构。尽管如此，GROUP BY 的结果仍然是一个集合。

```sql
SELECT department_id, count(*), first_name
  FROM employees
 GROUP BY department_id;
```

​		目的是按照部门统计员工的数量，但是存在一个语法错误，就是 first_name 不能出现在查询列表中。原因在于按照部门进行分组的话，每个部门包含多个员工；无法确定需要显示哪个员工的姓名，这是一个逻辑上的错误。

​		所以说，GROUP BY 改变了集合元素（数据行）的结构，创建了一个全新的关系。

![图片](https://mmbiz.qpic.cn/mmbiz_png/Pn4Sm0RsAujocXlZZM2qFMbuZtEcgicDVyHMEb66ch5SVxQJOiass4BFB6sFL2mPibZKRCkNLwVbicjIRDJAicJpuag/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)



#### UNION

​		SQL 面向集合特性最明显的体现就是 UNION（并集运算）、INTERSECT（交集运算）和 EXCEPT/MINUS（差集运算）。

​		这些集合运算符的作用都是将两个集合并成一个集合，因此需要满足以下条件：

- 两边的集合中字段的数量和顺序必须相同；
- 两边的集合中对应字段的类型必须匹配或兼容。



UNION 和 UNION ALL 用于计算两个集合的并集，返回出现在第一个查询结果或者第二个查询结果中的数据。它们的区别在于 UNION 排除了结果中的重复数据，UNION ALL 保留了重复数据。下面是 UNION 操作的示意图：

![图片](https://mmbiz.qpic.cn/mmbiz_png/Pn4Sm0RsAujocXlZZM2qFMbuZtEcgicDV0Gml7TayMkY6Cq3eeVGBBx7sqc616X2n7guGBA3Xtg4icqlQCE0oFsw/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)


INTERSECT 操作符用于返回两个集合中的共同部分，即同时出现在第一个查询结果和第二个查询结果中的数据，并且排除了结果中的重复数据。INTERSECT 运算的示意图如下：

![图片](https://mmbiz.qpic.cn/mmbiz_png/Pn4Sm0RsAujocXlZZM2qFMbuZtEcgicDV9bmfaczlnMYtzJjwtNBp9m0PO6XBylxV7pmoFRobfWfu3daPgia8fuA/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)


EXCEPT 或者 MINUS 操作符用于返回两个集合的差集，即出现在第一个查询结果中，但不在第二个查询结果中的记录，并且排除了结果中的重复数据。EXCEPT 运算符的示意图如下：

![图片](https://mmbiz.qpic.cn/mmbiz_png/Pn4Sm0RsAujocXlZZM2qFMbuZtEcgicDVueO9qKsKcibP8VEJvRTpocictia6g4Hz5OMpkPRQqMvPLsGhdc8qySpwg/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)


除此之外，DISTINCT 运算符用于消除重复数据，也就是排除集合中的重复元素。



> 📝SQL 中的关系概念来自数学中的集合理论，因此 UNION、INTERSECT 和 EXCEPT 分别来自集合论中的并集（∪\cup∪）、交集（∩\cap∩）和差集（∖\setminus∖）运算。需要注意的是，集合理论中的集合不允许存在重复的数据，但是 SQL 允许。因此，SQL 中的集合也被称为多重集合（multiset）；多重集合与集合理论中的集合都是无序的，但是 SQL 可以通过 ORDER BY 子句对查询结果进行排序。



#### JOIN

​		在 SQL 中，不仅实体对象存储在关系表中，对象之间的联系也存储在关系表中。因此，当我们想要获取这些相关的数据时，需要使用到另一个操作：连接查询（JOIN）。

​		常见的 SQL连接查类型包括**内连接**、**外连接**、**交叉连接**等。其中，外连接又可以分为**左外连接**、**右外连接**以及**全外连接**。

​		内连接（Inner Join）返回两个表中满足连接条件的数据，内连接的原理如下图所示：

![图片](https://mmbiz.qpic.cn/mmbiz_png/Pn4Sm0RsAujocXlZZM2qFMbuZtEcgicDVydGqMNNibZGiavegpic3UPjic8B8Vk8icZbqfA0X7Qo4o96NTldDVlqdvCA/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)
		左外连接（Left Outer Join)返回左表中所有的数据；对于右表，返回满足连接条件的数据；如果没有就返回空值。左外连接的原理如下图所示：

![图片](https://mmbiz.qpic.cn/mmbiz_png/Pn4Sm0RsAujocXlZZM2qFMbuZtEcgicDVepB2VpD6sfGclk1f0bvAWexiaMxJibrqV9aLA4HxhBPbJ9LIxibd4CS8Q/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)
		右外连接（Right Outer Join)返回右表中所有的数据；对于左表，返回满足连接条件的数据，如果没有就返回空值。右外连接与左外连接可以互换，以下两者等价：

```sql
t1 RIGHT JOIN t2t2 LEFT JOIN t1
```

​		全外连接（Full Outer Join）等价于左外连接加上右外连接，同时返回左表和右表中所有的数据；对于两个表中不满足连接条件的数据返回空值。全外连接的原理如下图所示：

![图片](https://mmbiz.qpic.cn/mmbiz_png/Pn4Sm0RsAujocXlZZM2qFMbuZtEcgicDVqxeMajURk9tibyvS5Tl21ZrsNYaMg3p2860J24icCZ5VOD6Q84xysQNA/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)
		交叉连接也称为笛卡尔积（Cartesian Product)。两个表的交叉连接相当于一个表的所有行和另一个表的所有行两两组合，结果的数量为两个表的行数相乘。交叉连接的原理如下图所示：

![图片](https://mmbiz.qpic.cn/mmbiz_png/Pn4Sm0RsAujocXlZZM2qFMbuZtEcgicDVI05kZGqibjMSBScic7GGiaB9SVepENGsWvaSmicxSXTS1xxANialSoWCx5A/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

> 📝其他类型的连接还有半连接（SEMI JOIN）、反连接（ANTI JOIN）。

集合操作将两个集合合并成一个更大或更小的集合；连接查询将两个集合转换成一个更大或更小的集合，同时获得了一个更大的元素（更多的列）。很多时候集合操作都可以通过连接查询来实现，例如：

```sql
SELECT department_id  FROM departments UNIONSELECT department_id  FROM employees;
```

等价于：

```sql
SELECT COALESCE(d.department_id, e.department_id)  FROM departments d  FULL JOIN employees e ON (e.department_id = d.department_id);
```

我们已经介绍了许多查询的示例，接下来看看其他的数据操作。



### DML

DML 表示数据操作语言，也就是插入、更新和删除。

```sql
CREATE TABLE test(id int);

-- MySQL、SQL Server 等
INSERT INTO test(id) VALUES (1),(2),(3);

-- Oracle
INSERT INTO test(id)
(SELECT 1 AS id FROM DUAL
UNION ALL
SELECT 2 FROM DUAL
UNION ALL
SELECT 3 FROM DUAL);
```

我们通过一个 INSERT 语句插入了 3 条记录，或者说是插入了一个包含 3 条记录的关系表。因为，UNION ALL 返回的是一个关系表。VALUES 同样是指定了一个关系表，在 SQL Server 和 PostgreSQL 中支持以下语句：

```sql
SELECT *
FROM (
  VALUES(1),(2),(3)
) test(id);
```

前面我们已经说过，FROM 之后是一个关系表，所以这里的 VALUES 也是一样。由于我们经常插入单条记录，并没有意识到实际上是以表为单位进行操作。

同样，UPDATE 和 DELETE 语句也都是以关系表为单位的操作；只不过我们习惯了说更新一行数据或者删除几条记录。