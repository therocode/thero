#pragma once
#include <thero/copyablepolymorphic.hpp>
#include <unordered_map>
#include <typeindex>

namespace th
{
template <typename BaseClass>
class ModuleSet
{
    public:
        template <typename Module, typename... Arguments>
        void emplace(Arguments&&... arguments)
        {
            mModules.emplace(std::type_index(typeid(Module)), Module(std::forward<Arguments>(arguments)...));
        }

        template <typename Module>
        void erase()
        {
            mModules.erase(std::type_index(typeid(Module)));
        }

        void onEach(std::function<void(BaseClass&)> function)
        {
            for(auto& module : mModules)
                function(*module.second);
        }

        void onEach(std::function<void(const BaseClass&)> function) const
        {
            for(auto& module : mModules)
                function(*module.second);
        }

        template <typename Module>
        Module& get()
        {
            return static_cast<Module&>(*mModules.at(std::type_index(typeid(Module))));
        }

        template <typename Module>
        const Module& get() const
        {
            return static_cast<const Module&>(*mModules.at(std::type_index(typeid(Module))));
        }

        template <typename Module>
        Module* find()
        {
            auto iterator = mModules.find(std::type_index(typeid(Module)));

            if(iterator != mModules.end())
                return static_cast<Module*>(&*iterator->second);
            else
                return nullptr;
        }

        template <typename Module>
        const Module* find() const
        {
            const auto iterator = mModules.find(std::type_index(typeid(Module)));

            if(iterator != mModules.end())
                return static_cast<const Module*>(&*iterator->second);
            else
                return nullptr;
        }
    private:
        std::unordered_map<std::type_index, PolymorphicWrapper<BaseClass>> mModules;
};
}

