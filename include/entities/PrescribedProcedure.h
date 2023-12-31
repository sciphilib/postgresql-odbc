#pragma once

#include "BaseObject.h"
#include "Procedure.h"

class PrescribedProcedure : public BaseObject
{
public:
    PrescribedProcedure(int idVisit, int idProcedure, int count)
        : id_(-1), idVisit_(idVisit), idProcedure_(idProcedure), count_(count)
    {
    }
    PrescribedProcedure(int id, int idVisit, int idProcedure, int count)
        : id_(id), idVisit_(idVisit), idProcedure_(idProcedure), count_(count)
    {
    }

    int getId() const { return id_; }
    int getIdVisit() const { return idVisit_; }
    int getIdProcedure() const { return idProcedure_; }
    int getCount() const { return count_; }

    void setId(int id) { id_ = id; }
    void setIdVisit(int idVisit) { idVisit_ = idVisit; }
    void setIdProcedure(int idProcedure) { idProcedure_ = idProcedure; }
    void setCount(int count) { count_ = count; }
    void print() const override
    {
        std::cout << "PrescribedProcedure: id = " << id_
                  << ", idVisit = " << idVisit_
                  << ", idProcedure = " << idProcedure_
                  << ", count = " << count_ << std::endl;
    }

private:
    int id_;
    int idVisit_;
    int idProcedure_;
    int count_;
};