#pragma once

#include "BaseObject.h"
#include <iostream>

class Patient : public BaseObject
{
public:
    Patient(int id, std::string& lastName, std::string& firstName,
            std::string& middleName, std::string& address)
        : id_(id), lastName_(lastName), firstName_(firstName),
          middleName_(middleName), address_(address)
    {
    }

    int getId() const { return id_; }
    std::string getLastName() const { return lastName_; }
    std::string getFirstName() const { return firstName_; }
    std::string getMiddleName() const { return middleName_; }
    std::string getAddress() const { return address_; }

    void setId(int id) { id_ = id; }
    void setLastName(std::string& lastName) { lastName_ = lastName; }
    void setFirstName(std::string& firstName) { firstName_ = firstName; }
    void setMiddleName(std::string& middleName) { middleName_ = middleName; }
    void setAddress(std::string& address) { address_ = address; }
    void print() const override
    {
        std::cout << "Patient: id_ = " << id_ << ", lastName_ = " << lastName_
                  << ", firstName_ = " << firstName_
                  << ", middleName_ = " << middleName_
                  << ", address_ = " << address_ << std::endl;
    }

private:
    int id_;
    std::string lastName_;
    std::string firstName_;
    std::string middleName_;
    std::string address_;
};