#pragma once

#include "DatabaseFacade.h"
#include "DatabaseTest.h"

class InsertTest : public DatabaseTest
{
public:
    InsertTest(DatabaseFacade* databaseFacade) : DatabaseTest(databaseFacade) {}
    void start() override
    {
        insertWeekdays();
        insertSpecializations();
        insertDoctors();
        insertAppointments();
        insertPatients();
        insertMedications();
        insertVisits();
        insertPrescribedMedications();
        insertProcedures();
        insertPrescribedProcedures();
        insertTests();
        insertTestResults();
        insertDiagnosis();
    }

private:
    void insertTestResults()
    {
        databaseFacade->addTestResult(1, 1,
                                      "Everything's fine, no abnormalities.");
        databaseFacade->addTestResult(2, 2,
                                      "Some above-average readings, retest");
        databaseFacade->addTestResult(3, 3, "Some below average, retreatment");
        databaseFacade->addTestResult(4, 4, "Pathology detected");
        databaseFacade->addTestResult(5, 5, "Tests were taken incorrectly");
    }

    void insertTests()
    {
        databaseFacade->addTest("Blood test");
        databaseFacade->addTest("HBA1C test");
        databaseFacade->addTest("Liver function test");
        databaseFacade->addTest("Thyroid profile test");
        databaseFacade->addTest("HCG test");
    }

    void insertPrescribedProcedures()
    {
        databaseFacade->addPrescribedProcedure(1, 1, 10);
        databaseFacade->addPrescribedProcedure(1, 2, 2);
        databaseFacade->addPrescribedProcedure(1, 3, 1);
        databaseFacade->addPrescribedProcedure(2, 4, 8);
        databaseFacade->addPrescribedProcedure(3, 5, 3);
        databaseFacade->addPrescribedProcedure(4, 1, 1);
        databaseFacade->addPrescribedProcedure(5, 2, 1);
    }

    void insertProcedures()
    {
        databaseFacade->addProcedure("Physiotherapy");
        databaseFacade->addProcedure("Rest");
        databaseFacade->addProcedure("Magnetic resonance imaging");
        databaseFacade->addProcedure("X-ray");
        databaseFacade->addProcedure("Breathing exercises");
    }

    void insertPrescribedMedications()
    {
        databaseFacade->addPrescribedMedication(1, 1);
        databaseFacade->addPrescribedMedication(1, 2);
        databaseFacade->addPrescribedMedication(1, 3);
        databaseFacade->addPrescribedMedication(2, 2);
        databaseFacade->addPrescribedMedication(3, 3);
        databaseFacade->addPrescribedMedication(4, 4);
        databaseFacade->addPrescribedMedication(5, 5);
    }

    void insertVisits()
    {
        bool isOnlyTime = false;
        databaseFacade->addVisit(1, 1, "Severe headaches",
                                 DateTime("2023-12-01 10:00", isOnlyTime),
                                 DateTime("2023-12-03 11:00", isOnlyTime),
                                 DateTime("2023-12-03 11:05", isOnlyTime));
        databaseFacade->addVisit(2, 2, "Muscle spasms",
                                 DateTime("2023-12-01 18:00", isOnlyTime),
                                 DateTime("2023-12-02 12:45", isOnlyTime),
                                 DateTime("2023-12-02 13:00", isOnlyTime));
        databaseFacade->addVisit(3, 3, "Dizzines and loss of consciousness",
                                 DateTime("2023-12-03 18:12", isOnlyTime),
                                 DateTime("2023-12-06 09:00", isOnlyTime),
                                 DateTime("2023-12-06 11:55", isOnlyTime));
        databaseFacade->addVisit(4, 4, "Weakness, apathy",
                                 DateTime("2023-12-04 11:11", isOnlyTime),
                                 DateTime("2023-12-04 12:45", isOnlyTime),
                                 DateTime("2023-12-04 13:45", isOnlyTime));
        databaseFacade->addVisit(5, 5, "Drowsiness, loss of attention",
                                 DateTime("2023-12-07 19:00", isOnlyTime),
                                 DateTime("2023-12-11 19:00", isOnlyTime),
                                 DateTime("2023-12-11 20:05", isOnlyTime));
    }

    void insertMedications()
    {
        databaseFacade->addMedication("Atorvastatin");
        databaseFacade->addMedication("Levothyroxine");
        databaseFacade->addMedication("Metformin");
        databaseFacade->addMedication("Lisinopril");
        databaseFacade->addMedication("Amlodipine");
        databaseFacade->addMedication("Metoprolol");
        databaseFacade->addMedication("Albuterol");
    }

    void insertPatients()
    {
        databaseFacade->addPatient(
            "Shpeyser", "Egor", "Vladimirovich",
            "Novosibirsk, Koshurnikova st., building 101, flat 245");
        databaseFacade->addPatient(
            "Tomm", "Ivan", "Ivanovich",
            "Novosibirsk, Koshurnikova st., building 12, flat 8");
        databaseFacade->addPatient(
            "Dubinin", "Vladimir", "Sergeevich",
            "Novosibirsk, Stofato st., building 85, flat 25");
        databaseFacade->addPatient(
            "Demkin", "Egor", "Dmitrievich",
            "Novosibirsk, Sovetskaya st., building 80, flat 55");
        databaseFacade->addPatient(
            "Kuptsov", "Nikolay", "Nikolaevich",
            "Novosibirsk, Esenina st., building 1, flat 1");
    }

    void insertAppointments()
    {
        bool isOnlyTime = true;
        databaseFacade->addAppointment(1, 1, DateTime("15:30", isOnlyTime),
                                       DateTime("18:00", isOnlyTime), 12, 101);
        databaseFacade->addAppointment(1, 2, DateTime("08:30", isOnlyTime),
                                       DateTime("16:30", isOnlyTime), 12, 101);
        databaseFacade->addAppointment(1, 3, DateTime("15:30", isOnlyTime),
                                       DateTime("19:00", isOnlyTime), 12, 101);
        databaseFacade->addAppointment(2, 4, DateTime("18:30", isOnlyTime),
                                       DateTime("21:30", isOnlyTime), 18, 123);
        databaseFacade->addAppointment(2, 5, DateTime("19:30", isOnlyTime),
                                       DateTime("22:30", isOnlyTime), 18, 123);
        databaseFacade->addAppointment(2, 6, DateTime("13:30", isOnlyTime),
                                       DateTime("17:00", isOnlyTime), 19, 123);
        databaseFacade->addAppointment(3, 1, DateTime("9:00", isOnlyTime),
                                       DateTime("14:15", isOnlyTime), 25, 111);
        databaseFacade->addAppointment(3, 5, DateTime("15:30", isOnlyTime),
                                       DateTime("18:30", isOnlyTime), 25, 111);
        databaseFacade->addAppointment(4, 3, DateTime("10:30", isOnlyTime),
                                       DateTime("11:45", isOnlyTime), 85, 234);
        databaseFacade->addAppointment(5, 1, DateTime("15:30", isOnlyTime),
                                       DateTime("20:00", isOnlyTime), 99, 331);
        databaseFacade->addAppointment(6, 3, DateTime("13:30", isOnlyTime),
                                       DateTime("17:00", isOnlyTime), 100, 5);
    }

    void insertWeekdays()
    {
        databaseFacade->addWeekday("Monday");
        databaseFacade->addWeekday("Tuesday");
        databaseFacade->addWeekday("Wednesday");
        databaseFacade->addWeekday("Thursday");
        databaseFacade->addWeekday("Friday");
        databaseFacade->addWeekday("Saturday");
        databaseFacade->addWeekday("Sunday");
    }

    void insertSpecializations()
    {
        databaseFacade->addSpecialization("Surgeon");
        databaseFacade->addSpecialization("Pediatrician");
        databaseFacade->addSpecialization("Neurosurgeon");
        databaseFacade->addSpecialization("Internist");
        databaseFacade->addSpecialization("Ophthalmologist");
        databaseFacade->addSpecialization("Trauma surgeon");
    }

    void insertDoctors()
    {
        databaseFacade->addDoctor("Ivanov", "Ivan", "Ivanovich", 1);
        databaseFacade->addDoctor("Perelman", "Egor", "Victorovich", 2);
        databaseFacade->addDoctor("Eyfeld", "Evgeny", "Alexsandrovich", 3);
        databaseFacade->addDoctor("Sinitsyn", "Vladimir", "Vladimirovich", 4);
        databaseFacade->addDoctor("Kotov", "Dmitry", "Platonovich", 5);
        databaseFacade->addDoctor("Kuznetsov", "Artyom", "Igorevich", 6);
    }

    void insertDiagnosis()
    {
        databaseFacade->addDiagnosis(1, "Unspecified essential hypertension");
        databaseFacade->addDiagnosis(2, "Lumbago");
        databaseFacade->addDiagnosis(
            3, "Routine general medical examination at a healthcare facility");
        databaseFacade->addDiagnosis(4, "Atrial fibrillation");
        databaseFacade->addDiagnosis(5, "Coronary artery atherosclerosis");
    }
};