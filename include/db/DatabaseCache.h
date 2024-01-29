#pragma once

#include "DatabaseFacade.h"

#include <memory>
#include <optional>
#include <unordered_map>

template <typename T>
class DatabaseCache
{
public:
    auto begin() const { return cacheMap.cbegin(); }
    auto end() const { return cacheMap.cend(); }

    void add(const T& object)
    {

        cacheMap.emplace(std::piecewise_construct,
                         std::forward_as_tuple(object.getId()),
                         std::forward_as_tuple(object));
    }

    void printCacheMap() const
    {
        int num = 0;
        for (const auto& i : cacheMap)
        {
            std::cout << num++ << ". ";
            i.second.print();
            std::cout << std::endl;
        }
    }

    void printCacheMapNormal() const
    {
        int num = 1;
        for (const auto& i : cacheMap)
        {
            std::cout << num++ << ". ";
            i.second.printNormal();
        }
    }

    void addMultiple(const std::unordered_map<int, T>& objects)
    {
        for (const auto& object : objects)
        {
            cacheMap.emplace(object.first, object.second);
        }
    }

    std::optional<T> getFromCacheMap(int id)
    {
        auto it = cacheMap.find(id);
        if (it != cacheMap.end())
        {
            return it->second;
        }
        return std::nullopt;
    }

    T& get(int n)
    {
        int i = 0;
        for (auto it = cacheMap.begin(); it != cacheMap.end(); it++)
        {
            if (i == n)
                return it->second;
            i++;
        }
    }

    void removeFromCacheMap(int id) { cacheMap.erase(id); }

private:
    std::unordered_map<int, T> cacheMap;
};