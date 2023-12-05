#pragma once

#include "BaseObject.h"
#include "DateTime.h"

class Visit : public BaseObject
{
public:
    Visit(int idPatient, int idDoctor, std::string& complaints,
          DateTime dateVisit, DateTime dateDischarge, DateTime dateClose)
        : id_(-1), idPatient_(idPatient), idDoctor_(idDoctor),
          complaints_(complaints), dateVisit_(dateVisit),
          dateDischarge_(dateDischarge), dateClose_(dateClose)
    {
    }

    int getId() const { return id_; }
    int getIdPatient() const { return idPatient_; }
    int getIdDoctor() const { return idDoctor_; }
    DateTime getDateVisit() const { return dateVisit_; }
    DateTime getDateDischarge() const { return dateDischarge_; }
    DateTime getDateClose() const { return dateClose_; }
    std::string getComplaints() const { return complaints_; }

    void setId(int id) { id_ = id; }
    void setIdPatient(int idPatient) { idPatient_ = idPatient; }
    void setIdDoctor(int idDoctor) { idDoctor_ = idDoctor; }
    void setDateVisit(DateTime dateVisit) { dateVisit_ = dateVisit; }
    void setDateDischarge(DateTime dateDischarge)
    {
        dateDischarge_ = dateDischarge;
    }
    void setDateClose(DateTime dateClose) { dateClose_ = dateClose; }
    void setComplaints(std::string& complaints) { complaints_ = complaints; }
    void print() const override
    {
        std::cout << "Visit: id_ = " << id_ << ", idPatient_ = " << idPatient_
                  << ", idDoctor_ = " << idDoctor_
                  << ", dateVisit_ = " << dateVisit_.toString()
                  << ", dateDischarge_ = " << dateDischarge_.toString()
                  << ", dateClose_ = " << dateClose_.toString()
                  << ", complaints_ = " << complaints_ << std::endl;
    }

private:
    int id_;
    int idPatient_;
    int idDoctor_;
    std::string complaints_;
    DateTime dateVisit_;
    DateTime dateDischarge_;
    DateTime dateClose_;
};