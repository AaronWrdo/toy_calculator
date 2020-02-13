# toy_calculator 玩具计算器
《C++程序设计原理与实践》练习，基于文法分析的计算器，用C++实现。

## 文法定义
```
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

Number:
    floating-point-literal
```

## 实现
通过四个函数来实现文法分析器。
