#pragma once
#include "BaseObject.h"
#include "DateTime.h"
#include "Weekday.h"

class Appointment : public BaseObject
{
public:
    struct Date;
    int getId() const;
    int getIdDoctor() const;
    int getIdWeekday() const;
    DateTime getBeginDate() const;
    DateTime getEndDate() const;
    int getOffice() const;
    int getDistrict() const;

    void setId(int id);
    void setIdDoctor(int idDoctor);
    void setIdWeekday(int idWeekday);
    void setBeginDate(DateTime date);
    void setEndDate(DateTime date);
    void setOffice(int office);
    void setDistrict(int district);

    Appointment(int idDoctor, int idWeekday, DateTime beginDate,
                DateTime endDate, int office, int district)
        : id_(-1), idDoctor_(idDoctor), idWeekday_(idWeekday),
          beginDate_(beginDate), endDate_(endDate), office_(office),
          district_(district)
    {
    }

    void print() const override
    {
        std::cout << "Appointment: id_ = " << id_
                  << ", idDoctor_ = " << idDoctor_
                  << ", idWeekday_ = " << idWeekday_
                  << ", beginDate_ = " << beginDate_.toString()
                  << ", endDate_ = " << endDate_.toString()
                  << ", office_ = " << office_ << ", district_ = " << district_
                  << std::endl;
    }

private:
    int id_;
    int idDoctor_;
    int idWeekday_;
    DateTime beginDate_;
    DateTime endDate_;
    int office_;
    int district_;
};
