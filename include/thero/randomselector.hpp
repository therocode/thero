#pragma once
#include <deque>
#include <random>
#include <thero/assert.hpp>

namespace th
{
template <typename Type, typename Generator = std::mt19937>
class RandomSelector
{
    public:
        RandomSelector(std::deque<std::pair<Type, float>> weightedItems, uint64_t seed):
            mGenerator(seed),
            mItems(weightedItems)
        {
            calculateTotalWeight();
        };

        RandomSelector(std::deque<Type> items, uint64_t seed):
            RandomSelector([] (const std::deque<Type>& _items)
            {
                std::deque<std::pair<Type, float>> weightedItems;
                
                for(const auto& item : _items)
                    weightedItems.push_back({item, 1.0f});

                return weightedItems ;
            }(items), seed)
        {

        };

        RandomSelector(std::deque<std::pair<Type, float>> items):
            RandomSelector(items, [] () { return std::random_device()();}() )
        {
        };

        RandomSelector(std::deque<Type> items):
            RandomSelector(items, [] () { return std::random_device()();}() )
        {
        };

        const Type& get() const
        {
            FEA_ASSERT(!mItems.empty(), "Trying to use a RandomSelector object with no items to select from");

            return retreiveRandomItemIterator()->first;
        }

        Type pop()
        {
            FEA_ASSERT(!mItems.empty(), "Trying to pop a RandomSelector object with no items to select from");

            auto valueIterator = retreiveRandomItemIterator();
            Type toReturn = valueIterator->first;
            mItems.erase(valueIterator);
            calculateTotalWeight();
            return toReturn;
        }

        void add(Type value, float weight = 1.0f)
        {
            FEA_ASSERT(weight >= 0.0f, "Cannot give a negative weight");
            mItems.push_back({std::move(value), weight});
            calculateTotalWeight();
        }

        RandomSelector<Type> combineWith(const RandomSelector<Type>& other)
        {
            return combineWith(other, mGenerator());
        }

        RandomSelector<Type> combineWith(const RandomSelector<Type>& other, uint64_t seed)
        {
            auto combinedItems = mItems;

            combinedItems.insert(combinedItems.end(), other.mItems.begin(), other.mItems.end());

            return RandomSelector<Type>(combinedItems, seed);
        }

        bool empty() const
        {
            return mItems.empty();
        }
    private:
        void calculateTotalWeight()
        {
            float weightSum = 0.0f;

            for(const auto& item : mItems)
            {
                FEA_ASSERT(item.second >= 0.0f, "Cannot give a negative weight");
                weightSum += item.second;
            }

            mWeightDistribution = std::uniform_real_distribution<float>(0.0f, weightSum);
        }
        typename std::deque<std::pair<Type, float>>::const_iterator retreiveRandomItemIterator() const
        {
            float weightIndex = mWeightDistribution(mGenerator);

            float accumulator = 0.0f;

            for(auto iter = mItems.cbegin(); iter != mItems.cend(); iter++)
            {
                accumulator += iter->second;
                if(accumulator >= weightIndex)
                    return iter;
            }

            FEA_ASSERT(false, "Somehow, no value was picked. This is a bug in RandomSelector");
            return mItems.cbegin(); //this is here to make the compiler not complain, it is never meant to run
        }

        mutable Generator mGenerator;
        mutable std::uniform_real_distribution<float> mWeightDistribution;
        std::deque<std::pair<Type, float>> mItems;
};
}
