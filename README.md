# toy_calculator 玩具计算器
基于文法分析的计算器，用C++实现。

支持对浮点数的加、减、乘、除、取余运算。

支持括号。

支持自定义变量。

教程见《[C++程序设计原理与实践](https://book.douban.com/subject/4068100/)》第6-7章：设计并实现一个简单计算器。

## 设计
文法设计如下：
```
Calculation: 
    Statement
    Calculation Statement
    Quit
    Print

Statement: //语句
    Declaration // 变量定义
    Expression // 表达式

Declaration:
    "var" Name "=" Expression";"

Expression: 
    Term 
    Expression"+"Term // 加
    Expression"-"Term // 减

Term:
    Primary 
    Term"*"Primary // 乘
    Term"/"Primary // 除
    Term"%"Primary // 求余(模)

Primary: 
    Number
    "("Expression")" // 括号
    Name // 已定义变量

Number:
    floating-point-literal
```

...to be continued