#pragma once

#include <stack>
#include <string>

class RPN
{
    private:
        std::string _input;

        bool validNumber(const std::string& input);
        bool validOperator(const std::string& input);
        bool vaildFormat(const std::string& input);

    public:
        RPN();
        RPN(std::string input);
        RPN(const RPN& other);
        RPN& operator=(const RPN& other);
        ~RPN();

        int calculate();
};

