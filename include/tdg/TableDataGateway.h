#pragma once

#include "BaseObject.h"
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#include <boost/locale.hpp>

class TableDataGateway
{
public:
    TableDataGateway(SQLHDBC hDbc) : hDbc_(hDbc) {}
    std::string to_utf8(const std::string& input);

    virtual void insert(const BaseObject& object) = 0;
    virtual std::unique_ptr<BaseObject> findById(int id) = 0;
    virtual bool update(int id, const BaseObject& object) = 0;
    virtual bool deleteById(int id) = 0;

protected:
    SQLHDBC hDbc_;
    SQLCHAR message[1000];
    SQLCHAR sqlState[SQL_SQLSTATE_SIZE + 1];
    SQLINTEGER nativeError;
    SQLSMALLINT textLength;
};

inline std::string TableDataGateway::to_utf8(const std::string& input)
{
    try
    {
        return boost::locale::conv::to_utf<char>(input, "Latin1");
    }
    catch (const boost::locale::conv::conversion_error& e)
    {
        std::cerr << "Conversion error: " << e.what() << std::endl;
        return std::string();
    }
}