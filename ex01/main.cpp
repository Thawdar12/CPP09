#include "RPN.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    try
    {
        RPN rpn(argv[1]);

        int result = rpn.calculate();
        std::cout << result << std::endl;
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
