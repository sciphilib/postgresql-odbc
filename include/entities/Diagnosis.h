#pragma once

#include "BaseObject.h"
#include <iostream>

class Diagnosis : public BaseObject
{
public:
    Diagnosis(int idVisit, std::string& description)
        : id_(-1), idVisit_(idVisit), description_(description)
    {
    }
    Diagnosis(int id, int idVisit, std::string& description)
        : id_(id), idVisit_(idVisit), description_(description)
    {
    }

    int getId() const { return id_; }
    int getIdVisit() const { return idVisit_; }
    std::string getDescription() const { return description_; }

    void setId(int id) { id_ = id; }
    void setIdVisit(int idVisit) { idVisit_ = idVisit; }
    void setDescription(std::string& description)
    {
        description_ = description;
    }
    void print() const override
    {
        std::cout << "Diagnosis: id = " << id_ << ", idVisit = " << idVisit_
                  << ", description = " << description_ << std::endl;
    }

private:
    int id_;
    int idVisit_;
    std::string description_;
};