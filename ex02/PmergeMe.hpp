#include <vector>
#include <deque>
#include <utility>

class PmergeMe
{
    private:
        std::vector<int> _vector;
        std::deque<int> _deque;

        std::vector<int> fordJohnsonVector(const std::vector<int>& containerVec);
        
        std::deque<int> fordJohnsonDeque(const std::deque<int>& containerDeque);

        void jacobVector(std::vector<int>& mainChain,
                            std::vector< std::pair<int, int> >& pairs,
                            bool hasLast,
                            int lastNum);

        void jacobDeque(
            std::deque<int>& mainChain,
            std::deque< std::pair<int, int> >& pairs,
            bool hasLast,
            int lastNum);

    public:
        PmergeMe();
        PmergeMe(const PmergeMe& other);
        PmergeMe& operator=(const PmergeMe& other);
        ~PmergeMe();

        void parseInput(int argc, char ** argv);

        void sortVector();
        void sortDeque();

        void printBefore(int argc, char ** argv) const;
        void printAfter() const;
};

