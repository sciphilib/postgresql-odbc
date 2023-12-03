#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <exception>

#include "Appointment.h"
#include "json.hpp"

class Parser
{
public:
    void parseAppointment(std::string filename);
    void printAppointments();
private:
    nlohmann::json json;
    std::vector<Appointment> appointments;
};