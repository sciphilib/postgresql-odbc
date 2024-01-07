#pragma once

#include "Appointment.h"
#include "DatabaseTest.h"
#include "Diagnosis.h"
#include "Doctor.h"
#include "Medication.h"
#include "Patient.h"

class UpdateTest : public DatabaseTest
{
public:
    UpdateTest(DatabaseFacade* databaseFacade) : DatabaseTest(databaseFacade) {}
    void start() override
    {
        updateAppointment();
        updateDiagnosis();
        updateDoctor();
        updateMedication();
        updatePatient();
    }

private:
    void updateAppointment()
    {
        std::cout << "Before update:\n";
        std::unique_ptr<Appointment> appointment;
        appointment = databaseFacade->findAppointmentById(1);
        appointment->print();
        appointment->setIdDoctor(2);
        appointment->setBeginDate(DateTime("08:30", true));
        appointment->setOffice(1212);
        databaseFacade->updateAppointment(1, *appointment);
        appointment = databaseFacade->findAppointmentById(1);
        std::cout << "After update:\n";
        appointment->print();
    }

    void updateDiagnosis()
    {
        std::cout << "Before update:\n";
        std::unique_ptr<Diagnosis> diagnosis;
        diagnosis = databaseFacade->findDiagnosisById(1);
        diagnosis->print();
        diagnosis->setIdVisit(3);
        std::string description("new description");
        diagnosis->setDescription(description);
        databaseFacade->updateDiagnosis(1, *diagnosis);
        diagnosis = databaseFacade->findDiagnosisById(1);
        std::cout << "After update:\n";
        diagnosis->print();
    }

    void updateDoctor()
    {
        std::cout << "Before update:\n";
        std::unique_ptr<Doctor> doctor;
        doctor = databaseFacade->findDoctorById(2);
        doctor->print();
        std::string lastName("NewLastName");
        std::string middleName("NewMiddleName");
        std::string firstName("NewFirstName");
        doctor->setLastName(lastName);
        doctor->setFirstName(firstName);
        doctor->setMiddleName(middleName);
        doctor->setIdSpec(3);
        databaseFacade->updateDoctor(2, *doctor);
        std::cout << "After update:\n";
        doctor->print();
    }

    void updateMedication()
    {
        std::cout << "Before update:\n";
        std::unique_ptr<Medication> medication;
        medication = databaseFacade->findMedicationById(3);
        medication->print();
        std::string name("New medication");
        medication->setName(name);
        databaseFacade->updateMedication(3, *medication);
        std::cout << "After update:\n";
        medication->print();
    }

    void updatePatient()
    {
        std::cout << "Before update:\n";
        std::unique_ptr<Patient> patient;
        patient = databaseFacade->findPatientById(4);
        patient->print();
        std::string lastName("NewLastName");
        std::string middleName("NewMiddleName");
        std::string firstName("NewFirstName");
        std::string address("NewAddress");
        patient->setLastName(lastName);
        patient->setFirstName(firstName);
        patient->setMiddleName(middleName);
        patient->setAddress(address);
        databaseFacade->updatePatient(4, *patient);
        std::cout << "After update:\n";
        patient->print();
    }
};