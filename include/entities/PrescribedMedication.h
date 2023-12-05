#pragma once

#include "BaseObject.h"
#include <iostream>

class PrescribedMedication : public BaseObject
{
public:
    PrescribedMedication(int idVisit, int idMedication)
        : id_(-1), idVisit_(idVisit), idMedication_(idMedication)
    {
    }

    int getId() const { return id_; }
    int getIdVisit() const { return idVisit_; }
    int getIdMedication() const { return idMedication_; }

    void setId(int id) { id_ = id; }
    void setIdVisit(int idVisit) { idVisit_ = idVisit; }
    void setIdMedication(int idMedication) { idMedication_ = idMedication; }
    void print() const override
    {
        std::cout << "PrescribedMedication id_ = " << id_
                  << ", idVisit_ = " << idVisit_
                  << ", idMedication_ = " << idMedication_ << std::endl;
    }

private:
    int id_;
    int idVisit_;
    int idMedication_;
};