#pragma once
#include <thero/assert.hpp>
#include <memory>

namespace th
{
template<typename Type>
class Optional
{
    public:
        Optional()
        {
        }

        template<typename ...Args>
        Optional(Args&& ...arguments)
        {
            mValue = std::make_shared<Type>(arguments...);
        }

        Optional(Type&& value)
        {
            mValue = std::make_shared<Type>(std::move(value));
        }

        Optional(const Optional& other)
        {
            if(other.mValue)
                mValue = std::make_shared<Type>(*other.mValue);
            else
                mValue.reset();
        }

        Optional(Optional& other):
            Optional(static_cast<const Optional&>(other))
        {
        }

        Optional(Optional&& other)
        {
            mValue = other.mValue;
            other.mValue = nullptr;
        }

        Optional& operator=(const Optional& other)
        {
            if(other.mValue)
                mValue = std::make_shared<Type>(*other.mValue);
            else
                mValue.reset();

            return *this;
        }

        Optional& operator=(Optional& other)
        {
            return *this = static_cast<const Optional&>(other);
        }

        Optional& operator=(Optional&& other)
        {
            mValue = other.mValue;
            other.mValue = nullptr;

            return *this;
        }

        explicit operator bool() const
        {
            return static_cast<bool>(mValue);
        }

        const Type& operator*() const
        {
            TH_ASSERT(mValue != nullptr, "Derferencing empty Optional");
            return *mValue;
        }

        Type& operator*()
        {
            TH_ASSERT(mValue != nullptr, "Derferencing empty Optional");
            return *mValue;
        }

        const Type* operator->() const
        {
            TH_ASSERT(mValue != nullptr, "Accessing empty Optional");
            return mValue.get();
        }

        Type* operator->()
        {
            TH_ASSERT(mValue != nullptr, "Accessing empty Optional");
            return mValue.get();
        }

        bool isNull()
        {
            return !*this;
        }

        template <typename InValue>
        Type valueOr(InValue&& in) const
        {
            if(*this)
                return **this;
            else
                return in;
        }

        template<typename ...Args>
        void reset(Args&& ...arguments)
        {
            mValue = std::make_shared<Type>(std::move(arguments...));
        }

        void reset(Type&& value)
        {
            mValue = std::make_shared<Type>(std::move(value));
        }

        void reset()
        {
            mValue.reset();
        }

    private:
        std::shared_ptr<Type> mValue;
};
}
