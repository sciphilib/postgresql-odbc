#pragma once

#include "DatabaseCache.h"
#include "DatabaseFacade.h"

#include <iomanip>

class DatabaseMenu
{
public:
    DatabaseMenu(DatabaseFacade* databaseFacade)
        : databaseFacade(databaseFacade), entitiesOnPage(3)
    {
    }
    void start();
    void setEntitiesOnPage(int count);

private:
    DatabaseFacade* databaseFacade;
    int entitiesOnPage;

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

    void printDoctors(int page);
    void printFirstNDoctors(int start, int n);
    void printSpecializations(int page);
    void printFirstNSpecializations(int start, int n);
    void printAppointments(int page);
    void printFirstNAppointments(int start, int n);
    void printPatients(int page);
    void printFirstNPatients(int start, int n);
    void printVisits(int page);
    void printFirstNVisits(int start, int n);
    void printDiagnosis(int page);
    void printFirstNDiagnosis(int start, int n);
    void printTests(int page);
    void printFirstNTests(int start, int n);
    void printTestResults(int page);
    void printFirstNTestResults(int start, int n);
    void printProcedures(int page);
    void printFirstNProcedures(int start, int n);
    void printPrescribedProcedures(int page);
    void printFirstNPrescribedProcedures(int start, int n);
    void printMedications(int page);
    void printFirstNMedications(int start, int n);
    void printPrescribedMedications(int page);
    void printFirstNPrescribedMedications(int start, int n);

    void invalidateSpecializations();
    void invalidateDoctors();
    void invalidateAppointments();
    void invalidatePatients();
    void invalidateVisits();
    void invalidateDiagnosis();
    void invalidateTests();
    void invalidateTestResults();
    void invalidateProcedures();
    void invalidatePrescribedProcedures();
    void invalidateMedications();
    void invalidatePrescribedMedications();

    void addDoctorMenu();
    void addSpecializationMenu();
    void addAppointmentMenu();
    void addPatientMenu();
    void addVisitMenu();
    void addDiagnosisMenu();
    void addTestMenu();
    void addTestResultMenu();
    void addProcedureMenu();
    void addPrescribedProcedureMenu();
    void addMedicationMenu();
    void addPrescribedMedicationMenu();

    void printDoctorMenu(int page);
    void printSpecializationMenu(int page);
    void printAppointmentMenu(int page);
    void printPatientMenu(int page);
    void printVisitMenu(int page);
    void printDiagnosisMenu(int page);
    void printTestMenu(int page);
    void printTestResultMenu(int page);
    void printProcedureMenu(int page);
    void printPrescribedProcedureMenu(int page);
    void printMedicationMenu(int page);
    void printPrescribedMedicationMenu(int page);

    void deleteDoctorMenu(int page);
    void deletePatientMenu(int page);

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