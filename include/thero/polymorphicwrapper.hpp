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
    PolymorphicWrapper(const DerivedType& derived)
    {
        static_assert(std::is_base_of<BaseType, DerivedType>::value, "Can only be constructed from types that inherit BaseType");
        
        mCopyFunction = [] (const std::unique_ptr<BaseType>& derivedToCopy)
        {
            const auto originalPtr = static_cast<DerivedType*>(derivedToCopy.get());
            return std::unique_ptr<BaseType>(new DerivedType(*originalPtr)); //uses the normal copy mechanics of the DerivedType class
        };

        mValue.reset(new DerivedType(derived));
    }

    PolymorphicWrapper(const PolymorphicWrapper& other)
    {
        mCopyFunction = other.mCopyFunction;
        mValue = mCopyFunction(other.mValue); //uses stored lambda to copy
    }

    PolymorphicWrapper(PolymorphicWrapper&& other)
    {
        mCopyFunction = other.mCopyFunction;
        mValue = std::move(other.mValue);
    }

    PolymorphicWrapper& operator=(const PolymorphicWrapper& other)
    {
        mCopyFunction = other.mCopyFunction;
        mValue = mCopyFunction(other.mValue); //uses stored lambda to copy
        return *this;
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
    std::function<std::unique_ptr<BaseType>(const std::unique_ptr<BaseType>&)> mCopyFunction;
    std::unique_ptr<BaseType> mValue;
};
}