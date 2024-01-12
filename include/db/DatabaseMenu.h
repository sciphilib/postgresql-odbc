#pragma once

#include "DatabaseCache.h"
#include "DatabaseFacade.h"

class DatabaseMenu
{
public:
    DatabaseMenu(DatabaseFacade* databaseFacade)
        : databaseFacade(databaseFacade)
    {
    }
    void start();

private:
    DatabaseFacade* databaseFacade;

    bool isDoctorCacheDirty;
    bool isSpecializationCacheDirty;
    bool isPatientCacheDirty;
    bool isAppointmentCacheDirty;
    bool isDiagnosisCacheDirty;
    bool isMedicationCacheDirty;
    bool isPrMedCacheDirty;
    bool isPrProcCacheDirty;
    bool isProcedureCacheDirty;
    bool isTestCacheDirty;
    bool isTestResultCacheDirty;
    bool isVisitCacheDirty;

    void init();
    int mainLoop();
    int doctorActivities();
    int specializationActivities();
    int patientActivities();
    int appointmentActivities();
    int diagnosisActivities();
    int medicationActivities();
    int prescribedMedicationActivities();
    int prescribedProcedureActivities();
    int procedureActivities();
    int testActivities();
    int testResultActivities();
    int visitActivities();

    DatabaseCache<Appointment> appointments;
    DatabaseCache<Diagnosis> diagnosis;
    DatabaseCache<Doctor> doctors;
    DatabaseCache<Medication> medications;
    DatabaseCache<Patient> patients;
    DatabaseCache<PrescribedMedication> prmeds;
    DatabaseCache<PrescribedProcedure> prprocs;
    DatabaseCache<Procedure> procedures;
    DatabaseCache<Specialization> specializations;
    DatabaseCache<Test> tests;
    DatabaseCache<TestResult> testResults;
    DatabaseCache<Visit> visits;
};