#include "DatabaseFacade.h"
#include <bits/types/struct_sched_param.h>
#include <string>

void DatabaseFacade::addWeekday(const std::string& dayName)
{
    std::map<std::string, std::string> parameters{{"dayName", dayName}};

    auto weekday = mainFactory.createObject("Weekday", parameters);

    WeekdayTDG weekdayTDG(hDbc_);
    weekdayTDG.insert(*weekday);

    // to do: use get method to get and assign id of object
}

void DatabaseFacade::addSpecialization(const std::string& name)
{
    std::map<std::string, std::string> parameters{{"name", name}};

    auto spec = mainFactory.createObject("Specialization", parameters);

    SpecializationTDG specializationTDG(hDbc_);
    specializationTDG.insert(*spec);
}

void DatabaseFacade::addDoctor(const std::string& lastName,
                               const std::string& firstName,
                               const std::string& middleName, int idSpec)
{
    std::map<std::string, std::string> parameters{
        {"lastName", lastName},
        {"firstName", firstName},
        {"middleName", middleName},
        {"idSpec", std::to_string(idSpec)}};

    auto doctor = mainFactory.createObject("Doctor", parameters);

    DoctorTDG doctorTDG(hDbc_);
    doctorTDG.insert(*doctor);
}

void DatabaseFacade::addAppointment(int idDoctor, int idWeekday,
                                    const DateTime& beginDate,
                                    const DateTime& endDate, int office,
                                    int district)
{
    std::map<std::string, std::string> parameters{
        {"idDoctor", std::to_string(idDoctor)},
        {"idWeekday", std::to_string(idWeekday)},
        {"beginDate", beginDate.toString()},
        {"endDate", endDate.toString()},
        {"office", std::to_string(office)},
        {"district", std::to_string(district)}};

    auto appointment = mainFactory.createObject("Appointment", parameters);

    AppointmentTDG appointmentTDG(hDbc_);
    appointmentTDG.insert(*appointment);
}

void DatabaseFacade::addPatient(const std::string& lastName,
                                const std::string& firstName,
                                const std::string& middleName,
                                const std::string& address)
{
    std::map<std::string, std::string> parameters{{"lastName", lastName},
                                                  {"firstName", firstName},
                                                  {"middleName", middleName},
                                                  {"address", address}};
    auto patient = mainFactory.createObject("Patient", parameters);

    PatientTDG patientTDG(hDbc_);
    patientTDG.insert(*patient);
}

void DatabaseFacade::addMedication(const std::string& name)
{
    std::map<std::string, std::string> parameters{{"name", name}};

    auto medication = mainFactory.createObject("Medication", parameters);

    MedicationTDG medicationTDG(hDbc_);
    medicationTDG.insert(*medication);
}

void DatabaseFacade::addVisit(int idPatient, int idDoctor,
                              const std::string& complaints,
                              const DateTime& dateVisit,
                              const DateTime& dateDischarge,
                              const DateTime& dateClose)
{
    std::map<std::string, std::string> parameters{
        {"id_patient", std::to_string(idPatient)},
        {"id_doctor", std::to_string(idDoctor)},
        {"complaints", complaints},
        {"date_visit", dateVisit.toString()},
        {"date_discharge", dateDischarge.toString()},
        {"date_close", dateClose.toString()}};

    auto visit = mainFactory.createObject("Visit", parameters);

    VisitTDG visitTDG(hDbc_);
    visitTDG.insert(*visit);
}

void DatabaseFacade::addPrescribedMedication(int idVisit, int idMedication)
{
    std::map<std::string, std::string> parameters{
        {"id_visit", std::to_string(idVisit)},
        {"id_medication", std::to_string(idMedication)}};

    auto prMedication =
        mainFactory.createObject("PrescribedMedication", parameters);

    PrescribedMedicationTDG prmedTDG(hDbc_);
    prmedTDG.insert(*prMedication);
}

void DatabaseFacade::addProcedure(const std::string& name)
{
    std::map<std::string, std::string> parameters{{"name", name}};

    auto procedure = mainFactory.createObject("Procedure", parameters);

    ProcedureTDG procedureTDG(hDbc_);
    procedureTDG.insert(*procedure);
}

void DatabaseFacade::addPrescribedProcedure(int idVisit, int idProcedure,
                                            int count)
{
    std::map<std::string, std::string> parameters{
        {"id_visit", std::to_string(idVisit)},
        {"id_procedure", std::to_string(idProcedure)},
        {"count", std::to_string(count)}};

    auto prescribedProcedure =
        mainFactory.createObject("PrescribedProcedure", parameters);

    PrescribedProcedureTDG prprocTDG(hDbc_);
    prprocTDG.insert(*prescribedProcedure);
}

void DatabaseFacade::addTest(const std::string& name)
{
    std::map<std::string, std::string> parameters{{"name", name}};

    auto test = mainFactory.createObject("Test", parameters);

    TestTDG testTDG(hDbc_);
    testTDG.insert(*test);
}

void DatabaseFacade::addTestResult(int idVisit, int idTest,
                                   const std::string& result)
{
    std::map<std::string, std::string> parameters{
        {"id_visit", std::to_string(idVisit)},
        {"id_test", std::to_string(idTest)},
        {"result", result}};

    auto testResult = mainFactory.createObject("TestResult", parameters);

    TestResultTDG testResultTDG(hDbc_);
    testResultTDG.insert(*testResult);
}

void DatabaseFacade::addDiagnosis(int idVisit, const std::string& description)
{
    std::map<std::string, std::string> parameters{
        {"id_visit", std::to_string(idVisit)}, {"description", description}};
    
    auto diagnosis = mainFactory.createObject("Diagnosis", parameters);

    DiagnosisTDG diagnosisTDG(hDbc_);
    diagnosisTDG.insert(*diagnosis);
}