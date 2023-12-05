#pragma once

#include "AppointmentFactory.h"
#include "BaseObject.h"
#include "DoctorFactory.h"
#include "IFactory.h"
#include "MedicationFactory.h"
#include "PatientFactory.h"
#include "PrescribedMedicationFactory.h"
#include "PrescribedProcedureFactory.h"
#include "ProcedureFactory.h"
#include "SpecializationFactory.h"
#include "TestFactory.h"
#include "TestResultFactory.h"
#include "VisitFactory.h"
#include "WeekdayFactory.h"
#include "DiagnosisFactory.h"

#include <memory>

class MainFactory : public IFactory
{
public:
    virtual std::unique_ptr<BaseObject>
    createObject(const std::string& type,
                 const std::map<std::string, std::string>& parameters) override
    {
        if (type == "Weekday")
        {
            WeekdayFactory weekdayFactory;
            return weekdayFactory.createObject(parameters);
        }
        else if (type == "Specialization")
        {
            SpecializationFactory specializationFactory;
            return specializationFactory.createObject(parameters);
        }
        else if (type == "Doctor")
        {
            DoctorFactory doctorFactory;
            return doctorFactory.createObject(parameters);
        }
        else if (type == "Appointment")
        {
            AppointmentFactory appointmentFactory;
            return appointmentFactory.createObject(parameters);
        }
        else if (type == "Patient")
        {
            PatientFactory patientFactory;
            return patientFactory.createObject(parameters);
        }
        else if (type == "Medication")
        {
            MedicationFactory medicationFactory;
            return medicationFactory.createObject(parameters);
        }
        else if (type == "PrescribedMedication")
        {
            PrescribedMedicationFactory prmedFactory;
            return prmedFactory.createObject(parameters);
        }
        else if (type == "Visit")
        {
            VisitFactory visitFactory;
            return visitFactory.createObject(parameters);
        }
        else if (type == "Procedure")
        {
            ProcedureFactory procedureFactory;
            return procedureFactory.createObject(parameters);
        }
        else if (type == "PrescribedProcedure")
        {
            PrescribedProcedureFactory prprocFactory;
            return prprocFactory.createObject(parameters);
        }
        else if (type == "Test")
        {
            TestFactory testFactory;
            return testFactory.createObject(parameters);
        }
        else if (type == "TestResult")
        {
            TestResultFactory testResultFactory;
            return testResultFactory.createObject(parameters);
        }
        else if (type == "Diagnosis")
        {
            DiagnosisFactory diagnosisFactory;
            return diagnosisFactory.createObject(parameters);
        }

        return nullptr;
    }
};