#pragma once
#include "BaseObject.h"
#include <iostream>

class Doctor : public BaseObject
{
public:
    Doctor(int id, std::string& lastName, std::string& firstName,
           std::string& middleName, int idSpec)
        : id_(id), lastName_(lastName), firstName_(firstName),
          middleName_(middleName), idSpec_(idSpec){};

    int getId() const { return id_; }
    std::string getLastName() const { return lastName_; }
    std::string getFirstName() const { return firstName_; }
    std::string getMiddleName() const { return middleName_; }
    int getIdSpec() const { return idSpec_; }

    void setId(int id) { id_ = id; }
    void setLastName(std::string& lastName) { lastName_ = lastName; }
    void setFirstName(std::string& firstName) { firstName_ = firstName; }
    void setMiddleName(std::string& middleName) { middleName_ = middleName; }
    void setIdSpec(int idSpec) { idSpec_ = idSpec; }
    void print() const override
    {
        std::cout << "Doctor: id_ = " << id_ << ", lastName_ = " << lastName_
                  << ", firstName_ = " << firstName_
                  << ", middleName_ = " << middleName_
                  << ", idSpec_ = " << idSpec_ << std::endl;
    }

private:
    int id_;
    std::string lastName_;
    std::string firstName_;
    std::string middleName_;
    int idSpec_;
};