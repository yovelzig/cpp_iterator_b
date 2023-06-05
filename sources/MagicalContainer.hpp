#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
namespace ariel
{
    class MagicalContainer
    {
    private:
        std::vector<int> elements;
        std::vector<int*> nextPrimePtrs;

        bool static isPrime(int number) 
        {
            if (number < 2){
                return false;
            }
            for (int i = 2; i <= std::sqrt(number); ++i)
            {
                if (number % i == 0){
                    return false;
                }
            }
            return true;
        }

    public:
        void addElement(int element)
        {
            if (std::find(elements.begin(), elements.end(), element) != elements.end())
            {
                // Element already exists, do nothing
                return;
            }
            elements.push_back(element);

            std::sort(elements.begin(), elements.end()); // Sort the elements vector
            
                nextPrimePtrs.clear();
                for (size_t i = 0; i < elements.size(); i++)
                {
                    if (isPrime(elements[i]))
                    {
                        nextPrimePtrs.push_back(&(elements[i]));
                    }
                }
            
            // for (size_t i = 0; i < this->nextPrimePtrs.size(); i++)
            // {
            //     cout << *nextPrimePtrs[i] << ' ' << endl;
            // }
            
        }

        void removeElement(int element)
        {
            auto iter = std::find(elements.begin(), elements.end(), element);
            if (iter == elements.end())
            {
                throw std::runtime_error("Element not found");
            }
            if (iter != elements.end())
            {
                elements.erase(iter);
                if (isPrime(element))
                {
                    nextPrimePtrs.erase(std::remove(nextPrimePtrs.begin(), nextPrimePtrs.end(), &(*iter)), nextPrimePtrs.end());
                }
            }
        }

        int size() const
        {
            return (int)elements.size();
        }

        class AscendingIterator
        {
        private:
            MagicalContainer &container;
            size_t current;

        public:
            AscendingIterator(MagicalContainer &cont) : container(cont), current(0) {}

            bool operator==(const AscendingIterator &other) const
            {
                
                return ((this->current == other.current)  && (&(this->container) == &(other.container)));
            }

            bool operator!=(const AscendingIterator &other) const
            {
                return !(*this == other);
            }
            AscendingIterator &operator=(const AscendingIterator &other){
             if (this != &other)
                {
                    if (this->container.elements != other.container.elements)
                    {
                       throw std::runtime_error("different containers");
                    }
                    *this = AscendingIterator(other);
                }
                return *this;
            }
            bool operator>(const AscendingIterator &other) const
            {
                return current > other.current;
            }

            bool operator<(const AscendingIterator &other) const
            {
                return current < other.current;
            }

            int operator*() const
            {
                return container.elements[current];
            }

            AscendingIterator &operator++()
            {
                if (current == container.elements.size())
                {
                   throw std::runtime_error("Attempt to increment beyond the end");
                }
                ++current;
                return *this;
            }

            AscendingIterator begin()
            {
                current = 0;
                return *this;
            }

            AscendingIterator end()
            {
                current = container.elements.size();
                return *this;
            }
        };

              class SideCrossIterator
        {
        private:
            MagicalContainer &container;
            size_t forwardIndex;
            size_t reverseIndex;
            bool isForward;
            bool isEndReached;
            int iterationCount;

        public:
            SideCrossIterator(MagicalContainer &cont)
                : container(cont),
                  forwardIndex(0),
                  reverseIndex(container.elements.size() - 1),
                  isForward(true),
                  isEndReached(false),
                  iterationCount(0) {}

            bool operator==(const SideCrossIterator &other) const
            {
                return forwardIndex == other.forwardIndex && reverseIndex == other.reverseIndex;
            }

            bool operator!=(const SideCrossIterator &other) const
            {
                return !(*this == other);
            }
            SideCrossIterator &operator=(const SideCrossIterator &other){
             if (this != &other)
                {
                    if (this->container.elements != other.container.elements)
                    {
                       throw std::runtime_error("different containers");
                    }
                    *this = SideCrossIterator(other);
                }
                return *this;
            }

            bool operator>(const SideCrossIterator &other) const
            {
                return *getForwardIndex() > *other.getForwardIndex() || *getReverseIndex() > *other.getReverseIndex();
            }

            bool operator<(const SideCrossIterator &other) const
            {
                return *getForwardIndex() < *other.getForwardIndex() || *getReverseIndex() < *other.getReverseIndex();
            }

            int operator*() const
            {
                if (isForward && forwardIndex < container.elements.size()){
                    return container.elements[forwardIndex];
                }
                if (!isForward && reverseIndex < container.elements.size()){
                    return container.elements[reverseIndex];
                }
                return -1; // Return a default value to handle invalid cases
            }

            SideCrossIterator &operator++()
            {
                if (forwardIndex == container.elements.size()||reverseIndex == container.elements.size())
                {
                throw std::runtime_error("Attempt to increment beyond the end");
                }
                if (isEndReached)
                {
                   return *this;
                }
                if (isForward)
                {
                    ++forwardIndex;
                }
                else
                {
                    --reverseIndex;
                }

                isForward = !isForward;

                // Check if the iteration count has reached the vector's size
                ++iterationCount;
                if (iterationCount == container.size())
                {
                    isEndReached = true;
                    forwardIndex = container.elements.size();
                    reverseIndex = container.elements.size();
                }

                return *this;
            }

            SideCrossIterator begin()
            {
                forwardIndex = 0;
                reverseIndex = container.elements.size() - 1;
                isForward = true;
                isEndReached = false;
                iterationCount = 0;
                return *this;
            }

            SideCrossIterator end()
            {
                forwardIndex = container.elements.size();
                reverseIndex = container.elements.size();
                return *this;
            }

            bool isEnd() const
            {
                return isEndReached;
            }

        private:
            const int* getForwardIndex() const
            {
                if (forwardIndex < container.elements.size()){
                    return &(container.elements[forwardIndex]);
                }
                return nullptr;
            }

            const int* getReverseIndex() const
            {
                if (reverseIndex < container.elements.size()){
                    return &(container.elements[reverseIndex]);
                }
                return nullptr;
            }
        };


        class PrimeIterator
        {
        private:
            MagicalContainer &container;
            size_t currentIndex;

        public:
            PrimeIterator(MagicalContainer &cont) : container(cont), currentIndex(0) {}

            bool operator==(const PrimeIterator &other) const
            {
                return currentIndex == other.currentIndex;
            }

            bool operator!=(const PrimeIterator &other) const
            {
                return !(*this == other);
            }
             PrimeIterator &operator=(const PrimeIterator &other){
             if (this != &other)
                {
                    if (this->container.elements != other.container.elements)
                    {
                       throw std::runtime_error("different containers");
                    }
                    *this = PrimeIterator(other);
                }
                return *this;
            }

            bool operator>(const PrimeIterator &other) const
            {
                return *this != other && !(*this < other);
            }

            bool operator<(const PrimeIterator &other) const
            {
                if (currentIndex >= container.nextPrimePtrs.size())
                {
                    return false;
                }
                if (other.currentIndex >= container.nextPrimePtrs.size())
                {
                    return true;
                }
                return *container.nextPrimePtrs[currentIndex] < *container.nextPrimePtrs[other.currentIndex];
            }

            int operator*() const
            {
                if (currentIndex >= container.nextPrimePtrs.size())
                {
                    return -1; // Return a default value to handle invalid cases
                }
                return *container.nextPrimePtrs[currentIndex];
            }

            PrimeIterator &operator++()
            {
                 if (container.nextPrimePtrs.empty())
                {
                   throw std::runtime_error("Attempt to increment empty container");
                }
                if (currentIndex == container.nextPrimePtrs.size())
                {
                   throw std::runtime_error("Attempt to increment beyond the end");
                }
                ++currentIndex;
                return *this;
            }

            PrimeIterator begin()
            {
                currentIndex = 0;
                return *this;
            }

            PrimeIterator end()
            {
                currentIndex = container.nextPrimePtrs.size();
                return *this;
            }
        };
    };
}

