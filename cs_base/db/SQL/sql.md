# sql

---



#### 创建数据库

```sql
CREATE DATABASE database-name 
```

#### 删除数据库

```sql
drop database dbname
```

#### 备份sql server

1. 创建 备份数据的 device

   ```sql
    USE master
   
   
   
   EXEC sp_addumpdevice 'disk', 'testBack', 'c:\mssql7backup\MyNwind_1.dat'
   ```

2. 开始 备份

   ```vbscript
   BACKUP DATABASE pubs TO testBack 
   ```

#### 创建新表

```cobol
create table tabname(col1 type1 [not null] [primary key],col2 type2 [not null],..)
```

根据已有的表创建新表：

```cobol
A：create table tab_new like tab_old (使用旧表创建新表)



B：create table tab_new as select col1,col2… from tab_old definition only
```



#### 删除新表

```sql
drop table tabname 
```

#### 增加一个列

```cobol
Alter table tabname add column col type



 



注：列增加后将不能删除。DB2中列加上后数据类型也不能改变，唯一能改变的是增加varchar类型的长度。
```

#### 添加主键

```sql
Alter table tabname add primary key(col)
```

#### 删除主键

```sql
Alter table tabname drop primary key(col)
```

#### 创建索引

```scss
create [unique] index idxname on tabname(col….)
```

#### [删除索引](https://so.csdn.net/so/search?q=删除索引&spm=1001.2101.3001.7020)

```perl
drop index idxname



 



注：索引是不可更改的，想更改必须删除重新建。
```

#### 创建视图

```sql
create view viewname as select statement
```

#### 删除视图

```sql
drop view viewname
```

#### 几个简单的基本的sql语句 

```cobol
选择：select * from table1 where 范围



插入：insert into table1(field1,field2) values(value1,value2)



删除：delete from table1 where 范围



更新：update table1 set field1=value1 where 范围



查找：select * from table1 where field1 like ’%value1%’ ---like的语法很精妙，查资料!



排序：select * from table1 order by field1,field2 [desc]



总数：select count as totalcount from table1



求和：select sum(field1) as sumvalue from table1



平均：select avg(field1) as avgvalue from table1



最大：select max(field1) as maxvalue from table1



最小：select min(field1) as minvalue from table1
```

#### 使用外连接

**left （outer） join**

左外连接（左连接）：结果集几包括连接表的匹配行，也包括左连接表的所有行。

```sql
SQL: select a.a, a.b, a.c, b.c, b.d, b.f from a LEFT OUT JOIN b ON a.a = b.c
```

**right （outer） join**

右外连接(右连接)：结果集既包括连接表的匹配连接行，也包括右连接表的所有行。

**full/cross （outer） join**

全外连接：不仅包括符号连接表的匹配行，还包括两个连接表中的所有记录。

#### 分组:Group by

一张表，一旦分组 完成后，查询后只能得到组相关的信息。
**组相关的信息：（统计信息） count,sum,max,min,avg 分组的标准)**
在SQLServer中分组时：不能以text,ntext,image类型的字段作为分组依据
在selecte统计函数中的字段，不能和普通的字段放在一起；