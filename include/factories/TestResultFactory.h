#pragma once

#include "IFactory.h"
#include "TestResult.h"
#include <memory>

class TestResultFactory
{
public:
    std::unique_ptr<BaseObject>
    createObject(const std::map<std::string, std::string>& parameters)
    {
        int idVisit, idTest;
        std::string result;

        if (parameters.find("id_visit") != parameters.end())
        {
            idVisit = std::stoi(parameters.at("id_visit"));
        }
        else
        {
            std::cout
                << "Error: there is no id_visit in parameters of TestResult"
                << std::endl;
            return nullptr;
        }

        if (parameters.find("id_test") != parameters.end())
        {
            idTest = std::stoi(parameters.at("id_test"));
        }
        else
        {
            std::cout
                << "Error: there is no id_test in parameters of TestResult"
                << std::endl;
            return nullptr;
        }

        if (parameters.find("result") != parameters.end())
        {
            result = parameters.at("result");
        }
        else
        {
            std::cout << "Error: there is no result in parameters of TestResult"
                      << std::endl;
            return nullptr;
        }

        return std::make_unique<TestResult>(idVisit, idTest, result);
    }
};