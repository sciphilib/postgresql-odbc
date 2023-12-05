#pragma once

#include "BaseObject.h"
#include "Test.h"

class TestResult : public BaseObject
{
public:
    TestResult(int idVisit, int idTest, std::string& result)
        : id_(-1), idVisit_(idVisit), idTest_(idTest), result_(result)
    {
    }

    int getId() const { return id_; }
    int getIdVisit() const { return idVisit_; }
    int getIdTest() const { return idTest_; }
    std::string getResult() const { return result_; }

    void setId(int id) { id_ = id; }
    void setIdVisit(int idVisit) { idVisit_ = idVisit; }
    void setIdTest(int idTest) { idTest_ = idTest; }
    void setResult(std::string& result) { result_ = result; }
    void print() const override
    {
        std::cout << "TestResult: id_ = " << id_ << ", idVisit_ = " << idVisit_
                  << ", idTest_ = " << idTest_ << ", result_ = " << result_
                  << std::endl;
    }

private:
    int id_;
    int idVisit_;
    int idTest_;
    std::string result_;
};