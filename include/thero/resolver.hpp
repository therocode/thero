#pragma once
#include <vector>

template <typename Condition, typename Value>
class Resolver
{
    public:
        Resolver(std::vector<std::pair<Condition, Value>> conditions, Value defaultValue):
            mConditions(std::move(conditions)),
            mDefaultValue(std::move(defaultValue))
    {
    }

    const Value& resolve(const Condition& condition)
    {
        for(const auto& pair : mConditions)
            if(pair.first == condition)
                return pair.second;

        return mDefaultValue;
    }

    private:
        std::vector<std::pair<Condition, Value>> mConditions;
        Value mDefaultValue;
};
