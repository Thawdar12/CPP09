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
#include <stdlib.h>
#include <iostream>
#include <cctype>
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
    if (!vaildFormat(_input))
        throw std::runtime_error("Error: invalid format");
    while (iss >> str)
    {
        std::cout << "iss: " << str << "\n" << std::endl;
        if (validNumber(str))
        {
            std::cout << "enter" << std::endl;
            const char* tmp = str.c_str();
            int num = std::atoi(tmp);
            std::cout << "num and tmp: "<< num << " " << tmp << std::endl;
            numbers.push(num);
        }
        else
        {
            if(validOperator(str))
            {
                if (numbers.size() < 2)
                    throw std::runtime_error("Error: size less than 2");
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
                throw std::runtime_error("Error: invalid expression");
        }
    }
    if (numbers.size() != 1)
        throw std::runtime_error("Error: not enough operators");
    return numbers.top();
}

bool RPN::validNumber(const std::string& input) 
{
    // int num = std::stoi(input);

    if (input.empty())
        return false;
    for (std::string::size_type i = 0; i < input.size(); i++)
    {
        if (!std::isdigit(input[i]))
        {
            std::cout << "num: " << input[i] <<"\n" << std::endl;
            return false;
        }
    }
    return true;
}

bool RPN::validOperator(const std::string& input) 
{
    if (input.empty())
        return false;
    if (input == "+" || input == "-" || input == "/" || input == "*")
    {
        std::cout << "op: " << input << "\n" << std::endl;
        return true;
    }
    return false;
}

bool RPN::vaildFormat(const std::string& input)
{
    if (input.empty())
        return false;

    std::string::size_type i = 0;

    // Skip leading spaces
    while (i < input.size() && input[i] == ' ')
        i++;

    // Only spaces
    if (i == input.size())
        return false;

    while (i < input.size())
    {
        // Token: exactly one character
        if (input[i] == ' ')
            return false;

        i++;

        // End of expression
        if (i == input.size())
            return true;

        // Exactly one space required
        if (input[i] != ' ')
            return false;

        i++;

        // Trailing spaces are allowed
        if (i == input.size())
            return true;

        // Second consecutive space
        if (input[i] == ' ')
            return false;
    }

    return true;
}

