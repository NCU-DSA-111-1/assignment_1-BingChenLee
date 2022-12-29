# Data Structure Assignment 1

**Subject**: Neural Network in C - Use NN to learn XOR  
**Author**: 通訊二 110503518 李秉宸  

## Compile & Run

```sh
# Compile
make

# Run
./bin/main
```

## Usage 1 - Show Iteration Process
```sh
Enter your target iteration times: 20000

Do you want to show the iteration process? Enter 1 if you want, and enter 2 if you don't.
1

Iteration Times: 1       Inputs/Output: 00/1  01/1  10/1  11/1  Error: 1.972858 MSE: 0.493214
Iteration Times: 2       Inputs/Output: 00/1  01/1  10/1  11/1  Error: 1.972005 MSE: 0.493108
...
...
Iteration Times: 19999   Inputs/Output: 00/0  01/1  10/1  11/0  Error: 0.000069  MSE: 0.013418
Iteration Times: 20000   Inputs/Output: 00/0  01/1  10/1  11/0  Error: 0.000069  MSE: 0.013418

Iterate successfully...
Final MSE: 0.013418
Now, you can enter your own test inputs (2~100 bits binary string, no space in the string)

Enter input to test: 101
Output: 0

Enter input to test: 1011
Output: 1  

Enter input to test: 01110
Output: 1

Enter input to test: 010001
Output: 0
```
You can type **Ctrl+c** in the terminal to stop the file.

## Usage 2 - Hide Iteration Process
```sh
Enter your target iteration times: 20000

Do you want to show the iteration process? Enter 1 if you want, and enter 2 if you don't.
2

Iterate successfully...
Final MSE: 0.013418
Now, you can enter your own test inputs (2~100 bits binary string, no space in the string)

Enter input to test: 101
Output: 0

Enter input to test: 1011
Output: 1  

Enter input to test: 01110
Output: 1

Enter input to test: 010001
Output: 0
```
You can type **Ctrl+c** in the terminal to stop the file.

## Notice
Your input should be **a binary string with 2~100 bits**, and **there should not contain any spaces in the string**.  
Here are some wrong input examples:

```sh
Enter input to test: 1
Please enter a binary string with at least 2 bits

Enter input to test: 0
Please enter a binary string with at least 2 bits

Enter input to test: 123
Please enter a binary string

Enter input to test: 0012
Please enter a binary string
```

## Reference
**[1] Neural-Network-framework-using-Backpropogation-in-C**
> https://github.com/mayurbhole/Neural-Network-framework-using-Backpropogation-in-C.git