#pragma once

#include "BaseObject.h"
#include "DateTime.h"

class Visit : public BaseObject
{
public:
    Visit(int id, int idPatient, DateTime dateVisit, DateTime dateDischarge,
          DateTime dateClose, std::string& complaints)
        : id_(id), idPatient_(idPatient), dateVisit_(dateVisit),
          dateDischarge_(dateDischarge), dateClose_(dateClose),
          complaints_(complaints)
    {
    }

    int getId() const { return id_; }
    int getIdPatient() const { return idPatient_; }
    DateTime getDateVisit() const { return dateVisit_; }
    DateTime getDateDischarge() const { return dateDischarge_; }
    DateTime getDateClose() const { return dateClose_; }
    std::string getComplaints() const { return complaints_; }

    void setId(int id) { id_ = id; }
    void setIdPatient(int idPatient) { idPatient_ = idPatient; }
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
                  << ", dateVisit_ = " << dateVisit_.toString()
                  << ", dateDischarge_ = " << dateDischarge_.toString()
                  << ", dateClose_ = " << dateClose_.toString()
                  << ", complaints_ = " << complaints_ << std::endl;
    }

private:
    int id_;
    int idPatient_;
    DateTime dateVisit_;
    DateTime dateDischarge_;
    DateTime dateClose_;
    std::string complaints_;
};