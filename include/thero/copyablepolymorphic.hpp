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
            const DerivedType* downCasted = dynamic_cast<DerivedType*>(&*needToCopy);
            return std::make_unique<DerivedType>(*downCasted); //uses the normal copy mechanics of the DerivedType class
        };

        mValue = std::make_unique<DerivedType>(need);
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

    const BaseType& get() const
    {
        return *mValue;
    }

    BaseType& get()
    {
        return *mValue;
    }
    
    private:
    std::function<std::unique_ptr<BaseType>(const std::unique_ptr<BaseType>&)> mCopyFunction;
    std::unique_ptr<BaseType> mValue;
};
}
