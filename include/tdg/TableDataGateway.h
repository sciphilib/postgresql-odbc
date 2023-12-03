#pragma once

#include "BaseObject.h"
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

class TableDataGateway
{
public:
    TableDataGateway(SQLHDBC hDbc) : hDbc_(hDbc) {}

    virtual void insert(const BaseObject& object) = 0;

protected:
    SQLHDBC hDbc_;
};