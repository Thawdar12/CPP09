#pragma once

#include <string>
#include <map>
#include <exception>

class BitcoinExchange
{
    private:
        std::map<std::string, double> _database;

        void loadDatabase(const std::string& filename);
        bool validateDate(const std::string& date);
        bool validateValue(const double value);
        
    public:
        BitcoinExchange();
        BitcoinExchange(const std::string& fileName);
        BitcoinExchange(const BitcoinExchange& other);
        BitcoinExchange& operator=(const BitcoinExchange& other);
        ~BitcoinExchange();

        void processInputFile(const std::string& filename);

        class Exception : public std::exception
        {
            private:
                const std::string msg;
            public:
                Exception(const std::string& message) : msg(message) {}
                ~Exception() throw();

                const char* what() const throw()
                {
                    return msg.c_str();
                }
        };
};
