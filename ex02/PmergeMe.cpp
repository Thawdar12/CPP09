#include "PmergeMe.hpp"
#include <stdexcept>
#include <climits>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <utility>

PmergeMe::PmergeMe() : _vector(), _deque() {}

PmergeMe::PmergeMe(const PmergeMe& other)
    : _vector(other._vector), _deque(other._deque) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
    if (this != &other)
    {
        _vector = other._vector;
        _deque = other._deque;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}


void PmergeMe::parseInput(int argc, char **argv)
{
    if (argc < 2)
        throw std::runtime_error("Error: Wrong num of arg");

    for(int k = 1; k < argc; k++)
    {
        long number = 0;
        int i = 0;

        if (!argv[k][0])
            throw std::runtime_error("Error: Empty");

        while (argv[k][i])
        {
            if (!std::isdigit(static_cast<unsigned char>(argv[k][i])))
                throw std::runtime_error("Error: not digit");

            number = number * 10 + (argv[k][i] - '0');

            if (number > INT_MAX)
                throw std::runtime_error("Error: over int max");

            i++;
        }

        if (number <= 0)
            throw std::runtime_error("Error: 0 or negative");

        _vector.push_back(static_cast<int>(number));
        _deque.push_back(static_cast<int>(number));
    }
}


void PmergeMe::jacobVector(std::vector<int>& mainChain,
    std::vector< std::pair<int, int> >& pairs, bool hasLast, int lastNum)
{
    if(!pairs.empty())
        mainChain.insert(mainChain.begin(), pairs[0].first);

    std::size_t pairCount = pairs.size();
    std::size_t inserted = 1;
    std::size_t jacobPrev = 1;
    std::size_t jacobCurr = 3;

    while(inserted < pairCount)
    {
        std::size_t end = jacobCurr;

        if(end > pairCount)
            end = pairCount;

        std::size_t i = end;

        while(i > inserted)
        {
            --i;

            int value = pairs[i].first;
            int largeValue = pairs[i].second;

            std::vector<int>::iterator upper;

            upper = std::find(
                mainChain.begin(),
                mainChain.end(),
                largeValue
            );

            std::vector<int>::iterator pos;

            pos = std::lower_bound(
                mainChain.begin(),
                upper,
                value
            );

            mainChain.insert(pos, value);
        }

        inserted = end;

        std::size_t next = jacobCurr + 2 * jacobPrev;

        jacobPrev = jacobCurr;
        jacobCurr = next;
    }

    if(hasLast)
    {
        std::vector<int>::iterator pos;

        pos = std::lower_bound(
            mainChain.begin(),
            mainChain.end(),
            lastNum
        );

        mainChain.insert(pos, lastNum);
    }
}

std::vector<int> PmergeMe::fordJohnsonVector(
    const std::vector<int>& containerVec)
{
    if(containerVec.size() <= 1)
        return containerVec;

    std::vector< std::pair<int, int> > pairs;
    std::vector<int> largePair;
    int last_num = 0;
    bool hasLast = false;

    for(std::size_t i = 0; i + 1 < containerVec.size(); i += 2)
    {
        std::pair<int, int> newPair;

        newPair.first = containerVec[i];
        newPair.second = containerVec[i + 1];

        if(newPair.first > newPair.second)
            std::swap(newPair.first, newPair.second);

        pairs.push_back(newPair);
    }

    if(containerVec.size() % 2 != 0)
    {
        last_num = containerVec[containerVec.size() - 1];
        hasLast = true;
    }

    for(std::size_t i = 0; i < pairs.size(); i++)
        largePair.push_back(pairs[i].second);

    std::vector<int> sortedLarge = fordJohnsonVector(largePair);

    // reorder pairs so each small value
    // attached to its corresponding large value
    std::vector< std::pair<int, int> > sortedPairs;
    std::vector<bool> used(pairs.size(), false);

    for(std::size_t i = 0; i < sortedLarge.size(); i++)
    {
        for(std::size_t j = 0; j < pairs.size(); j++)
        {
            if(!used[j] && pairs[j].second == sortedLarge[i])
            {
                sortedPairs.push_back(pairs[j]);
                used[j] = true;
                break;
            }
        }
    }

    jacobVector(sortedLarge, sortedPairs, hasLast, last_num);

    return sortedLarge;
}

void PmergeMe::sortVector()
{
    _vector = fordJohnsonVector(_vector);
}


void PmergeMe::jacobDeque(
    std::deque<int>& mainChain,
    std::deque< std::pair<int, int> >& pairs,
    bool hasLast,
    int lastNum)
{
    if(!pairs.empty())
    {
        mainChain.insert(
            mainChain.begin(),
            pairs[0].first
        );
    }

    std::size_t pairCount = pairs.size();
    std::size_t inserted = 1;
    std::size_t jacobPrev = 1;
    std::size_t jacobCurr = 3;

    while(inserted < pairCount)
    {
        std::size_t end = jacobCurr;

        if(end > pairCount)
            end = pairCount;

        std::size_t i = end;

        while(i > inserted)
        {
            --i;

            int value = pairs[i].first;
            int largeValue = pairs[i].second;

            std::deque<int>::iterator upper;

            upper = std::find(
                mainChain.begin(),
                mainChain.end(),
                largeValue
            );

            std::deque<int>::iterator pos;

            pos = std::lower_bound(
                mainChain.begin(),
                upper,
                value
            );

            mainChain.insert(pos, value);
        }

        inserted = end;

        std::size_t next = jacobCurr + 2 * jacobPrev;

        jacobPrev = jacobCurr;
        jacobCurr = next;
    }

    if(hasLast)
    {
        std::deque<int>::iterator pos;

        pos = std::lower_bound(
            mainChain.begin(),
            mainChain.end(),
            lastNum
        );

        mainChain.insert(pos, lastNum);
    }
}

std::deque<int> PmergeMe::fordJohnsonDeque(
    const std::deque<int>& containerDeque)
{
    if(containerDeque.size() <= 1)
        return containerDeque;

    std::deque< std::pair<int, int> > pairs;
    int last_num = 0;
    bool hasLast = false;

    for(std::size_t i = 0;
        i + 1 < containerDeque.size();
        i += 2)
    {
        std::pair<int, int> newPair;

        newPair.first = containerDeque[i];
        newPair.second = containerDeque[i + 1];

        if(newPair.first > newPair.second)
            std::swap(newPair.first, newPair.second);

        pairs.push_back(newPair);
    }

    if(containerDeque.size() % 2 != 0)
    {
        last_num = containerDeque[containerDeque.size() - 1];
        hasLast = true;
    }

    std::deque<int> largePair;

    for(std::size_t i = 0; i < pairs.size(); i++)
        largePair.push_back(pairs[i].second);


    std::deque<int> sortedLarge =
        fordJohnsonDeque(largePair);

    // Reorder the pairs so that each small value
    // stays attached to its corresponding large value.
    
    std::deque< std::pair<int, int> > sortedPairs;
    std::vector<bool> used(pairs.size(), false);

    for(std::size_t i = 0; i < sortedLarge.size(); i++)
    {
        for(std::size_t j = 0; j < pairs.size(); j++)
        {
            if(!used[j] && pairs[j].second == sortedLarge[i])
            {
                sortedPairs.push_back(pairs[j]);
                used[j] = true;
                break;
            }
        }
    }

    jacobDeque(
        sortedLarge,
        sortedPairs,
        hasLast,
        last_num
    );

    return sortedLarge;
}

void PmergeMe::sortDeque()
{
    _deque = fordJohnsonDeque(_deque);
}


void PmergeMe::printBefore(int argc, char **argv) const
{
    std::cout << "Before: ";

    for(int i = 1; i < argc; i++)
        std::cout << argv[i] << " ";

    std::cout << std::endl;
}

void PmergeMe::printAfter() const
{
    std::cout << "After: ";

    for(std::size_t i = 0; i < _vector.size(); i++)
        std::cout << _vector[i] << " ";

    std::cout << std::endl;
}

