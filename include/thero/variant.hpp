#pragma once
#include <typeindex>
#include <memory>
#include <functional>
#include <thero/assert.hpp>

namespace th
{
    class Variant
    {
        public:
            Variant():
                mStoredType(typeid(void))
            {
                clear();
            }
            Variant(Variant&& other):
                mStoredType(typeid(void))
            {
                std::swap(other.mStoredData, mStoredData);
                std::swap(mStoredType, other.mStoredType);
                std::swap(mCopyFunction, other.mCopyFunction);
            }

            Variant& operator=(Variant&& other)
            {
                std::swap(mStoredData, other.mStoredData);
                std::swap(mStoredType, other.mStoredType);
                std::swap(mCopyFunction, other.mCopyFunction);

                return *this;
            }

            Variant(const Variant& other):
                mStoredType(other.mStoredType)
            {
                mStoredData = other.mCopyFunction(other.mStoredData);
                mCopyFunction = other.mCopyFunction;
            }

            Variant& operator=(const Variant& other)
            {
                mStoredType = other.mStoredType;
                mStoredData = other.mCopyFunction(other.mStoredData);
                mCopyFunction = other.mCopyFunction;
                return *this;
            }

            template<typename Type>
            Variant(Type data) :
                mStoredType(typeid(data))
            {
                mStoredData = std::static_pointer_cast<void>(std::make_shared<Type>(std::move(data)));
            
                mCopyFunction = [](std::shared_ptr<void> data)
                {
                    std::shared_ptr<Type> toCopy = std::static_pointer_cast<Type>(data);
                    std::shared_ptr<Type> copy = std::make_shared<Type>(*toCopy);
                    return std::static_pointer_cast<void>(copy);
                };
            }
            
            template<typename Type>
            Type& get()
            {
                TH_ASSERT(mStoredType == typeid(Type), "Trying to get variant as the type " + std::string(typeid(Type).name()) + " when it is of type " + std::string(mStoredType.name()));
                TH_ASSERT(mStoredData != nullptr, "Trying to get uninitialised variant");
            
                return *std::static_pointer_cast<Type>(mStoredData);
            }

            template<typename Type>
            const Type& get() const
            {
                TH_ASSERT(mStoredType == typeid(Type), "Trying to get variant as the type " + std::string(typeid(Type).name()) + " when it is of type " + std::string(mStoredType.name()));
                TH_ASSERT(mStoredData != nullptr, "Trying to set uninitialised variant");
            
                return *std::static_pointer_cast<Type>(mStoredData);
            }

            template<typename Type>
            void set(const Type& data)
            {
                TH_ASSERT(mStoredType == typeid(Type), "Trying to set variant as the type " + std::string(typeid(Type).name()) + " when it is of type " + std::string(mStoredType.name()));
            
                *std::static_pointer_cast<Type>(mStoredData) = data;
            }

            template<typename Type>
            Type release()
            {
                Type released = std::move(*std::static_pointer_cast<Type>(mStoredData));
                clear();
                return released;
            }

            template<typename Type>
            bool isOfType() const
            {
                return mStoredType == typeid(Type);
            }

            bool isSameTypeAs(const Variant& other) const
            {
                return mStoredType == other.mStoredType;
            }

            void clear()
            {
                mStoredType = typeid(void);
                mStoredData.reset();

                mCopyFunction = [](std::shared_ptr<void> data)
                {
                    return data;
                };
            }
        private:
            std::type_index mStoredType;
            std::shared_ptr<void> mStoredData;
            std::function<std::shared_ptr<void>(std::shared_ptr<void>)> mCopyFunction;
    };
}
