#pragma once

#include <iostream>

class BaseObject
{
public:
    virtual ~BaseObject() = default;
    virtual void print() const = 0;
};