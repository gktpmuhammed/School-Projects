# Cmpe 250 Assignment 2

* **Deadline:** 29.10.2019 Tuesday @23:00, very strict.
* For your questions on the project,
please contact with your student teaching assistant
Ahmet Gedemenli via [afgedemenli@gmail.com](mailto:afgedemenli@gmail.com).

## Description
The restaurant we had in the previous assignment has become a larger enterprise. Now it has tables with capacity more than one and customer groups with size more than one. Because of that, the restaurant is having some trouble with organization issues. You are expected to assign each incoming customer group to a table by considering the following constraints:

* This restaurant has *n* number of tables, where *n* is an integer from 3 to 100.000 (3 <= *n* <= 10^5).
* Tables are named from 0 to *n-1*. The smallest table number is 0.
* Each table can have a different capacity, let's say *c_i*, such that 1 <= *c_i* <= 100000.
* Initially all tables are empty.
* There will be q customer groups in the input, arriving respectively, where (3 <= q <= 10^5).
* Customers might arrive as groups or single. Group sizes can vary between 1 to 100000.
* When a group is arrived, regardless of the group size, it will be assigned to the table with current **maximum remaining capacity**. Your program should output the assigned table number. Note that different groups can be assigned to the same table.
* You do not have to think about the leaving groups. There will be no leave until the last group arrives.
* If the arrived group is too large to assign, unfortunately the restaurant cannot accept them. (i.e if a group arrives with a size larger than the maximum remaining capacity, ignore them.) In such case, you are expected to print **"-1" (without quotes)** as their assigned table number.
* If there are more than one table with the same remaining capacity, the group must be assigned to the one with the **smallest table number**.
* In the end, we need to know the final state of our restaurant. You are expected to print the remaining capacities of all tables in descending order.

To implement this project, you will be provided some code written by the assistants, and you are going to implement the missing part. 

The files you will be provided are: Restaurant.h, Restaurant.cpp, and main.cpp.

The file you should fill in is: Restaurant.cpp. You only need to complete the required functions, which are commented. So, for this project any code written outside Restaurant.cpp will be useless.


## Input
You don't need to read the input. The already implemented parts of the project reads the input. The functions you implement will be called with corresponding parameters.
The input is formatted as follows:

- First line will be the number of tables, *n*.
- Second line contains the sizes for each table, separated by a space.
- Third line will be the number of groups to arrive, *q*.
- Fourth line contains the sizes for each group, separated by a space.

## Output
You are expected to produce an output consisting of **exactly** *q+1* lines.

* First *q* lines must be the assigned table numbers of incoming *q* groups, respectively. You should do this in **addNewGroup** function.
* The last line must be the sorted list of remaining capacities of tables, in descending order, separated by a **single space character**. You should do this in **printSorted** function.
* You need to print the output to a file. Output file is already created for you, and you need to use provided ostream operator to write to it. (eg. `outfile << 'test';` command will write 'test' to the output file.) 

## Sample Input 1

```
3
10 10 10
7
3 5 7 1 1 1 1
```

## Sample Output 1

```
0
1
2
0
0
0
1
4 4 3
```

## Sample Input 2

```
3
5 5 5
7
3 5 7 1 1 1 1
```

## Sample Output 2

```
0
1
-1
2
2
2
0
2 1 0
```

## Important notes
* Remember what you learned in the class. Try to implement as efficient as possible. 
* Your code will be tested with large inputs (n=100000, q=100000) and is expected to produce the correct output in **3 seconds**. Any test case that lasts more than three seconds will be counted as wrong. Most of the test cases will be large, you will get a low score if you don't implement proper heap functions.
* We will provide you an example large input (input3.txt), for you to test your code's runtime. We will also provide you its correct output (output3.txt) for you to test the output of your code against it.
* The examples we provide you will not be used in grading your projects!
* You do not need additional memory. We will run your code in a memory-restricted sandbox, so don't make large allocations.
* Do not use a library other than we provided, which are already defined in the code.
* **Do not include any extra libraries!** Such lines will be removed from your code before the evaluation. Excuses like *'My IDE added this library, I was not aware'* will not be accepted. You are responsible form your IDE, therefore please be aware.

## Submission Policy
You are supposed to use the Git system provided to you for all projects. No other means of submission will be accepted. Also pay attention to the following points:

* All source codes are checked automatically for similarity with other submissions and exercises from previous years. Make sure you write and submit your own code. Plagiarism and any other forms of cheating will have serious consequences, including getting “-100” as the project score and failing the course.
* Make sure you document your code with necessary inline comments, and use meaningful variable names. Do not over-comment, or make your variable names unnecessarily long.
* If you want to make any changes on your project, you should do it before submission system closes. Your changes will not be accepted by e-mail. Connectivity problems to the Internet or to GitHub in the last few minutes are not valid excuses for being unable to submit. You should not risk leaving your submission to the last few minutes. After uploading to GitHub, check to make sure that your project appears there.
* We will test your code with different input combinations, so we recommend you to test your code with different input combinations (written by you) as well.
* Deadline is 29 October 2019, 23:00. 

**Good luck!**
