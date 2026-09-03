#include "BitcoinExchange.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sstream>

void BitcoinExchange::loadDatabase(const std::string& filename)
{
    std::ifstream infile(filename.c_str());
    if(!infile)
        throw Exception("Error: could not open file.");

    std::string line;
    std::string date;
    std::string rate;
    double changedRate;
    std::getline(infile,line);

    while(std::getline(infile, line))
    {
        std::string::size_type pos = 0;
        if((pos = line.find(",", pos)) != std::string::npos)
        {
            date = line.substr(0, pos);
            rate = line.substr(pos + 1);
            changedRate = std::atof(rate.c_str());
            _database.insert(std::make_pair(date, changedRate));
        }
    }
    
    // testing
    // for(std::map<std::string, double>::iterator it = _database.begin(); it != _database.end(); it++)
    // {
    //     std::cout << it->first << " => " << std::fixed << std::setprecision(2) << it->second << '\n';
    // }
}

BitcoinExchange::BitcoinExchange() : _database() {}

BitcoinExchange::BitcoinExchange(const std::string& filename)
{
    loadDatabase(filename);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
    : _database(other._database) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
    if(this != &other)
        _database = other._database;
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

bool BitcoinExchange::validateDate(const std::string& date)
{
    if(date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;
    for(std::string::size_type i = 0; i < date.size(); i++)
    {
        if(i != 4 && i != 7)
        {
            if(!std::isdigit(date[i]))
                return false;
        }
    }
    std::string yearstr = date.substr(0, 4);
    std::string monthstr = date.substr(5, 2);
    std::string daystr = date.substr(8, 2);

    int year = std::atoi(yearstr.c_str());
    int month = std::atoi(monthstr.c_str());
    int day = std::atoi(daystr.c_str());

    if (month <= 0 || month > 12)
        return false;

    // 1,3,5,7,8,10,12 -> 31
    // 4,6,9,11 -> 30
    // 2 -> 28/29
    if (month == 2)
    {
        if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
        {
            if(day <= 0 || day > 29)
                return false;
        } else
        {
            if(day <= 0 || day > 28)
                return false;
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if(day <= 0 || day > 30)
            return false;
    } else
    {
        if (day <= 0 || day > 31)
            return false;
    }
    return true;
}

bool BitcoinExchange::validateValue(const double value)
{
    if(value < 0)
    {
        std::cout << "Error: not a positive number." << std::endl;
        return false;
    }
    else if(value > 1000)
    {
        std::cout << "Error: too large a number." << std::endl;
        return false;
    }
    return true;
}

BitcoinExchange::Exception::~Exception() throw() {}

void BitcoinExchange::processInputFile(const std::string& filename)
{
    std::ifstream infile(filename.c_str());
    if (!infile)
        throw Exception("Error: could not open file.");

    std::string line;
    std::getline(infile, line);

    while (std::getline(infile, line))
    {
        std::string::size_type pos = line.find("|");

        if (pos == std::string::npos)
        {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }

        std::string date = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        std::string::size_type start = date.find_first_not_of(" \t\r");
        std::string::size_type end = date.find_last_not_of(" \t\r");

        if (start != std::string::npos)
            date = date.substr(start, end - start + 1);

        start = value.find_first_not_of(" \t\r");
        end = value.find_last_not_of(" \t\r");

        if (start != std::string::npos)
            value = value.substr(start, end - start + 1);

        std::istringstream valueStream(value);
        double changedValue;
        if(!(valueStream >> changedValue) || !valueStream.eof())
        {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }

        if (!validateDate(date))
        {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }

        if (!validateValue(changedValue))
            continue;

        std::map<std::string, double>::iterator it = _database.lower_bound(date);

        if (it == _database.end())
        {
            --it;
        }
        else if (it->first != date)
        {
            if (it == _database.begin())
            {
                std::cout << "Error: date is before database range." << std::endl;
                continue;
            }
            --it;
        }

        double rate = it->second;
        double result = changedValue * rate;

        std::cout << date << " => " << changedValue
                  << " = " << result << std::endl;
    }
}
