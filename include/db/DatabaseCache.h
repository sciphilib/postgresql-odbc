#pragma once

#include "DatabaseFacade.h"

#include <memory>
#include <unordered_map>

template <typename T>
class DatabaseCache
{
public:
    /* void add(std::unique_ptr<T> object)
    {
        cache.push_back(*object);
    } */

    void print()
    {
        int num = 0;
        for (const auto& i : cache)
        {
            std::cout << num++ << " | ";
            i.print();
            std::cout << std::endl;
        }
    }

    void addMultiple(const std::vector<T>& objects)
    {
        for (const auto& object : objects)
        {
            cache.push_back(object);
        }
    }

private:
    std::vector<T> cache;
};