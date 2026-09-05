/*
read token
↓
is it a valid number?
    ↓ yes → push onto stack
    ↓ no
is it a valid operator?
    ↓ yes → check stack has 2 operands
         ↓
              calculate
                 ↓
              push result
    ↓ no
error
↓
finished?
↓
stack must contain exactly 1 result
*/

#include "RPN.hpp"
#include <sstream>
#include <stack>
#include <string>
// std::stack<int> _numbers;

RPN::RPN() : _input() {}

RPN::RPN(std::string str) : _input(str) {}

RPN::RPN(const RPN& other) : _input(other._input) {}

RPN& RPN::operator=(const RPN& other) 
{
    if (this != &other)
        _input = other._input;
    return *this;
}

RPN::~RPN() {}

int RPN::calculate() 
{
    std::stack<int> numbers;
    std::istringstream iss(_input);
    std::string str;
    
    while (iss >> str)
    {
        if (validNumber(str))
        {
            int num = std::stoi(str);
            numbers.push(num);
        }
        else
        {
            if(validOperator(str))
            {
                if (numbers.size() < 2)
                throw std::runtime_error("Error\n");
                char op = str[0];
                int right = numbers.top();
                numbers.pop();
                
                int left = numbers.top();
                numbers.pop();

                int result;

                switch (op)
                {
                    case '+':
                    {
                        result = left + right;
                        break;
                    }
                    case '-':
                    {
                        result = left - right;
                        break;
                    }
                    case '/':
                    {
                        result = left / right;
                        break;
                    }
                    case '*':
                    {
                        result = left * right;
                        break;
                    }
                    default:
                        break;
                }
                numbers.push(result);
            }
            else
                throw std::runtime_error("Error\n");
        }
    }
    if (numbers.size() != 1)
        throw std::runtime_error("Error\n");
    return numbers.top();
}

bool RPN::validNumber(const std::string& input) 
{

}

bool RPN::validOperator(const std::string& input) 
{

}

