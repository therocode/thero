#pragma once
#include <memory>
#include <functional>

namespace th
{
template<typename BaseType>
class PolymorphicWrapper
{
    public:
    template<typename DerivedType>
    PolymorphicWrapper(DerivedType&& derived)
    {
        using DecayedType = typename std::decay<DerivedType>::type;
        static_assert(std::is_base_of<BaseType, DecayedType>::value, "Can only be constructed from types that inherit BaseType");
        
        mCopyFunction = [] (const std::unique_ptr<BaseType>& derivedToCopy)
        {
            const auto originalPtr = static_cast<DecayedType*>(derivedToCopy.get());
            return std::unique_ptr<BaseType>(new DecayedType(*originalPtr)); //uses the normal copy mechanics of the DecayedType class
        };

        mValue.reset(new DecayedType(std::forward<DerivedType>(derived)));
    }

    PolymorphicWrapper(const PolymorphicWrapper& other)
    {
        mCopyFunction = other.mCopyFunction;
        mValue = mCopyFunction(other.mValue); //uses stored lambda to copy
    }

    PolymorphicWrapper(PolymorphicWrapper& other):
        PolymorphicWrapper(static_cast<const PolymorphicWrapper&>(other))
    {
    }

    PolymorphicWrapper(PolymorphicWrapper&& other)
    {
        mCopyFunction = std::move(other.mCopyFunction);
        mValue = std::move(other.mValue);
    }

    PolymorphicWrapper& operator=(const PolymorphicWrapper& other)
    {
        mCopyFunction = other.mCopyFunction;
        mValue = mCopyFunction(other.mValue); //uses stored lambda to copy
        return *this;
    }

    PolymorphicWrapper& operator=(PolymorphicWrapper& other)
    {
        return static_cast<const PolymorphicWrapper&>(other);
    }

    PolymorphicWrapper& operator=(PolymorphicWrapper&& other)
    {
        mCopyFunction = other.mCopyFunction;
        mValue = std::move(other.mValue);
        return *this;
    }

    const BaseType& operator*() const
    {
        return *mValue;
    }

    BaseType& operator*()
    {
        return *mValue;
    }

    const BaseType* operator->() const
    {
        return mValue.get();
    }

    BaseType* operator->()
    {
        return mValue.get();
    }

    private:
    std::unique_ptr<BaseType> (*mCopyFunction)(const std::unique_ptr<BaseType>&);
    std::unique_ptr<BaseType> mValue;
};
}
