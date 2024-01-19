#include "DatabaseFacade.h"
#include <memory>
#include <string>
#include <unordered_map>

void DatabaseFacade::addWeekday(const std::string& dayName)
{
    std::map<std::string, std::string> parameters{{"dayName", dayName}};

    auto weekday = mainFactory.createObject("Weekday", parameters);

    weekdayTDG->insert(*weekday);
}

void DatabaseFacade::addSpecialization(const std::string& name)
{
    std::map<std::string, std::string> parameters{{"name", name}};

    auto spec = mainFactory.createObject("Specialization", parameters);

    specializationTDG->insert(*spec);
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

    doctorTDG->insert(*doctor);
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

    appointmentTDG->insert(*appointment);
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

    patientTDG->insert(*patient);
}

void DatabaseFacade::addMedication(const std::string& name)
{
    std::map<std::string, std::string> parameters{{"name", name}};

    auto medication = mainFactory.createObject("Medication", parameters);

    medicationTDG->insert(*medication);
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

    visitTDG->insert(*visit);
}

void DatabaseFacade::addPrescribedMedication(int idVisit, int idMedication)
{
    std::map<std::string, std::string> parameters{
        {"id_visit", std::to_string(idVisit)},
        {"id_medication", std::to_string(idMedication)}};

    auto prMedication =
        mainFactory.createObject("PrescribedMedication", parameters);

    prescribedMedicationTDG->insert(*prMedication);
}

void DatabaseFacade::addProcedure(const std::string& name)
{
    std::map<std::string, std::string> parameters{{"name", name}};

    auto procedure = mainFactory.createObject("Procedure", parameters);

    procedureTDG->insert(*procedure);
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

    prescribedProcedureTDG->insert(*prescribedProcedure);
}

void DatabaseFacade::addTest(const std::string& name)
{
    std::map<std::string, std::string> parameters{{"name", name}};

    auto test = mainFactory.createObject("Test", parameters);

    testTDG->insert(*test);
}

void DatabaseFacade::addTestResult(int idVisit, int idTest,
                                   const std::string& result)
{
    std::map<std::string, std::string> parameters{
        {"id_visit", std::to_string(idVisit)},
        {"id_test", std::to_string(idTest)},
        {"result", result}};

    auto testResult = mainFactory.createObject("TestResult", parameters);

    testResultTDG->insert(*testResult);
}

void DatabaseFacade::addDiagnosis(int idVisit, const std::string& description)
{
    std::map<std::string, std::string> parameters{
        {"id_visit", std::to_string(idVisit)}, {"description", description}};

    auto diagnosis = mainFactory.createObject("Diagnosis", parameters);

    diagnosisTDG->insert(*diagnosis);
}

std::unique_ptr<Appointment> DatabaseFacade::findAppointmentById(int id)
{
    std::unique_ptr<BaseObject> baseObject = appointmentTDG->findById(id);
    if (baseObject)
        return std::unique_ptr<Appointment>(
            static_cast<Appointment*>(baseObject.release()));
    else
        return nullptr;
}

std::unique_ptr<Weekday> DatabaseFacade::findWeekdayById(int id)
{
    std::unique_ptr<BaseObject> baseObject = weekdayTDG->findById(id);
    if (baseObject)
        return std::unique_ptr<Weekday>(
            static_cast<Weekday*>(baseObject.release()));
    else
        return nullptr;
}

std::unique_ptr<Specialization> DatabaseFacade::findSpecializationById(int id)
{
    std::unique_ptr<BaseObject> baseObject = specializationTDG->findById(id);
    if (baseObject)
        return std::unique_ptr<Specialization>(
            static_cast<Specialization*>(baseObject.release()));
    else
        return nullptr;
}

std::unique_ptr<Doctor> DatabaseFacade::findDoctorById(int id)
{
    std::unique_ptr<BaseObject> baseObject = doctorTDG->findById(id);
    if (baseObject)
        return std::unique_ptr<Doctor>(
            static_cast<Doctor*>(baseObject.release()));
    else
        return nullptr;
}

std::unique_ptr<Patient> DatabaseFacade::findPatientById(int id)
{
    std::unique_ptr<BaseObject> baseObject = patientTDG->findById(id);
    if (baseObject)
        return std::unique_ptr<Patient>(
            static_cast<Patient*>(baseObject.release()));
    else
        return nullptr;
}

std::unique_ptr<Visit> DatabaseFacade::findVisitById(int id)
{
    std::unique_ptr<BaseObject> baseObject = visitTDG->findById(id);
    if (baseObject)
        return std::unique_ptr<Visit>(
            static_cast<Visit*>(baseObject.release()));
    else
        return nullptr;
}

std::unique_ptr<PrescribedMedication>
DatabaseFacade::findPrescribedMedicationById(int id)
{
    std::unique_ptr<BaseObject> baseObject =
        prescribedMedicationTDG->findById(id);
    if (baseObject)
        return std::unique_ptr<PrescribedMedication>(
            static_cast<PrescribedMedication*>(baseObject.release()));
    else
        return nullptr;
}

std::unique_ptr<Medication> DatabaseFacade::findMedicationById(int id)
{
    std::unique_ptr<BaseObject> baseObject = medicationTDG->findById(id);
    if (baseObject)
        return std::unique_ptr<Medication>(
            static_cast<Medication*>(baseObject.release()));
    else
        return nullptr;
}

std::unique_ptr<PrescribedProcedure>
DatabaseFacade::findPrescribedProcedureById(int id)
{
    std::unique_ptr<BaseObject> baseObject =
        prescribedProcedureTDG->findById(id);
    if (baseObject)
        return std::unique_ptr<PrescribedProcedure>(
            static_cast<PrescribedProcedure*>(baseObject.release()));
    else
        return nullptr;
}

std::unique_ptr<Procedure> DatabaseFacade::findProcedureById(int id)
{
    std::unique_ptr<BaseObject> baseObject = procedureTDG->findById(id);
    if (baseObject)
        return std::unique_ptr<Procedure>(
            static_cast<Procedure*>(baseObject.release()));
    else
        return nullptr;
}

std::unique_ptr<TestResult> DatabaseFacade::findTestResultById(int id)
{
    std::unique_ptr<BaseObject> baseObject = testResultTDG->findById(id);
    if (baseObject)
        return std::unique_ptr<TestResult>(
            static_cast<TestResult*>(baseObject.release()));
    else
        return nullptr;
}

std::unique_ptr<Test> DatabaseFacade::findTestById(int id)
{
    std::unique_ptr<BaseObject> baseObject = testTDG->findById(id);
    if (baseObject)
        return std::unique_ptr<Test>(static_cast<Test*>(baseObject.release()));
    else
        return nullptr;
}

std::unique_ptr<Diagnosis> DatabaseFacade::findDiagnosisById(int id)
{
    std::unique_ptr<BaseObject> baseObject = diagnosisTDG->findById(id);
    if (baseObject)
        return std::unique_ptr<Diagnosis>(
            static_cast<Diagnosis*>(baseObject.release()));
    else
        return nullptr;
}

std::unordered_map<int, Doctor> DatabaseFacade::selectDoctorAll(int limit,
                                                                int offset)
{
    return doctorTDG->select(limit, offset);
}

std::unordered_map<int, Specialization>
DatabaseFacade::selectSpecializationAll(int limit, int offset)
{
    return specializationTDG->select(limit, offset);
}

std::unordered_map<int, PrescribedMedication>
DatabaseFacade::selectPrMedAll(int limit, int offset)
{
    return prescribedMedicationTDG->select(limit, offset);
}

std::unordered_map<int, TestResult>
DatabaseFacade::selectTestResultAll(int limit, int offset)
{
    return testResultTDG->select(limit, offset);
}

std::unordered_map<int, Test> DatabaseFacade::selectTestAll(int limit,
                                                            int offset)
{
    return testTDG->select(limit, offset);
}

std::unordered_map<int, Diagnosis>
DatabaseFacade::selectDiagnosisAll(int limit, int offset)
{
    return diagnosisTDG->select(limit, offset);
}

std::unordered_map<int, Procedure>
DatabaseFacade::selectProcedureAll(int limit, int offset)
{
    return procedureTDG->select(limit, offset);
}

std::unordered_map<int, Appointment>
DatabaseFacade::selectAppointmentAll(int limit, int offset)
{
    return appointmentTDG->select(limit, offset);
}

std::unordered_map<int, Medication>
DatabaseFacade::selectMedicationAll(int limit, int offset)
{
    return medicationTDG->select(limit, offset);
}

std::unordered_map<int, PrescribedProcedure>
DatabaseFacade::selectPrProcAll(int limit, int offset)
{

    return prescribedProcedureTDG->select(limit, offset);
}

std::unordered_map<int, Patient> DatabaseFacade::selectPatientAll(int limit,
                                                                  int offset)
{
    return patientTDG->select(limit, offset);
}

std::unordered_map<int, Visit> DatabaseFacade::selectVisitAll(int limit,
                                                              int offset)
{
    return visitTDG->select(limit, offset);
}

bool DatabaseFacade::updateAppointment(int id, const Appointment& appointment)
{
    std::unique_ptr<BaseObject> baseObject = appointmentTDG->findById(id);
    if (baseObject)
        return appointmentTDG->update(id, appointment);
    else
        return false;
}

bool DatabaseFacade::updateDiagnosis(int id, const Diagnosis& diagnosis)
{
    std::unique_ptr<BaseObject> baseObject = diagnosisTDG->findById(id);
    if (baseObject)
        return diagnosisTDG->update(id, diagnosis);
    else
        return false;
}

bool DatabaseFacade::updateDoctor(int id, const Doctor& doctor)
{
    std::unique_ptr<BaseObject> baseObject = doctorTDG->findById(id);
    if (baseObject)
        return doctorTDG->update(id, doctor);
    else
        return false;
}

bool DatabaseFacade::updateMedication(int id, const Medication& medication)
{
    std::unique_ptr<BaseObject> baseObject = medicationTDG->findById(id);
    if (baseObject)
        return medicationTDG->update(id, medication);
    else
        return false;
}

bool DatabaseFacade::updatePatient(int id, const Patient& patient)
{
    std::unique_ptr<BaseObject> baseObject = patientTDG->findById(id);
    if (baseObject)
        return patientTDG->update(id, patient);
    else
        return false;
}

bool DatabaseFacade::deleteDoctor(int id) { return doctorTDG->deleteById(id); }

bool DatabaseFacade::deletePatient(int id) { return true; }