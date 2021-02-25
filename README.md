# How to run this program
本程式碼使用 WTFPL (Do What the Fuck You Want to Public License)


### Introduction
將布林邏輯轉換成 Conjunctive Normal Form (CNF) 的形式再進行推論 (Resolution)，範例如下。

```
# Convert to CNF
((A imp B) and (B imp A))
(((neg A) or B) and ((neg B) or A))
((neg A or B) and (neg B or A))
(B or neg A) and (A or neg B)
```


### Usage
```sh
$ g++ cnf.cpp -o cnf
$ g++ res.cpp -o res
$ ./cnf [input file name] [output file name]
$ ./res [input file name] [output file name]
```

### Sample input file
* cnf.cpp - cnf1.txt cnf2.txt
* res.cpp - res.txt
