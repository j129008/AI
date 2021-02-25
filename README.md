# How to run this program
本程式碼使用 WTFPL (Do What the Fuck You Want to Public License)


### Introduction
將布林邏輯轉換成 Conjunctive Normal Form (CNF) 的形式，再進行推論 (Resolution)，範例如下。

```
# Convert to CNF
((A imp B) and (B imp A)) # 某個邏輯敘述
(((neg A) or B) and ((neg B) or A))
((neg A or B) and (neg B or A))
(B or neg A) and (A or neg B) # CNF

rule1: B or neg A
rule2: A or neg B

```
由此可知 (B or neg A) 以及 (A or neg B) 是兩個必須滿足的規則(rule)，而這些用 or 連接的邏輯形式稱爲稱爲 Clause，方便接下來 Resolution 推論。

若是要判斷 A 是否爲真，以反證法加入條件 neg A，若合併結束爲空集合則代表 A 必須爲真。

```
# Resolution
rule1: B or neg A
rule2: A or neg B
rule3: neg A
```
以上範例無法合併爲空集合，A 不能推定爲真



### Usage
```sh
$ g++ cnf.cpp -o cnf
$ g++ res.cpp -o res
$ ./cnf [input file name] [output file name]
$ ./res [input file name] [output file name]
```

### Sample input file
由於作業需求，cnf.cpp 以及 res.cpp 的輸入格式不同，請參考以下範例檔案
* cnf.cpp - cnf1.txt cnf2.txt
* res.cpp - res.txt
