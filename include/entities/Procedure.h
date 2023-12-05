#pragma once

#include "BaseObject.h"
#include <iostream>

class Procedure : public BaseObject
{
public:
    Procedure(std::string& name) : id_(-1), name_(name) {}

    int getId() const { return id_; }
    std::string getName() const { return name_; }

    void setId(int id) { id_ = id; }
    void setName(std::string& name) { name_ = name; }
    void print() const override
    {
        std::cout << "Procedure: id_ = " << id_ << ", name_ = " << name_
                  << std::endl;
    }

private:
    int id_;
    std::string name_;
};