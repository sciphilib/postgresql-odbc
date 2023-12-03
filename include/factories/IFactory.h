#pragma once

#include "BaseObject.h"
#include <iostream>
#include <memory>
#include <map>

class IFactory {
public:
    virtual std::unique_ptr<BaseObject> createObject(const std::string& type, const std::map<std::string, std::string>& parameters) = 0;
};