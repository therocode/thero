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
            mValue = std::make_shared<Type>(std::move(arguments...));
        }

        Optional(Type&& value)
        {
            mValue = std::make_shared<Type>(std::move(value));
        }

        operator bool() const
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
        void set(Args&& ...arguments)
        {
            mValue = std::make_shared<Type>(std::move(arguments...));
        }

        void set(Type&& value)
        {
            mValue = std::make_shared<Type>(std::move(value));
        }

    private:
        std::shared_ptr<Type> mValue;
};
}
