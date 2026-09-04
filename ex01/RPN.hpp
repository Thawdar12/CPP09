#pragma once

#include <stack>
#include <string>

class RPN
{
    private:
        std::stack<int> _numbers;

    public:
        RPN();
        RPN(std::string& input);
        RPN(const RPN& other);
        RPN& operator=(const RPN& other);
        ~RPN();

};
