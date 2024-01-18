#include "DatabaseMenu.h"
#include <string>

void DatabaseMenu::start()
{
    init();
    mainLoop();
}

void DatabaseMenu::init()
{
    entitiesOnPage = 5;

    doctors.addMultiple(databaseFacade->selectDoctorAll(100, 0));
    isDoctorCacheDirty = false;

    specializations.addMultiple(
        databaseFacade->selectSpecializationAll(100, 0));
    isSpecializationCacheDirty = false;

    appointments.addMultiple(databaseFacade->selectAppointmentAll(100, 0));
    isAppointmentCacheDirty = false;

    patients.addMultiple(databaseFacade->selectPatientAll(100, 0));
    isPatientCacheDirty = false;

    visits.addMultiple(databaseFacade->selectVisitAll(100, 0));
    isVisitCacheDirty = false;

    prmeds.addMultiple(databaseFacade->selectPrMedAll(100, 0));
    isPrMedCacheDirty = false;

    medications.addMultiple(databaseFacade->selectMedicationAll(100, 0));
    isMedicationCacheDirty = false;

    prprocs.addMultiple(databaseFacade->selectPrProcAll(100, 0));
    isPrProcCacheDirty = false;

    procedures.addMultiple(databaseFacade->selectProcedureAll(100, 0));
    isProcedureCacheDirty = false;

    testResults.addMultiple(databaseFacade->selectTestResultAll(100, 0));
    isTestResultCacheDirty = false;

    tests.addMultiple(databaseFacade->selectTestAll(100, 0));
    isTestCacheDirty = false;

    diagnosis.addMultiple(databaseFacade->selectDiagnosisAll(100, 0));
    isDiagnosisCacheDirty = false;
}

int DatabaseMenu::mainLoop()
{
    while (true)
    {
        std::cout << "\n*** Database control menu v. 1.0 ***\n";
        std::cout << "1. Doctors\n";
        std::cout << "2. Specializations\n";
        std::cout << "3. Appointments\n";
        std::cout << "4. Patients\n";
        std::cout << "5. Visits\n";
        std::cout << "6. Diagnosis\n";
        std::cout << "7. Tests\n";
        std::cout << "8. Test results\n";
        std::cout << "9. Procedures\n";
        std::cout << "10. Presribed procedures\n";
        std::cout << "11. Medications\n";
        std::cout << "12. Prescribed medications\n";
        std::cout << "0. Exit from program\n";
        std::cout << "Choose action: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                doctorActivities();
                break;
            case 2:
                specializationActivities();
                break;
            case 3:
                appointmentActivities();
                break;
            case 4:
                patientActivities();
                break;
            case 5:
                visitActivities();
                break;
            case 6:
                diagnosisActivities();
                break;
            case 7:
                testActivities();
                break;
            case 8:
                testResultActivities();
                break;
            case 9:
                procedureActivities();
                break;
            case 10:
                prescribedProcedureActivities();
                break;
            case 11:
                medicationActivities();
                break;
            case 12:
                prescribedMedicationActivities();
                break;
            case 0:
                std::cout << "Exiting...\n";
                return 1;
            default:
                std::cout << "Incorrect input. Choose number "
                             "from 0 to 12 please.\n";
        }
    }
}

int DatabaseMenu::doctorActivities()
{
    int page = 0;
    while (true)
    {
        std::cout << "\n*** Doctor activities ***\n";
        std::cout << "1. Show all doctors\n";
        std::cout << "2. Add doctor\n";
        std::cout << "3. Update doctor\n";
        std::cout << "4. Delete doctor\n";
        std::cout << "5. Back\n";
        std::cout << "Choose action: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                while (1)
                {
                    std::cout << "\nPage " << page + 1 << std::endl;
                    std::cout << "1. Next page\n";
                    std::cout << "2. Previoues page\n";
                    std::cout << "3. Print\n";
                    std::cout << "0. Back\n";
                    std::cout << "Choose action: ";
                    int choice;
                    std::cin >> choice;
                    switch (choice)
                    {
                        case 1:
                            page++;
                            break;
                        case 2:
                            page--;
                            break;
                        case 3:
                            if (isDoctorCacheDirty)
                            {
                                auto doctorList =
                                    databaseFacade->selectDoctorAll(100, 0);
                                doctors.addMultiple(doctorList);
                                isDoctorCacheDirty = false;
                            }
                            printDoctor(page);
                            break;
                        case 0:
                            std::cout << "\n";
                            return 0;
                        default:
                            std::cout
                                << "Incorrect input. Please, choose number "
                                   "from 1 to 5.\n";
                    }
                }
                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:
                std::cout << "\n";
                return 0;
            default:
                std::cout << "Incorrect input. Please, choose number "
                             "from 1 to 5.\n";
        }
    }
}

void printWithBorder(const std::string& text)
{
    int width = 50;
    std::string border(width, '*');
    std::cout << '\n' << border << '\n';
    std::cout << "* " << std::left << std::setw(width - 3) << text << "*\n";
    std::cout << border << '\n';
}

void printFooter()
{
    int width = 50;
    std::string border(width, '*');
    std::cout << '\n' << border << '\n';
}

void DatabaseMenu::setEntitiesOnPage(int count) { entitiesOnPage = count; }

void DatabaseMenu::printFirstNDoctors(int start, int n)
{
    int count = 0, idSpec;
    std::string name, middleName, lastName, spec;
    for (auto it = doctors.begin(); it != doctors.end() && count < start + n;
         ++it)
    {
        if (count >= start)
        {
            auto doctor = it->second;
            name = doctor.getFirstName();
            middleName = doctor.getMiddleName();
            lastName = doctor.getLastName();
            idSpec = doctor.getIdSpec();
            spec = (*(specializations.getFromCacheMap(idSpec))).getName();

            std::cout << '\n'
                      << count + 1 << "." << lastName << " " << name << " "
                      << middleName << std::endl;
            std::cout << "Specialization: " << spec << std::endl;
        }
        ++count;
    }
}

void DatabaseMenu::printDoctor(int page)
{

    std::string header = "Doctors. Page " + std::to_string(page + 1);
    printWithBorder(header);
    printFirstNDoctors(page * entitiesOnPage, entitiesOnPage);
    printFooter();
}

int DatabaseMenu::specializationActivities()
{
    while (true)
    {
        std::cout << "\n*** Specialization activities ***\n";
        std::cout << "1. Show all specs\n";
        std::cout << "2. Add specialization\n";
        std::cout << "3. Update specialization\n";
        std::cout << "4. Delete specialization\n";
        std::cout << "5. Back\n";
        std::cout << "Choose action: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                if (isSpecializationCacheDirty)
                {
                    auto specList =
                        databaseFacade->selectSpecializationAll(100, 0);
                    specializations.addMultiple(specList);
                    isSpecializationCacheDirty = false;
                }
                specializations.printCacheMap();
                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:
                std::cout << "\n";
                return 0;
            default:
                std::cout << "Incorrect input. Please, choose number "
                             "from 1 to 5.\n";
        }
    }
}

int DatabaseMenu::appointmentActivities()
{
    while (true)
    {
        std::cout << "\n*** Appointment activities ***\n";
        std::cout << "1. Show all appointments\n";
        std::cout << "2. Add appointment\n";
        std::cout << "3. Update appointment\n";
        std::cout << "4. Delete appointment\n";
        std::cout << "5. Back\n";
        std::cout << "Choose action: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                if (isAppointmentCacheDirty)
                {
                    auto appList = databaseFacade->selectAppointmentAll(100, 0);
                    appointments.addMultiple(appList);
                    isAppointmentCacheDirty = false;
                }
                appointments.printCacheMap();
                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:
                std::cout << "\n";
                return 0;
            default:
                std::cout << "Incorrect input. Please, choose number "
                             "from 1 to 5.\n";
        }
    }
}

int DatabaseMenu::patientActivities()
{
    while (true)
    {
        std::cout << "\n*** Patient activities ***\n";
        std::cout << "1. Show all patients\n";
        std::cout << "2. Add patient\n";
        std::cout << "3. Update patient\n";
        std::cout << "4. Delete patient\n";
        std::cout << "5. Back\n";
        std::cout << "Choose action: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                if (isPatientCacheDirty)
                {
                    auto patList = databaseFacade->selectPatientAll(100, 0);
                    patients.addMultiple(patList);
                    isPatientCacheDirty = false;
                }
                patients.printCacheMap();
                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:
                std::cout << "\n";
                return 0;
            default:
                std::cout << "Incorrect input. Please, choose number "
                             "from 1 to 5.\n";
        }
    }
}

int DatabaseMenu::visitActivities()
{
    while (true)
    {
        std::cout << "\n*** Visit activities ***\n";
        std::cout << "1. Show all visits\n";
        std::cout << "2. Add visit\n";
        std::cout << "3. Update visit\n";
        std::cout << "4. Delete visit\n";
        std::cout << "5. Back\n";
        std::cout << "Choose action: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                if (isVisitCacheDirty)
                {
                    auto visitsList = databaseFacade->selectVisitAll(100, 0);
                    visits.addMultiple(visitsList);
                    isVisitCacheDirty = false;
                }
                visits.printCacheMap();
                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:
                std::cout << "\n";
                return 0;
            default:
                std::cout << "Incorrect input. Please, choose number "
                             "from 1 to 5.\n";
        }
    }
}

int DatabaseMenu::prescribedMedicationActivities()
{
    while (true)
    {
        std::cout << "\n*** Prescribed Medication activities ***\n";
        std::cout << "1. Show all prescribed medications\n";
        std::cout << "2. Add prescribed medication\n";
        std::cout << "3. Update prescribed medication\n";
        std::cout << "4. Delete prescribed medication\n";
        std::cout << "5. Back\n";
        std::cout << "Choose action: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                if (isPrMedCacheDirty)
                {
                    auto prMedsList = databaseFacade->selectPrMedAll(100, 0);
                    prmeds.addMultiple(prMedsList);
                    isPrMedCacheDirty = false;
                }
                prmeds.print();
                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:
                std::cout << "\n";
                return 0;
            default:
                std::cout << "Incorrect input. Please, choose number "
                             "from 1 to 5.\n";
        }
    }
}

int DatabaseMenu::medicationActivities()
{
    while (true)
    {
        std::cout << "\n*** Medication activities ***\n";
        std::cout << "1. Show all medications\n";
        std::cout << "2. Add medication\n";
        std::cout << "3. Update medication\n";
        std::cout << "4. Delete medication\n";
        std::cout << "5. Back\n";
        std::cout << "Choose action: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                if (isMedicationCacheDirty)
                {
                    auto medicationsList =
                        databaseFacade->selectMedicationAll(100, 0);
                    medications.addMultiple(medicationsList);
                    isMedicationCacheDirty = false;
                }
                medications.print();
                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:
                std::cout << "\n";
                return 0;
            default:
                std::cout << "Incorrect input. Please, choose number "
                             "from 1 to 5.\n";
        }
    }
}

int DatabaseMenu::prescribedProcedureActivities()
{
    while (true)
    {
        std::cout << "\n*** Prescribed Procedures activities ***\n";
        std::cout << "1. Show all prescribed procedures\n";
        std::cout << "2. Add prescribed procedure\n";
        std::cout << "3. Update prescribed procedure\n";
        std::cout << "4. Delete prescribed procedure\n";
        std::cout << "5. Back\n";
        std::cout << "Choose action: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                if (isPrProcCacheDirty)
                {
                    auto prprocsList = databaseFacade->selectPrProcAll(100, 0);
                    prprocs.addMultiple(prprocsList);
                    isPrProcCacheDirty = false;
                }
                prprocs.print();
                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:
                std::cout << "\n";
                return 0;
            default:
                std::cout << "Incorrect input. Please, choose number "
                             "from 1 to 5.\n";
        }
    }
}

int DatabaseMenu::procedureActivities()
{
    while (true)
    {
        std::cout << "\n*** Procedures activities ***\n";
        std::cout << "1. Show all procedures\n";
        std::cout << "2. Add procedure\n";
        std::cout << "3. Update procedure\n";
        std::cout << "4. Delete procedure\n";
        std::cout << "5. Back\n";
        std::cout << "Choose action: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                if (isProcedureCacheDirty)
                {
                    auto proceduresList =
                        databaseFacade->selectProcedureAll(100, 0);
                    procedures.addMultiple(proceduresList);
                    isProcedureCacheDirty = false;
                }
                procedures.print();
                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:
                std::cout << "\n";
                return 0;
            default:
                std::cout << "Incorrect input. Please, choose number "
                             "from 1 to 5.\n";
        }
    }
}

int DatabaseMenu::testResultActivities()
{
    while (true)
    {
        std::cout << "\n*** Test Results activities ***\n";
        std::cout << "1. Show all test results\n";
        std::cout << "2. Add test result\n";
        std::cout << "3. Update test result\n";
        std::cout << "4. Delete test result\n";
        std::cout << "5. Back\n";
        std::cout << "Choose action: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                if (isTestResultCacheDirty)
                {
                    auto testResultsList =
                        databaseFacade->selectTestResultAll(100, 0);
                    testResults.addMultiple(testResultsList);
                    isTestResultCacheDirty = false;
                }
                testResults.print();
                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:
                std::cout << "\n";
                return 0;
            default:
                std::cout << "Incorrect input. Please, choose number "
                             "from 1 to 5.\n";
        }
    }
}

int DatabaseMenu::testActivities()
{
    while (true)
    {
        std::cout << "\n*** Tests activities ***\n";
        std::cout << "1. Show all tests\n";
        std::cout << "2. Add test\n";
        std::cout << "3. Update test\n";
        std::cout << "4. Delete test\n";
        std::cout << "5. Back\n";
        std::cout << "Choose action: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                if (isTestCacheDirty)
                {
                    auto testsList = databaseFacade->selectTestAll(100, 0);
                    tests.addMultiple(testsList);
                    isTestCacheDirty = false;
                }
                tests.print();
                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:
                std::cout << "\n";
                return 0;
            default:
                std::cout << "Incorrect input. Please, choose number "
                             "from 1 to 5.\n";
        }
    }
}

int DatabaseMenu::diagnosisActivities()
{
    while (true)
    {
        std::cout << "\n*** Diagnosis activities ***\n";
        std::cout << "1. Show all diagnosis\n";
        std::cout << "2. Add diagnosis\n";
        std::cout << "3. Update diagnosis\n";
        std::cout << "4. Delete diagnosis\n";
        std::cout << "5. Back\n";
        std::cout << "Choose action: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                if (isDiagnosisCacheDirty)
                {
                    auto diagnosisList =
                        databaseFacade->selectDiagnosisAll(100, 0);
                    diagnosis.addMultiple(diagnosisList);
                    isDiagnosisCacheDirty = false;
                }
                diagnosis.print();
                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:
                std::cout << "\n";
                return 0;
            default:
                std::cout << "Incorrect input. Please, choose number "
                             "from 1 to 5.\n";
        }
    }
}