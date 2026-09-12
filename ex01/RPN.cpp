#include "RPN.hpp"
#include <sstream>
#include <stack>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <stdexcept>

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
        if (validNumber(str))
        {
            int num = std::atoi(str.c_str());
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
                        if (right == 0)
                            throw std::runtime_error("Error: division by zero");
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
    if (input.empty())
        return false;
    for (std::string::size_type i = 0; i < input.size(); i++)
    {
        if (!std::isdigit(input[i]))
            return false;
    }
    return true;
}

bool RPN::validOperator(const std::string& input) 
{
    if (input.empty())
        return false;
    if (input == "+" || input == "-" || input == "/" || input == "*")
        return true;
    return false;
}

bool RPN::vaildFormat(const std::string& input)
{
    if (input.empty())
        return false;

    std::string::size_type i = 0;

    while (i < input.size() && input[i] == ' ')
        i++;

    if (i == input.size())
        return false;

    while (i < input.size())
    {
        //exactly one character
        if (input[i] == ' ')
            return false;

        i++;

        // end of input
        if (i == input.size())
            return true;

        // exactly one space required
        if (input[i] != ' ')
            return false;

        i++;

        // Trailing spaces are allowed
        if (i == input.size())
            return true;

        // Second space
        if (input[i] == ' ')
            return false;
    }

    return true;
}

