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
        DateTime beginDate(dateTimeBeginStr);
        DateTime endDate(dateTimeEndStr);
        Weekday weekday(item["idWeekday"].get<int>());
        appointments.emplace_back(item["id"], item["idDoctor"], weekday,
                                  beginDate, endDate, item["office"],
                                  item["district"]);
    }

    // return appointments;
}

void Parser::printAppointments()
{
    std::cout << "~~~~ Appointments ~~~~" << std::endl;
    for (const auto& item : appointments)
    {
        std::cout << "id = " << item.getId() << std::endl;
        std::cout << "idDoctor = " << item.getIdDoctor() << std::endl;
        std::cout << "idWeekday = " << item.getIdWeekday().toString() << std::endl;
        std::cout << "beginDate = " << item.getBeginDate().toString()
                  << std::endl;
        std::cout << "endDate = " << item.getEndDate().toString() << std::endl;
        std::cout << "office = " << item.getOffice() << std::endl;
        std::cout << "district = " << item.getDistrict() << std::endl;
        std::cout << std::endl;
    }
}


const std::array<std::string, 7> Weekday::DAYS = {
    "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
};

