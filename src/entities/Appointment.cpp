#include "Appointment.h"

#include <iostream>

int Appointment::getId() const { return id_; }
int Appointment::getIdDoctor() const { return idDoctor_; }
Weekday Appointment::getIdWeekday() const { return idWeekday_; }
DateTime Appointment::getBeginDate() const { return beginDate_; }
DateTime Appointment::getEndDate() const { return endDate_; }
int Appointment::getOffice() const { return office_; }
int Appointment::getDistrict() const { return district_; }

void Appointment::setId(int id)
{
    if (id < 0)
    {
        std::cout << "ERROR: id can not be negative\n";
        return;
    }
    id_ = id;
}

void Appointment::setIdDoctor(int idDoctor)
{
    if (idDoctor < 0)
    {
        std::cout << "ERROR: id can not be negative\n";
        return;
    }
    idDoctor_ = idDoctor;
}

/* void Appointment::setIdWeekday(Weekday weekday)
{
    idWeekday_ = weekday;
} */

void Appointment::setBeginDate(DateTime date) { beginDate_ = date; }

void Appointment::setEndDate(DateTime date) { endDate_ = date; }

void Appointment::setOffice(int office)
{
    if (office < 0)
    {
        std::cout << "ERROR: office can not be negative\n";
        return;
    }
    office_ = office;
}

void Appointment::setDistrict(int district)
{
    if (district < 0)
    {
        std::cout << "ERROR: disctrict can not be negative\n";
        return;
    }
    district_ = district;
}