// ArithmeticSolve.cpp : Defines the entry point for the console application.
//Without Magdalina Todorova's textbook

#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <fstream>
#include "stack.h"
using namespace std;

struct Element
{
	int number;
	bool isNumber;
	Element(int num, bool isNum) : number(num), isNumber(isNum) {}
	Element () : number(0), isNumber(false) {}
};

//check if any char is a digit
bool isNumber(char c)
{
	return (c <= '9' && c >= '0');
}

//check if any char is an operation
bool isOperation(char c)
{
	return (c == '-' || c == '+' || c == '*' || c == '/');
}

//converting string to an integer
int toInteger(char *str, int n)
{
	int num = (int)pow(10.0, (double)(n - 1));
	int returnResult = 0;
	for (int i = 0; i < n; i++) {
		returnResult += num * (int)(*(str+i) - '0');
		num /= 10;
	}
	return returnResult;
}

//saving any stack with type Element into file
void saveToFile(Stack<Element> stack, char *fileName)
{
	ofstream write(fileName);
	Element el;
	while(stack.pop(el)) {
		if (el.isNumber) {
			if (write)
				write<<el.number<<' ';
		} else {
			if (write)
				write<<(char)el.number<<' ';
		}
	}
	write.close();
}

int calculate(Stack<Element>);

Stack<Element> toPrefix(char *expression)
{
	Stack<Element> tempStack;	//temporary stack for operations
	Stack<Element> stack;	//stack for numbers and operations
	Stack<Element> priority;
	int index = 0;	//index for the string expression
	char fileName[19] = "polishNotation.dat";	//if i decide to save it into file
	char tempNumber[100];	//here i'll contain all numbers before i convert them into int
	Element tempElement;	//used for reading
	const Element bracket(40, false);	//used for compare

	while (*(expression+index) != '\0') {

		if (*(expression+index) == '(') {

			tempStack.push(bracket);

		} else if (isNumber(*(expression+index))) {

			int i = -1;

			//reading every digit from the number and putting it
			//into an array which is going to be convert by toInteger()
			//and the int will be put into the return stack
			while (isNumber(*(expression+index))) {
				i++;
				tempNumber[i] = *(expression+index);
				index++;
			}

			stack.push(Element(toInteger(tempNumber, i + 1), true));
			index--;

		} else if (isOperation(*(expression+index))) {

			tempStack.push(Element((int)(*(expression+index)), false));
			//every operation will be put into the tempStack
			if ((*(expression+index) == '*' || *(expression+index) == '/') &&
				isNumber(*(expression+index+1))) {	
				//there is no way to be out of boundaries here
				index++;
				int i = -1;

				//reading every digit from the number and putting it
				//into an array which is going to be convert by toInteger()
				//and the int will be put into the return stack
				while (isNumber(*(expression+index))) {
					i++;
					tempNumber[i] = *(expression+index);
					index++;
				}

				stack.push(Element(toInteger(tempNumber, i + 1), true));
				index--;
				tempStack.pop(tempElement);
				stack.push(tempElement);
			}

		} else if (*(expression+index) == ')') {
		
			//if there is an closing bracket i'm pushing all operations before
			//it into the return stack
			do {
					tempStack.pop(tempElement);
					if (tempElement.isNumber != bracket.isNumber ||
						tempElement.number != bracket.number)
						stack.push(tempElement);
			} while (tempElement.isNumber != bracket.isNumber &&
				tempElement.number != bracket.number);
			
			//removing the last bracket
			//tempStack.pop(tempElement);
			tempElement.isNumber = false;
			tempElement.number = 0;

		}

		index++;
	}

	while (tempStack.pop(tempElement)) {
		if (tempElement.isNumber != bracket.isNumber ||
			tempElement.number != bracket.number)
			stack.push(tempElement);
	}

	//saving the polish notation to a file
	//saveToFile(stack, fileName);
	return stack;
}

//do any operation
int doOperation(int operandOne, int operandTwo, char operation)
{
	switch (operation) {
		case '+' : return operandOne + operandTwo;
			break;
		case '-' : return operandOne - operandTwo;
			break;
		case '*' : return operandOne * operandTwo;
			break;
		case '/' : return operandOne / operandTwo;
			break;
		default: return 0;
	}
}

int calculate(char *expression)
{
	//getting the stack
	Stack<Element> stack = toPrefix(expression);
	int returnValue;	//value that is going to be returned
	int tempValue;	//temp value, to contain the result of any operations
	Element operandOne;	//the first operand
	Element operandTwo;	//the second operand
	Stack<Element> tempStack;	//temporary stack
	Element temp;
	//the stack need to be reversed, so i'm doing it
	stack.reverse();

	//while in the stack is only the result
	while (!stack.isEmpty()) {

		//poping element from the stack
		stack.pop(temp);

		//if it's number just add it into the tempStack
		if (temp.isNumber) {
			tempStack.push(temp);

		//if it's not a number i know that there are
		//less two numbers in the tempStack so
		//i'm poping them and replacing with the calculatin result
		} else {
			tempStack.pop(operandOne);
			tempStack.pop(operandTwo);
			tempValue = doOperation(operandTwo.number, operandOne.number,
				(char)temp.number);
			tempStack.push(Element(tempValue,true));
		}
	}

	//getting the result
	tempStack.pop(temp);
	returnValue = temp.number;

	//return the result
	return returnValue;
}

int main()
{
	cout<<"2*(6+3*8/4-2) = "<<calculate("2*(6+3*8/4-2)")<<endl;
	return 0;
}