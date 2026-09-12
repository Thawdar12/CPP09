#include "PmergeMe.hpp"

#include <iostream>
#include <stdexcept>
#include <ctime>

int main(int argc, char **argv)
{
    try
    {
        PmergeMe sorting;

        sorting.parseInput(argc, argv);
        sorting.printBefore(argc, argv);

        clock_t startVector = clock();
        
        sorting.sortVector();

        clock_t endVector = clock();

        clock_t startDeque = clock();

        sorting.sortDeque();

        clock_t endDeque = clock();

        double vectorTime =
            static_cast<double>(endVector - startVector)
            / CLOCKS_PER_SEC * 1000000;

        double dequeTime =
            static_cast<double>(endDeque - startDeque)
            / CLOCKS_PER_SEC * 1000000;

        sorting.printAfter();

        std::cout << "Time to process a range of "
                  << argc - 1
                  << " elements with std::vector : "
                  << vectorTime
                  << " us"
                  << std::endl;

        std::cout << "Time to process a range of "
                  << argc - 1
                  << " elements with std::deque : "
                  << dequeTime
                  << " us"
                  << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
