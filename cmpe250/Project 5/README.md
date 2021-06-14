# Cmpe 250 Assignment 5

* **Deadline:** 22.12.2019 Sunday @23:00, very strict.
* For your questions on the project, please contact with your student teaching assistant Ahmet Gedemenli via [afgedemenli@gmail.com](mailto:afgedemenli@gmail.com).

## Description

Since Ayşe likes chocolate very much, she wants to buy and stock up as many chocolates as possible. However, her mother does not allow her to buy more than *M* chocolates per day. As a little girl, she does not have a big budget, so she wants to buy chocolates as cheap as possible. Unfortunately, chocolate is not something easily reachable where she lives. Because of the famine and black market of chocolates, prices are increasing significantly each day. Ayşe wants to know the minimum possible price she needs to pay to buy K chocolates without breaking her mother’s rule.

There are *N* chocolates available on the black market of chocolate. Each chocolate has a different price and the prices are increasing every day with the formula Pn,i = i * Cn, where Pn,i is the price of chocolate n at day i, and Cn is the price of chocolate n at day 1. For instance, if the price a chocolate is 5 on the first day, it will be 10, 15, 20, … and so on, on the second, third and the other days. She wants to buy the chocolates quickly, but her mother’s rule of buying maximum M chocolates per day limits her. Ayşe needs your help to find the minimum cost of buying K chocolates, for each K from 1 to N.

## Input

In the first row of the input, there are 2 numbers. ```(N, M)``` 
- First number specifies the number of chocolates available.
- Second number M specifies the maximum number of chocolates Ayşe can buy in one day because of her mother’s rule.

In the second row of the input, there are *N* numbers, prices of the chocolates.

## Output

You should output N numbers, the minimum amounts that Ayşe needs to pay in order to buy K chocolates, for K=1, K=2, ... K=N respectively.


## Constraits

N (number of chocolates available) <= 200000

M (maximum number of chocolates Ayşe can buy each day) <= 200000

M<=N

P_i <= 200000 (Price for each chocolate)

**Important note: Please be aware of that your calculations in the solution, and the answers, may exceed 32 bits.**

## Hint

You need to find an efficient algorithm to pass the **one second** runtime constraint on large inputs. 

To find an efficient algorithm, you may consider using **dynamic programming**.  You can start from a less efficient trivial algorithm, analyze it to find subproblems that the algorithm is recalculating, and try to store the answers to these subproblems to prevent recalculation.

A large input will be given for you to test your code against time limit.

## Sample Input 1

```
9 2
6 19 3 4 4 2 6 7 8
```

## Sample Output 1

```
2 5 11 18 30 43 62 83 121
```

## Sample Input 2

```
1 1
7
```

## Sample Output 2

```
7
```

## Submission Policy
You are supposed to use the Git system provided to you for all projects. No other means of submission will be accepted. Also, pay attention to the following points:

* Your code is expected to produce the correct output in **one second**. Any test case that lasts more than one second will be counted as wrong. 
* All source codes are checked automatically for similarity with other submissions and exercises from previous years. Make sure you write and submit your own code. Plagiarism and any other forms of cheating will have serious consequences, including getting “-100” as the project score and failing the course.
* Make sure you document your code with simple inline comments and use meaningful variable names. Do not over-comment, or make your variable names unnecessarily long.
* If you want to make any changes to your project, you should do it before the submission system closes. Your changes will not be accepted by e-mail. Connectivity problems to the Internet or to GitHub in the last few minutes are not valid excuses for being unable to submit. You should not risk leaving your submission to the last few minutes. After uploading to GitHub, check to make sure that your project appears there.
* We will test your code with different input combinations, so we recommend you to test your code with different input combinations (written by you) as well.
* Deadline is 22 December 2019, 23:00. 

**Good Luck and Happy Coding!**
