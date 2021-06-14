# Cmpe 250 Assignment 3

* **Deadline:** 16.11.2019 Saturday @23:00, very strict.
* For your questions on the project, please contact with your student teaching assistant Kayacan Vesek via [kayacanvesek@gmail.com](mailto:kayacanvesek@gmail.com).

## Description
Cmpe Airlines is an airline company that aims to provide highest quality services for their customers. They are planning to change their customer assignment policy, and you are expected to build a system that assigns customers to the airplane seats with desired constraints.

Cmpe Airlines' planes have 2 lines of seats named as Line A and Line B. The lines have different sizes: the size of the first line (A) is N and the size of the second line (B) is M. This design allows everyone to have a window seat.


Every person is assigned a ticket and when the person enters the plane he/she goes to the seat on his/her ticket. 

* The seat number may exceed the size of the lines, so people sits to the place according to modulo of that line's size. Let's say the seat number is X, if the seat is in the first line (A column) he/she sits X % N, if the seat is in the second line (B column) he/she sits X % M. 


If the seat is full, he/she causes the sitting person move, and he/she sits there. The displaced person needs to find a new place, there are 3 different types of people who make a different move when they need to find a new place:

* The **first type of person** goes the other seat line and sits other seat in modulo of that line's size. (For example, if at first he/she sits X % N in the Line A, than he/she goes to X%M in the Line B).
* The **second type of person** moves to one back seat. If he/she was on final seat, he/she goes to the first seat of the **other line!**
* The **third type of person** moves to the seat back at the first time, to the seat that is 4 behind the original seat at the second time, to the seat that is 9 behind the original seat at third time, and so on... (For example if he/she sits originally at A4 and got moved 4 times, he/she sits in the seats: `A4 -> A5 -> A8 -> A13 -> A20`). If the seat number exceeds the line size he/she goes the head of the other line and continues to go back. (For example N=10 and a person sits in 8 and needs to go 7 seats back. He has to go seat 5 in the Line B, if that line does not have seat 5, eg. for M=3, he/she changes line again, and needs to sit the second seat in the Line A). 

**NOTE:** When a person goes to the new place always he/she sits that place and cause to move previous person.

To implement this project, you will be provided some code written by the assistants, which handles the reading input and output files. 

The files you will be provided are SeatOperations.h, SeatOperations.cpp, and main.cpp.

The file you should fill is: SeatOperations.cpp. You are also allowed to change the content of SeatOperations.h. So, for this project **any code written outside SeatOperations.cpp and SeatOperations.h will be useless.**

## Input
You don't need to read the input. The already implemented parts of the project reads the input. The functions you implement will be called with corresponding parameters.
The input is formatted as follows:

- First row is the sizes of the Line A and Line B, and the number of queries: *N* *M* *Q*
- The next Q rows have queries, each query describing a new person coming into plane. Formatted as two properties seperated by space:
    - First, is the **type of the incoming person**
    - Second, is the **ticket information**, described as line name (A or B) with seat number.
    - Example: `3 A12` means type 3 person coming to the Line A, seat 12.

**NOTE:** The seat number can be greater than N and M, Lets say seat number is X, when a person occured in this situation he/she goes the seat number X % N if it is in the Line A, X % M if it is in the Line B (seat number 0 equals to N).

## Output

You are expected to produce an output consisting of **exactly** N+M rows, describing the state of the seats on the plane.
* First N rows is for **Line A**, contains the real ticket of information of the people if seat is full, and "0" if seat is empty. 
* Next M rows is for **Line B**, contains the real ticket of information of the people if seat is full, and "0" if seat is empty. 


You need to print the output to a file in **printAllSeats()** function. Output file is already created for you, and you need to use provided ostream operator to write to it. (eg. `outfile << 'test';` command will write 'test' to the output file.) 
 

## Sample Input 1


```
5 3 4
3 A1
2 A1
1 A1
1 A2
```

## Sample Output 1

```
1 A1
1 A2
2 A1
0
3 A1
0
0
0

```
### Output Explanation
```
Initially:
    A Line- 0 0 0 0 0 
    B Line- 0 0 0

3 A1 arrives:
    A Line- (3 A1) 0 0 0 0 
    B Line- 0 0 0

2 A1 arrives:
    A Line- (2 A1) (3 A1) 0 0 0 
    B Line- 0 0 0

1 A1 arrives:
    A Line- (1 A1) (2 A1) 0 0 (3 A1) 
    B Line- 0 0 0

1 A2 arrives:
    A Line- (1 A1) (1 A2) (2 A1) 0 (3 A1) 
    B Line- 0 0 0
```



## Sample Input 2

```
3 3 6
3 A1
2 A1
1 A1
3 A2
2 A2
1 A2
```

## Sample Output 2

```
1 A1
1 A2
2 A2
2 A1
3 A1
3 A2

```
## Limits
* N, M <= 1000
* Q <= N + M
* X_i (The ticket seat number) <= 100000

## Important notes

* Your code will be compiled with `cmake CMakeLists.txt && make` command. You are expected to make sure your code runs in the provided virtual machine environment.
* Your code will be tested with large inputs (N,M=1000, Q=2000) and is expected to produce the correct output in **3 seconds**. Any test case that lasts more than three seconds will be counted as wrong. It is guaranteed that the infinite loop case will never be occurred.
* We will provide you an example large input (input3.txt), for you to test your code's runtime. We will also provide you its correct output (output3.txt) for you to test the output of your code against it.
* You do not need to use any libraries other than provided. So you **should not** import any libraries other than we provided.
* Half of the test cases does not include the third type of person, so you can get the half grade if you did not implement that part.
* The examples we provide you will not be used in grading your projects!


## Submission Policy
You are supposed to use the Git system provided to you for all projects. No other means of submission will be accepted. Also, pay attention to the following points:

* All source codes are checked automatically for similarity with other submissions and exercises from previous years. Make sure you write and submit your own code. Plagiarism and any other forms of cheating will have serious consequences, including getting “-100” as the project score and failing the course.
* Make sure you document your code with simple inline comments and use meaningful variable names. Do not over-comment, or make your variable names unnecessarily long.
* If you want to make any changes to your project, you should do it before the submission system closes. Your changes will not be accepted by e-mail. Connectivity problems to the Internet or to GitHub in the last few minutes are not valid excuses for being unable to submit. You should not risk leaving your submission to the last few minutes. After uploading to GitHub, check to make sure that your project appears there.
* We will test your code with different input combinations, so we recommend you to test your code with different input combinations (written by you) as well.
* Deadline is 16 November 2019, 23:00. 

**Good luck!**
