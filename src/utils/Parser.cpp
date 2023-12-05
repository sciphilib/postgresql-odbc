#include "Parser.h"

void Parser::parseAppointment(std::string filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Can not open json file for appointments\n";
        throw std::exception();
    }
    file >> json;

    for (const auto& item : json)
    {
        std::string dateTimeBeginStr = item["beginDate"];
        std::string dateTimeEndStr = item["endDate"];
        DateTime beginDate(dateTimeBeginStr, true);
        DateTime endDate(dateTimeEndStr, true);
        appointments.emplace_back(item["idDoctor"], item["idWeekday"],
                                  beginDate, endDate, item["office"],
                                  item["district"]);
    }
}

void Parser::printAppointments()
{
    std::cout << "~~~~ Appointments ~~~~" << std::endl;
    for (const auto& item : appointments)
    {
        std::cout << "idDoctor = " << item.getIdDoctor() << std::endl;
        std::cout << "idWeekday = " << item.getIdWeekday() << std::endl;
        std::cout << "beginDate = " << item.getBeginDate().toString()
                  << std::endl;
        std::cout << "endDate = " << item.getEndDate().toString() << std::endl;
        std::cout << "office = " << item.getOffice() << std::endl;
        std::cout << "district = " << item.getDistrict() << std::endl;
        std::cout << std::endl;
    }
}