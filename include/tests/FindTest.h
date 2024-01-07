#pragma once

#include "Appointment.h"
#include "DatabaseFacade.h"
#include "DatabaseTest.h"
#include "Diagnosis.h"
#include "Doctor.h"
#include "Medication.h"
#include "Patient.h"
#include "PrescribedMedication.h"
#include "PrescribedProcedure.h"
#include "Procedure.h"
#include "Specialization.h"
#include "Test.h"
#include "TestResult.h"
#include "Visit.h"
#include "Weekday.h"

class FindTest : public DatabaseTest
{
public:
    FindTest(DatabaseFacade* databaseFacade) : DatabaseTest(databaseFacade) {}
    void start() override
    {
        std::unique_ptr<Appointment> appointment;
        appointment = databaseFacade->findAppointmentById(1);
        if (appointment->getId() == 1)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";
        appointment->print();
        appointment = databaseFacade->findAppointmentById(5);
        appointment->print();
        if (appointment->getId() == 5)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";

        std::unique_ptr<Weekday> weekday;
        weekday = databaseFacade->findWeekdayById(1);
        weekday->print();
        if (weekday->getId() == 1)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";
        weekday = databaseFacade->findWeekdayById(5);
        weekday->print();
        if (weekday->getId() == 5)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";

        std::unique_ptr<Specialization> spec;
        spec = databaseFacade->findSpecializationById(1);
        spec->print();
        if (spec->getId() == 1)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";
        spec = databaseFacade->findSpecializationById(3);
        spec->print();
        if (spec->getId() == 3)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";

        std::unique_ptr<Doctor> doctor;
        doctor = databaseFacade->findDoctorById(1);
        doctor->print();
        if (doctor->getId() == 1)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";
        doctor = databaseFacade->findDoctorById(4);
        doctor->print();
        if (doctor->getId() == 4)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";
        int id = 100;
        doctor = databaseFacade->findDoctorById(id);
        if (doctor)
        {
            doctor->print();
            std::cout << "SUCCESS\n";
        }
        else
            std::cout << "NOT SUCCESS Doctor: there is no doctor with id = "
                      << id << std::endl;

        std::unique_ptr<Patient> patient;
        patient = databaseFacade->findPatientById(1);
        patient->print();
        if (patient->getId() == 1)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";
        patient = databaseFacade->findPatientById(2);
        patient->print();
        if (patient->getId() == 2)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";

        std::unique_ptr<Visit> visit;
        visit = databaseFacade->findVisitById(1);
        visit->print();
        if (visit->getId() == 1)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";
        visit = databaseFacade->findVisitById(5);
        visit->print();
        if (visit->getId() == 5)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";

        std::unique_ptr<PrescribedMedication> prmed;
        prmed = databaseFacade->findPrescribedMedicationById(1);
        prmed->print();
        if (prmed->getId() == 1)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";
        prmed = databaseFacade->findPrescribedMedicationById(3);
        prmed->print();
        if (prmed->getId() == 3)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";

        std::unique_ptr<Medication> med;
        med = databaseFacade->findMedicationById(1);
        med->print();
        if (med->getId() == 1)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";
        med = databaseFacade->findMedicationById(3);
        med->print();
        if (med->getId() == 3)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";

        std::unique_ptr<PrescribedProcedure> prproc;
        prproc = databaseFacade->findPrescribedProcedureById(1);
        prproc->print();
        if (prproc->getId() == 1)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";
        prproc = databaseFacade->findPrescribedProcedureById(3);
        prproc->print();
        if (prproc->getId() == 3)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";

        std::unique_ptr<Procedure> procedure;
        procedure = databaseFacade->findProcedureById(1);
        procedure->print();
        if (procedure->getId() == 1)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";
        procedure = databaseFacade->findProcedureById(3);
        procedure->print();
        if (procedure->getId() == 3)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";

        std::unique_ptr<TestResult> testResult;
        testResult = databaseFacade->findTestResultById(1);
        testResult->print();
        if (testResult->getId() == 1)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";
        testResult = databaseFacade->findTestResultById(3);
        testResult->print();
        if (testResult->getId() == 3)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";

        std::unique_ptr<Test> test;
        test = databaseFacade->findTestById(1);
        test->print();
        if (test->getId() == 1)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";
        test = databaseFacade->findTestById(3);
        test->print();
        if (test->getId() == 3)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";

        std::unique_ptr<Diagnosis> diagnosis;
        diagnosis = databaseFacade->findDiagnosisById(1);
        diagnosis->print();
        if (diagnosis->getId() == 1)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";
        diagnosis = databaseFacade->findDiagnosisById(3);
        diagnosis->print();
        if (diagnosis->getId() == 3)
            std::cout << "SUCCESS\n";
        else
            std::cout << "NOT SUCCESS\n";
    }
};