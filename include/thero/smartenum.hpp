#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <iostream>
#include <algorithm>

namespace thero
{
    inline std::string trimWhitespace(std::string str)
    {
        auto const begin = std::find_if_not(str.begin(), str.end(), &isspace);
        auto const end = std::find_if_not(str.rbegin(), str.rend(), &isspace);

        return {begin, end.base()};
    }
    
    inline std::string extractEntry(std::string& valuesString)
    {
        std::string result;
        size_t nextCommaPos = valuesString.find(',');
    
        if(nextCommaPos != std::string::npos)
        {
            std::string segment = valuesString.substr(0, nextCommaPos);
            valuesString.erase(0, nextCommaPos + 1);
            result = trimWhitespace(segment);
        }
        else
        {
            result = trimWhitespace(valuesString);
            valuesString = "";
        };
        return result;
    };
    
    inline std::unordered_map<int32_t, std::string> makeEnumNameMap(std::string enumValuesString)
    {
        std::unordered_map<int32_t, std::string> nameMap;
    
        int32_t currentEnumValue = 0;
        while(enumValuesString != "")
        {
            std::string currentEnumEntry = extractEntry(enumValuesString);
    
            size_t equalSignPos = currentEnumEntry.find('=');
            if(equalSignPos != std::string::npos)
            {
                std::string rightHandSide = currentEnumEntry.substr(equalSignPos + 1);
                currentEnumValue = std::stoi(rightHandSide);
                currentEnumEntry.erase(equalSignPos);
            }
    
            currentEnumEntry = trimWhitespace(currentEnumEntry);
    
            nameMap[currentEnumValue] = currentEnumEntry;
    
            currentEnumValue++;
        }
    
        return nameMap;
    }
    
    template<typename Type>
    std::vector<Type> makeEnumList(std::string enumValuesString)
    {
        std::vector<Type> enumList;
    
        int32_t currentEnumValue = 0;
        while(enumValuesString != "")
        {
            std::string currentEnumEntry = extractEntry(enumValuesString);
    
            size_t equalSignPos = currentEnumEntry.find('=');
            if(equalSignPos != std::string::npos)
            {
                std::string rightHandSide = currentEnumEntry.substr(equalSignPos + 1);
                currentEnumValue = std::stoi(rightHandSide);
                currentEnumEntry.erase(equalSignPos);
            }
    
            currentEnumEntry = trimWhitespace(currentEnumEntry);
    
            enumList.push_back(static_cast<Type>(currentEnumValue));
    
            currentEnumValue++;
        }
    
        return enumList;
    }
}

#define smart_enum(Type, ...) enum Type { __VA_ARGS__}; \
    static const std::unordered_map<int32_t, std::string> Type##_enum_names = thero::makeEnumNameMap(#__VA_ARGS__);\
    static const std::vector<Type> Type##_list = thero::makeEnumList<Type>(#__VA_ARGS__);\
    \
    inline const std::string& to_string(Type value) \
    { \
        return Type##_enum_names.at((int32_t)value);\
    } \

#define smart_enum_class(Type, ...) enum class Type { __VA_ARGS__}; \
    static const std::unordered_map<int32_t, std::string> Type##_enum_names = thero::makeEnumNameMap(#__VA_ARGS__);\
    static const std::vector<Type> Type##_list = thero::makeEnumList<Type>(#__VA_ARGS__);\
    \
    inline const std::string& to_string(Type value) \
    { \
        return Type##_enum_names.at((int32_t)value);\
    } \
    \
    inline std::ostream& operator<<(std::ostream& outStream, Type value)\
    {\
        outStream << to_string(value);\
        return outStream;\
    }
