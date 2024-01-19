#include "DatabaseMenu.h"
#include <string>
#include <sys/types.h>

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
                    std::cout << "2. Previous page\n";
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
                            if (page > 0)
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
                            printDoctors(page);
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

void DatabaseMenu::printFirstNPrescribedMedications(int start, int n)
{
    int count = 0, idVisit, idMedication;
    std::string medicationName, doctorFirstName, doctorMiddleName,
        doctorLastName, patientFirstName, patientMiddleName, patientLastName;
    for (auto it = prmeds.begin(); it != prmeds.end() && count < start + n;
         ++it)
    {
        if (count >= start)
        {
            auto prmed = it->second;
            idMedication = prmed.getIdMedication();
            idVisit = prmed.getIdVisit();
            auto visit = *(visits.getFromCacheMap(idVisit));
            auto medication = *(medications.getFromCacheMap(idMedication));
            medicationName = medication.getName();
            auto patient = *(patients.getFromCacheMap(visit.getIdPatient()));
            patientFirstName = patient.getFirstName();
            patientMiddleName = patient.getMiddleName();
            patientLastName = patient.getLastName();
            auto doctor = *(doctors.getFromCacheMap(visit.getIdDoctor()));
            doctorFirstName = doctor.getFirstName();
            doctorMiddleName = doctor.getMiddleName();
            doctorLastName = doctor.getLastName();

            std::cout << '\n' << count + 1 << "." << std::endl;
            std::cout << "Patient: " << patientLastName << " "
                      << patientFirstName << " " << patientMiddleName
                      << std::endl;
            std::cout << "Doctor : " << doctorLastName << " " << doctorFirstName
                      << " " << doctorMiddleName << std::endl;
            std::cout << "Medication: " << medicationName << std::endl;
        }
        ++count;
    }
}

void DatabaseMenu::printFirstNMedications(int start, int n)
{
    int count = 0;
    std::string name;
    for (auto it = medications.begin();
         it != medications.end() && count < start + n; ++it)
    {
        if (count >= start)
        {
            auto medication = it->second;
            name = medication.getName();

            std::cout << '\n' << count + 1 << "." << name << std::endl;
        }
        ++count;
    }
}

void DatabaseMenu::printFirstNPrescribedProcedures(int start, int n)
{
    int count = 0, procedureCount, idVisit, idProcedure;
    std::string procedureName, patientFirstName, patientMiddleName,
        patientLastName, doctorFirstName, doctorMiddleName, doctorLastName;
    for (auto it = prprocs.begin(); it != prprocs.end() && count < start + n;
         ++it)
    {
        if (count >= start)
        {
            auto prProcedure = it->second;
            procedureCount = prProcedure.getCount();
            idVisit = prProcedure.getIdVisit();
            idProcedure = prProcedure.getIdProcedure();
            auto procedure = *(procedures.getFromCacheMap(idProcedure));
            procedureName = procedure.getName();
            auto visit = *(visits.getFromCacheMap(idVisit));
            auto patient = *(patients.getFromCacheMap(visit.getIdPatient()));
            patientFirstName = patient.getFirstName();
            patientMiddleName = patient.getMiddleName();
            patientLastName = patient.getLastName();
            auto doctor = *(doctors.getFromCacheMap(visit.getIdDoctor()));
            doctorFirstName = doctor.getFirstName();
            doctorMiddleName = doctor.getMiddleName();
            doctorLastName = doctor.getLastName();

            std::cout << '\n' << count + 1 << "." << std::endl;
            std::cout << "Patient: " << patientLastName << " "
                      << patientFirstName << " " << patientMiddleName
                      << std::endl;
            std::cout << "Doctor : " << doctorLastName << " " << doctorFirstName
                      << " " << doctorMiddleName << std::endl;
            std::cout << "Prescribed procedure: " << procedureName << std::endl;
            std::cout << "Procedure count: " << procedureCount << std::endl;
        }
        ++count;
    }
}

void DatabaseMenu::printFirstNProcedures(int start, int n)
{
    int count = 0;
    std::string name;
    for (auto it = procedures.begin();
         it != procedures.end() && count < start + n; ++it)
    {
        if (count >= start)
        {
            auto procedure = it->second;
            name = procedure.getName();

            std::cout << '\n' << count + 1 << ". " << name << std::endl;
        }
        ++count;
    }
}

void DatabaseMenu::printFirstNTestResults(int start, int n)
{
    int count = 0, idTest, idVisit;
    std::string testName, result, doctorFirstName, doctorMiddleName,
        doctorLastName, patientFirstName, patientMiddleName, patientLastName;
    for (auto it = testResults.begin();
         it != testResults.end() && count < start + n; ++it)
    {
        if (count >= start)
        {
            auto testResult = it->second;
            idVisit = testResult.getIdVisit();
            idTest = testResult.getIdTest();
            result = testResult.getResult();
            auto visit = *(visits.getFromCacheMap(idVisit));
            auto doctor = *(doctors.getFromCacheMap(visit.getIdDoctor()));
            doctorFirstName = doctor.getFirstName();
            doctorMiddleName = doctor.getMiddleName();
            doctorLastName = doctor.getLastName();
            auto patient = *(patients.getFromCacheMap(visit.getIdPatient()));
            patientFirstName = patient.getFirstName();
            patientMiddleName = patient.getMiddleName();
            patientLastName = patient.getLastName();
            auto test = *(tests.getFromCacheMap(idTest));
            testName = test.getName();

            std::cout << '\n' << count + 1 << "." << std::endl;
            std::cout << "Patient: " << patientLastName << " "
                      << patientFirstName << " " << patientMiddleName
                      << std::endl;
            std::cout << "Doctor: " << doctorLastName << " " << doctorFirstName
                      << " " << doctorMiddleName << std::endl;
            std::cout << "Test: " << testName << std::endl;
            std::cout << "Test result: " << result << std::endl;
        }
        ++count;
    }
}

void DatabaseMenu::printFirstNTests(int start, int n)
{
    int count = 0;
    std::string name;
    for (auto it = tests.begin(); it != tests.end() && count < start + n; ++it)
    {
        if (count >= start)
        {
            auto test = it->second;
            name = test.getName();

            std::cout << '\n' << count + 1 << "." << name << std::endl;
        }
        ++count;
    }
}

void DatabaseMenu::printFirstNDiagnosis(int start, int n)
{
    int count = 0, idVisit, idDoctor, idPatient;
    std::string name, dateVisit, dateDischarge, dateClose, complaints,
        doctorFirstName, doctorMiddleName, doctorLastName, patientFirstName,
        patientMiddleName, patientLastName;
    for (auto it = diagnosis.begin();
         it != diagnosis.end() && count < start + n; ++it)
    {
        if (count >= start)
        {
            auto diag = it->second;
            name = diag.getDescription();
            idVisit = diag.getIdVisit();
            auto visit = *(visits.getFromCacheMap(idVisit));
            idDoctor = visit.getIdDoctor();
            idPatient = visit.getIdPatient();
            dateVisit = visit.getDateVisit().toString();
            dateDischarge = visit.getDateDischarge().toString();
            dateClose = visit.getDateClose().toString();
            complaints = visit.getComplaints();
            auto doctor = *(doctors.getFromCacheMap(idDoctor));
            doctorFirstName = doctor.getFirstName();
            doctorMiddleName = doctor.getMiddleName();
            doctorLastName = doctor.getLastName();
            auto patient = *(patients.getFromCacheMap(idPatient));
            patientFirstName = patient.getFirstName();
            patientMiddleName = patient.getMiddleName();
            patientLastName = patient.getLastName();

            std::cout << '\n' << count + 1 << "." << name << std::endl;
            std::cout << "Complaints: " << complaints << std::endl;
            std::cout << "Patient: " << patientLastName << " "
                      << patientFirstName << " " << patientMiddleName
                      << std::endl;
            std::cout << "Doctor:  " << doctorLastName << " " << doctorFirstName
                      << " " << doctorMiddleName << std::endl;
            std::cout << "Date visit    : " << dateVisit << std::endl;
            std::cout << "Date discharge: " << dateDischarge << std::endl;
            std::cout << "Date close    : " << dateClose << std::endl;
        }
        ++count;
    }
}

void DatabaseMenu::printFirstNVisits(int start, int n)
{
    int count = 0, idDoctor, idPatient;
    std::string complaints, dateVisit, dateDischarge, dateClose,
        doctorFirstName, doctorMiddleName, doctorLastName, patientFirstName,
        patientMiddleName, patientLastName;
    for (auto it = visits.begin(); it != visits.end() && count < start + n;
         ++it)
    {
        if (count >= start)
        {
            auto visit = it->second;
            idDoctor = visit.getIdDoctor();
            idPatient = visit.getIdPatient();
            complaints = visit.getComplaints();
            dateVisit = visit.getDateVisit().toString();
            dateDischarge = visit.getDateDischarge().toString();
            dateClose = visit.getDateClose().toString();
            doctorFirstName =
                (*(doctors.getFromCacheMap(idDoctor))).getFirstName();
            doctorMiddleName =
                (*(doctors.getFromCacheMap(idDoctor))).getMiddleName();
            doctorLastName =
                (*(doctors.getFromCacheMap(idDoctor))).getLastName();
            patientFirstName =
                (*(patients.getFromCacheMap(idPatient))).getFirstName();
            patientMiddleName =
                (*(patients.getFromCacheMap(idPatient))).getMiddleName();
            patientLastName =
                (*(patients.getFromCacheMap(idPatient))).getLastName();

            std::cout << '\n' << count + 1 << std::endl;
            std::cout << "Patient: " << patientLastName << " "
                      << patientFirstName << " " << patientMiddleName
                      << std::endl;
            std::cout << "Doctor:  " << doctorLastName << " " << doctorFirstName
                      << " " << doctorMiddleName << std::endl;
            std::cout << "Date visit:     " << dateVisit << std::endl;
            std::cout << "Date discharge: " << dateDischarge << std::endl;
            std::cout << "Date close:     " << dateClose << std::endl;
            std::cout << "Complaints: " << complaints << std::endl;
        }
        ++count;
    }
}

void DatabaseMenu::printFirstNPatients(int start, int n)
{
    int count = 0;
    std::string name, middleName, lastName, address;
    for (auto it = patients.begin(); it != patients.end() && count < start + n;
         ++it)
    {
        if (count >= start)
        {
            auto patient = it->second;
            name = patient.getFirstName();
            middleName = patient.getMiddleName();
            lastName = patient.getLastName();
            address = patient.getAddress();

            std::cout << '\n'
                      << count + 1 << "." << lastName << " " << name << " "
                      << middleName << std::endl;
            std::cout << "Address: " << address << std::endl;
        }
        ++count;
    }
}

void DatabaseMenu::printFirstNAppointments(int start, int n)
{
    int count = 0, office = -1, district = -1, idDoctor, idWeekday;
    std::string doctorFirstName, doctorMiddleName, doctorLastName, weekday,
        beginDate, endDate;
    for (auto it = appointments.begin();
         it != appointments.end() && count < start + n; ++it)
    {
        if (count >= start)
        {
            auto appointment = it->second;
            beginDate = appointment.getBeginDate().toString();
            endDate = appointment.getEndDate().toString();
            office = appointment.getOffice();
            district = appointment.getDistrict();
            idDoctor = appointment.getIdDoctor();
            idWeekday = appointment.getIdWeekday();
            weekday =
                (*(databaseFacade->findWeekdayById(idWeekday))).toString();
            doctorFirstName =
                (*(doctors.getFromCacheMap(idDoctor))).getFirstName();
            doctorMiddleName =
                (*(doctors.getFromCacheMap(idDoctor))).getMiddleName();
            doctorLastName =
                (*(doctors.getFromCacheMap(idDoctor))).getLastName();

            std::cout << '\n'
                      << count + 1 << '\n'
                      << beginDate << " " << endDate << " " << weekday
                      << std::endl;
            std::cout << "Doctor: " << doctorLastName << " " << doctorFirstName
                      << " " << doctorMiddleName << std::endl;
            std::cout << "Office: " << office << std::endl;
            std::cout << "Disctrict: " << district << std::endl;
        }
        ++count;
    }
}

void DatabaseMenu::printFirstNSpecializations(int start, int n)
{
    int count = 0;
    std::string name;
    for (auto it = specializations.begin();
         it != specializations.end() && count < start + n; ++it)
    {
        if (count >= start)
        {
            auto specializations = it->second;
            name = specializations.getName();

            std::cout << '\n' << count + 1 << ". " << name << std::endl;
        }
        ++count;
    }
}

void DatabaseMenu::printDoctors(int page)
{
    std::string header = "Doctors. Page " + std::to_string(page + 1);
    printWithBorder(header);
    printFirstNDoctors(page * entitiesOnPage, entitiesOnPage);
    printFooter();
}

void DatabaseMenu::printPrescribedMedications(int page)
{
    std::string header =
        "Prescribed medications. Page " + std::to_string(page + 1);
    printWithBorder(header);
    printFirstNPrescribedMedications(page * entitiesOnPage, entitiesOnPage);
    printFooter();
}

void DatabaseMenu::printMedications(int page)
{
    std::string header = "Medications. Page " + std::to_string(page + 1);
    printWithBorder(header);
    printFirstNMedications(page * entitiesOnPage, entitiesOnPage);
    printFooter();
}

void DatabaseMenu::printPrescribedProcedures(int page)
{
    std::string header =
        "Prescribed procedures. Page " + std::to_string(page + 1);
    printWithBorder(header);
    printFirstNPrescribedProcedures(page * entitiesOnPage, entitiesOnPage);
    printFooter();
}

void DatabaseMenu::printProcedures(int page)
{
    std::string header = "Procedures. Page " + std::to_string(page + 1);
    printWithBorder(header);
    printFirstNProcedures(page * entitiesOnPage, entitiesOnPage);
    printFooter();
}

void DatabaseMenu::printTestResults(int page)
{
    std::string header = "Test results. Page " + std::to_string(page + 1);
    printWithBorder(header);
    printFirstNTestResults(page * entitiesOnPage, entitiesOnPage);
    printFooter();
}

void DatabaseMenu::printTests(int page)
{
    std::string header = "Tests. Page " + std::to_string(page + 1);
    printWithBorder(header);
    printFirstNTests(page * entitiesOnPage, entitiesOnPage);
    printFooter();
}

void DatabaseMenu::printDiagnosis(int page)
{
    std::string header = "Diagnosis. Page " + std::to_string(page + 1);
    printWithBorder(header);
    printFirstNDiagnosis(page * entitiesOnPage, entitiesOnPage);
    printFooter();
}

void DatabaseMenu::printVisits(int page)
{
    std::string header = "Visits. Page " + std::to_string(page + 1);
    printWithBorder(header);
    printFirstNVisits(page * entitiesOnPage, entitiesOnPage);
    printFooter();
}

void DatabaseMenu::printPatients(int page)
{
    std::string header = "Patients. Page " + std::to_string(page + 1);
    printWithBorder(header);
    printFirstNPatients(page * entitiesOnPage, entitiesOnPage);
    printFooter();
}

void DatabaseMenu::printAppointments(int page)
{
    std::string header = "Appointments. Page " + std::to_string(page + 1);
    printWithBorder(header);
    printFirstNAppointments(page * entitiesOnPage, entitiesOnPage);
    printFooter();
}

void DatabaseMenu::printSpecializations(int page)
{
    std::string header = "Specializations. Page " + std::to_string(page + 1);
    printWithBorder(header);
    printFirstNSpecializations(page * entitiesOnPage, entitiesOnPage);
    printFooter();
}

int DatabaseMenu::specializationActivities()
{
    int page = 0;
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
                while (1)
                {
                    std::cout << "\nPage " << page + 1 << std::endl;
                    std::cout << "1. Next page\n";
                    std::cout << "2. Previous page\n";
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
                            if (page > 0)
                                page--;
                            break;
                        case 3:
                            if (isSpecializationCacheDirty)
                            {
                                auto specList =
                                    databaseFacade->selectSpecializationAll(100,
                                                                            0);
                                specializations.addMultiple(specList);
                                isSpecializationCacheDirty = false;
                            }
                            printSpecializations(page);
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

int DatabaseMenu::appointmentActivities()
{
    int page = 0;
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
                while (1)
                {
                    std::cout << "\nPage " << page + 1 << std::endl;
                    std::cout << "1. Next page\n";
                    std::cout << "2. Previous page\n";
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
                            if (page > 0)
                                page--;
                            break;
                        case 3:
                            if (isAppointmentCacheDirty)
                            {
                                auto appList =
                                    databaseFacade->selectAppointmentAll(100,
                                                                         0);
                                appointments.addMultiple(appList);
                                isAppointmentCacheDirty = false;
                            }
                            printAppointments(page);
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

int DatabaseMenu::patientActivities()
{
    int page = 0;
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
                while (1)
                {
                    std::cout << "\nPage " << page + 1 << std::endl;
                    std::cout << "1. Next page\n";
                    std::cout << "2. Previous page\n";
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
                            if (page > 0)
                                page--;
                            break;
                        case 3:
                            if (isPatientCacheDirty)
                            {
                                auto patList =
                                    databaseFacade->selectPatientAll(100, 0);
                                patients.addMultiple(patList);
                                isPatientCacheDirty = false;
                            }
                            printPatients(page);
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

int DatabaseMenu::visitActivities()
{
    int page = 0;
    while (true)
    {
        std::cout << "\n*** Visits activities ***\n";
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
                while (1)
                {
                    std::cout << "\nPage " << page + 1 << std::endl;
                    std::cout << "1. Next page\n";
                    std::cout << "2. Previous page\n";
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
                            if (page > 0)
                                page--;
                            break;
                        case 3:
                            if (isVisitCacheDirty)
                            {
                                auto visitsList =
                                    databaseFacade->selectVisitAll(100, 0);
                                visits.addMultiple(visitsList);
                                isVisitCacheDirty = false;
                            }
                            printVisits(page);
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

int DatabaseMenu::prescribedMedicationActivities()
{
    int page = 0;
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
                while (1)
                {
                    std::cout << "\nPage " << page + 1 << std::endl;
                    std::cout << "1. Next page\n";
                    std::cout << "2. Previous page\n";
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
                            if (page > 0)
                                page--;
                            break;
                        case 3:
                            if (isPrMedCacheDirty)
                            {
                                auto prMedsList =
                                    databaseFacade->selectPrMedAll(100, 0);
                                prmeds.addMultiple(prMedsList);
                                isPrMedCacheDirty = false;
                            }
                            printPrescribedMedications(page);
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

int DatabaseMenu::medicationActivities()
{
    int page = 0;
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
                while (1)
                {
                    std::cout << "\nPage " << page + 1 << std::endl;
                    std::cout << "1. Next page\n";
                    std::cout << "2. Previous page\n";
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
                            if (page > 0)
                                page--;
                            break;
                        case 3:

                            if (isMedicationCacheDirty)
                            {
                                auto medicationsList =
                                    databaseFacade->selectMedicationAll(100, 0);
                                medications.addMultiple(medicationsList);
                                isMedicationCacheDirty = false;
                            }
                            printMedications(page);
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

int DatabaseMenu::prescribedProcedureActivities()
{
    int page = 0;
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
                while (1)
                {
                    std::cout << "\nPage " << page + 1 << std::endl;
                    std::cout << "1. Next page\n";
                    std::cout << "2. Previous page\n";
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
                            if (page > 0)
                                page--;
                            break;
                        case 3:
                            if (isPrProcCacheDirty)
                            {
                                auto prprocsList =
                                    databaseFacade->selectPrProcAll(100, 0);
                                prprocs.addMultiple(prprocsList);
                                isPrProcCacheDirty = false;
                            }
                            printPrescribedProcedures(page);
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

int DatabaseMenu::procedureActivities()
{
    int page = 0;
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
                while (1)
                {
                    std::cout << "\nPage " << page + 1 << std::endl;
                    std::cout << "1. Next page\n";
                    std::cout << "2. Previous page\n";
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
                            if (page > 0)
                                page--;
                            break;
                        case 3:
                            if (isProcedureCacheDirty)
                            {
                                auto proceduresList =
                                    databaseFacade->selectProcedureAll(100, 0);
                                procedures.addMultiple(proceduresList);
                                isProcedureCacheDirty = false;
                            }
                            printProcedures(page);
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

int DatabaseMenu::testResultActivities()
{
    int page = 0;
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
                while (1)
                {
                    std::cout << "\nPage " << page + 1 << std::endl;
                    std::cout << "1. Next page\n";
                    std::cout << "2. Previous page\n";
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
                            if (page > 0)
                                page--;
                            break;
                        case 3:
                            if (isTestResultCacheDirty)
                            {
                                auto testResultsList =
                                    databaseFacade->selectTestResultAll(100, 0);
                                testResults.addMultiple(testResultsList);
                                isTestResultCacheDirty = false;
                            }
                            printTestResults(page);
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

int DatabaseMenu::testActivities()
{
    int page = 0;
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
                while (1)
                {
                    std::cout << "\nPage " << page + 1 << std::endl;
                    std::cout << "1. Next page\n";
                    std::cout << "2. Previous page\n";
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
                            if (page > 0)
                                page--;
                            break;
                        case 3:
                            if (isTestCacheDirty)
                            {
                                auto testsList =
                                    databaseFacade->selectTestAll(100, 0);
                                tests.addMultiple(testsList);
                                isTestCacheDirty = false;
                            }
                            printTests(page);
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

int DatabaseMenu::diagnosisActivities()
{
    int page = 0;
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
                while (1)
                {
                    std::cout << "\nPage " << page + 1 << std::endl;
                    std::cout << "1. Next page\n";
                    std::cout << "2. Previous page\n";
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
                            if (page > 0)
                                page--;
                            break;
                        case 3:
                            if (isDiagnosisCacheDirty)
                            {
                                auto diagnosisList =
                                    databaseFacade->selectDiagnosisAll(100, 0);
                                diagnosis.addMultiple(diagnosisList);
                                isDiagnosisCacheDirty = false;
                            }
                            printDiagnosis(page);
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