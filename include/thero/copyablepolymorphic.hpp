#include <memory>
#include <functional>

namespace th
{
template<typename BaseType>
class CopyablePolymorphic
{
    public:
    template<typename DerivedType>
    CopyablePolymorphic(const DerivedType& need)
    {
        static_assert(std::is_base_of<BaseType, DerivedType>::value, "Can only be constructed from types that inherit BaseType");
        
        mCopyFunction = [] (const std::unique_ptr<BaseType>& needToCopy)
        {
            const auto originalPtr = static_cast<DerivedType*>(needToCopy.get());
            return std::unique_ptr<BaseType>(new DerivedType(*originalPtr)); //uses the normal copy mechanics of the DerivedType class
        };

        mValue.reset(new DerivedType(need));
    }

    CopyablePolymorphic(const CopyablePolymorphic& other)
    {
        mCopyFunction = other.mCopyFunction;
        mValue = mCopyFunction(other.mValue); //uses stored lambda to copy
    }

    CopyablePolymorphic(CopyablePolymorphic&& other)
    {
        mCopyFunction = other.mCopyFunction;
        mValue = std::move(other.mValue);
    }

    CopyablePolymorphic& operator=(const CopyablePolymorphic& other)
    {
        mCopyFunction = other.mCopyFunction;
        mValue = mCopyFunction(other.mValue); //uses stored lambda to copy
        return *this;
    }

    CopyablePolymorphic& operator=(CopyablePolymorphic&& other)
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
