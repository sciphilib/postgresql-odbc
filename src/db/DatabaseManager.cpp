#include "DatabaseManager.h"
#include <memory>
#include <sqltypes.h>

DatabaseManager::DatabaseManager(const std::string& dsn,
                                 const std::string& user,
                                 const std::string& pass)
    : dsn_(dsn), user_(user), pass_(pass)
{
    connect();
    initGateways();
    initFacade();
    initDatabase();
}

DatabaseManager::~DatabaseManager()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

void DatabaseManager::connect()
{
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) == SQL_ERROR)
    {
        std::cerr << "Error allocating an environment handle\n";
        exit(-1);
    }

    if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0) ==
        SQL_ERROR)
    {
        std::cerr << "Error setting the environment attribute setting\n";
        exit(-1);
    }

    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) == SQL_ERROR)
    {
        std::cerr << "Error allocating a database connection handle\n";
        exit(-1);
    }

    int retcode = SQLConnect(hDbc, (SQLCHAR*)"PostgreSQLDSN", SQL_NTS,
                             (SQLCHAR*)"sciphilib", SQL_NTS,
                             (SQLCHAR*)"sciphilib", SQL_NTS);
    if (retcode == SQL_ERROR)
    {
        std::cerr << "Error connecting to the data source\n";
        exit(-1);
    }

    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_ERROR)
    {
        std::cerr << "Error allocating a statement handle\n";
        exit(-1);
    }

    SQLCHAR setEncoding[] = "SET client_encoding TO 'UTF8'";
    SQLExecDirect(hStmt, setEncoding, SQL_NTS);

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::initGateways()
{
    appointmentTDG = std::make_unique<AppointmentTDG>(hDbc);
    doctorTDG = std::make_unique<DoctorTDG>(hDbc);
    medicationTDG = std::make_unique<MedicationTDG>(hDbc);
    patientTDG = std::make_unique<PatientTDG>(hDbc);
    prescribedMedicationTDG = std::make_unique<PrescribedMedicationTDG>(hDbc);
    prescribedProcedureTDG = std::make_unique<PrescribedProcedureTDG>(hDbc);
    procedureTDG = std::make_unique<ProcedureTDG>(hDbc);
    specializationTDG = std::make_unique<SpecializationTDG>(hDbc);
    visitTDG = std::make_unique<VisitTDG>(hDbc);
    weekdayTDG = std::make_unique<WeekdayTDG>(hDbc);
    testTDG = std::make_unique<TestTDG>(hDbc);
    testResultTDG = std::make_unique<TestResultTDG>(hDbc);
    diagnosisTDG = std::make_unique<DiagnosisTDG>(hDbc);
}

void DatabaseManager::initDatabase()
{
    createTableAll();
    insertAll();
    findSomeObjects();
    updateAll();
    deleteAll();
    /* Parser parser;
    parser.parseAppointment("data/appointments.json");
    parser.printAppointments(); */
    return;
}

void DatabaseManager::initFacade()
{
    databaseFacade = std::make_unique<DatabaseFacade>(
        appointmentTDG.get(), doctorTDG.get(), medicationTDG.get(),
        patientTDG.get(), prescribedMedicationTDG.get(),
        prescribedProcedureTDG.get(), procedureTDG.get(),
        specializationTDG.get(), visitTDG.get(), weekdayTDG.get(),
        testTDG.get(), testResultTDG.get(), diagnosisTDG.get());
}

void DatabaseManager::findSomeObjects()
{
    std::cout << "~~~~ Table search ~~~~" << std::endl;
    std::unique_ptr<Appointment> appointment;
    appointment = databaseFacade->findAppointmentById(1);
    appointment->print();
    appointment = databaseFacade->findAppointmentById(5);
    appointment->print();
    std::unique_ptr<Weekday> weekday;
    weekday = databaseFacade->findWeekdayById(1);
    weekday->print();
    weekday = databaseFacade->findWeekdayById(5);
    weekday->print();
    std::unique_ptr<Specialization> spec;
    spec = databaseFacade->findSpecializationById(1);
    spec->print();
    spec = databaseFacade->findSpecializationById(3);
    spec->print();
    std::unique_ptr<Doctor> doctor;
    doctor = databaseFacade->findDoctorById(1);
    doctor->print();
    doctor = databaseFacade->findDoctorById(4);
    doctor->print();
    int id = 100;
    doctor = databaseFacade->findDoctorById(id);
    if (doctor)
        doctor->print();
    else
        std::cout << "Doctor: there is no doctor with id = " << id << std::endl;
    std::unique_ptr<Patient> patient;
    patient = databaseFacade->findPatientById(1);
    patient->print();
    patient = databaseFacade->findPatientById(2);
    patient->print();
    std::unique_ptr<Visit> visit;
    visit = databaseFacade->findVisitById(1);
    visit->print();
    visit = databaseFacade->findVisitById(5);
    visit->print();
    std::unique_ptr<PrescribedMedication> prmed;
    prmed = databaseFacade->findPrescribedMedicationById(1);
    prmed->print();
    prmed = databaseFacade->findPrescribedMedicationById(3);
    prmed->print();
    std::unique_ptr<Medication> med;
    med = databaseFacade->findMedicationById(1);
    med->print();
    med = databaseFacade->findMedicationById(3);
    med->print();
    std::unique_ptr<PrescribedProcedure> prproc;
    prproc = databaseFacade->findPrescribedProcedureById(1);
    prproc->print();
    prproc = databaseFacade->findPrescribedProcedureById(3);
    prproc->print();
    std::unique_ptr<Procedure> procedure;
    procedure = databaseFacade->findProcedureById(1);
    procedure->print();
    procedure = databaseFacade->findProcedureById(3);
    procedure->print();
    std::unique_ptr<TestResult> testResult;
    testResult = databaseFacade->findTestResultById(1);
    testResult->print();
    testResult = databaseFacade->findTestResultById(3);
    testResult->print();
    std::unique_ptr<Test> test;
    test = databaseFacade->findTestById(1);
    test->print();
    test = databaseFacade->findTestById(3);
    test->print();
    std::unique_ptr<Diagnosis> diagnosis;
    diagnosis = databaseFacade->findDiagnosisById(1);
    diagnosis->print();
    diagnosis = databaseFacade->findDiagnosisById(3);
    diagnosis->print();
}

void DatabaseManager::updateAll()
{
    std::cout << "~~~~ Table updating ~~~~" << std::endl;
    updateAppointment();
    updateDiagnosis();
    updateDoctor();
    updateMedication();
    updatePatient();
}

void DatabaseManager::updateAppointment()
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

void DatabaseManager::updateDiagnosis()
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

void DatabaseManager::updateDoctor()
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

void DatabaseManager::updateMedication()
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

void DatabaseManager::updatePatient()
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

void DatabaseManager::deleteAll() { deleteDoctor(); }

void DatabaseManager::deleteDoctor()
{
    int id = 1;
    if (databaseFacade->deleteDoctor(id))
    {
        std::cout << "Doctor with id  = " << id << " was deleted\n";
    }
    else
    {
        std::cout << "Doctor with id  = " << id << " was NOT deleted\n";
    }
}

void DatabaseManager::insertAll()
{
    std::cout << "~~~~ Table inserting ~~~~" << std::endl;
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

void DatabaseManager::insertDiagnosis()
{
    databaseFacade->addDiagnosis(1, "Unspecified essential hypertension");
    databaseFacade->addDiagnosis(2, "Lumbago");
    databaseFacade->addDiagnosis(
        3, "Routine general medical examination at a healthcare facility");
    databaseFacade->addDiagnosis(4, "Atrial fibrillation");
    databaseFacade->addDiagnosis(5, "Coronary artery atherosclerosis");
}

void DatabaseManager::insertTestResults()
{
    databaseFacade->addTestResult(1, 1, "Everything's fine, no abnormalities.");
    databaseFacade->addTestResult(2, 2, "Some above-average readings, retest");
    databaseFacade->addTestResult(3, 3, "Some below average, retreatment");
    databaseFacade->addTestResult(4, 4, "Pathology detected");
    databaseFacade->addTestResult(5, 5, "Tests were taken incorrectly");
}

void DatabaseManager::insertTests()
{
    databaseFacade->addTest("Blood test");
    databaseFacade->addTest("HBA1C test");
    databaseFacade->addTest("Liver function test");
    databaseFacade->addTest("Thyroid profile test");
    databaseFacade->addTest("HCG test");
}

void DatabaseManager::insertPrescribedProcedures()
{
    databaseFacade->addPrescribedProcedure(1, 1, 10);
    databaseFacade->addPrescribedProcedure(1, 2, 2);
    databaseFacade->addPrescribedProcedure(1, 3, 1);
    databaseFacade->addPrescribedProcedure(2, 4, 8);
    databaseFacade->addPrescribedProcedure(3, 5, 3);
    databaseFacade->addPrescribedProcedure(4, 1, 1);
    databaseFacade->addPrescribedProcedure(5, 2, 1);
}

void DatabaseManager::insertProcedures()
{
    databaseFacade->addProcedure("Physiotherapy");
    databaseFacade->addProcedure("Rest");
    databaseFacade->addProcedure("Magnetic resonance imaging");
    databaseFacade->addProcedure("X-ray");
    databaseFacade->addProcedure("Breathing exercises");
}

void DatabaseManager::insertPrescribedMedications()
{
    databaseFacade->addPrescribedMedication(1, 1);
    databaseFacade->addPrescribedMedication(1, 2);
    databaseFacade->addPrescribedMedication(1, 3);
    databaseFacade->addPrescribedMedication(2, 2);
    databaseFacade->addPrescribedMedication(3, 3);
    databaseFacade->addPrescribedMedication(4, 4);
    databaseFacade->addPrescribedMedication(5, 5);
}

void DatabaseManager::insertVisits()
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

void DatabaseManager::insertMedications()
{
    databaseFacade->addMedication("Atorvastatin");
    databaseFacade->addMedication("Levothyroxine");
    databaseFacade->addMedication("Metformin");
    databaseFacade->addMedication("Lisinopril");
    databaseFacade->addMedication("Amlodipine");
    databaseFacade->addMedication("Metoprolol");
    databaseFacade->addMedication("Albuterol");
}

void DatabaseManager::insertPatients()
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
    databaseFacade->addPatient("Kuptsov", "Nikolay", "Nikolaevich",
                               "Novosibirsk, Esenina st., building 1, flat 1");
}

void DatabaseManager::insertAppointments()
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

void DatabaseManager::insertDoctors()
{
    databaseFacade->addDoctor("Ivanov", "Ivan", "Ivanovich", 1);
    databaseFacade->addDoctor("Perelman", "Egor", "Victorovich", 2);
    databaseFacade->addDoctor("Eyfeld", "Evgeny", "Alexsandrovich", 3);
    databaseFacade->addDoctor("Sinitsyn", "Vladimir", "Vladimirovich", 4);
    databaseFacade->addDoctor("Kotov", "Dmitry", "Platonovich", 5);
    databaseFacade->addDoctor("Kuznetsov", "Artyom", "Igorevich", 6);
}

void DatabaseManager::insertWeekdays()
{
    databaseFacade->addWeekday("Monday");
    databaseFacade->addWeekday("Tuesday");
    databaseFacade->addWeekday("Wednesday");
    databaseFacade->addWeekday("Thursday");
    databaseFacade->addWeekday("Friday");
    databaseFacade->addWeekday("Saturday");
    databaseFacade->addWeekday("Sunday");
}

void DatabaseManager::insertSpecializations()
{
    databaseFacade->addSpecialization("Surgeon");
    databaseFacade->addSpecialization("Pediatrician");
    databaseFacade->addSpecialization("Neurosurgeon");
    databaseFacade->addSpecialization("Internist");
    databaseFacade->addSpecialization("Ophthalmologist");
    databaseFacade->addSpecialization("Trauma surgeon");
}

void DatabaseManager::createTableAll()
{
    std::cout << "~~~~ Table creating ~~~~" << std::endl;
    createTableWeekdays();
    createTableSpecializations();
    createTableDoctors();
    createTableAppointments();
    createTablePatients();
    createTableVisits();
    createTableMedications();
    createTablePrescribedMedications();
    createTableProcedures();
    createTablePrescribedProcedures();
    createTableTests();
    createTableTestResults();
    createTableDiagnosis();
}

void DatabaseManager::createTableWeekdays()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'weekdays'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'weekdays' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] = "CREATE TABLE weekdays (id SERIAL "
                                         "PRIMARY KEY, name VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'weekdays' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table 'weekdays'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableSpecializations()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] =
        "SELECT 1 FROM information_schema.tables WHERE "
        "table_schema = 'public' AND table_name = 'specializations'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'specializations' is already exists."
                      << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE specializations (id SERIAL "
                "PRIMARY KEY, name VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'specializations' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table "
                             "'specializations'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableDoctors()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'doctors'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'doctors' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE doctors (id SERIAL "
                "PRIMARY KEY, last_name VARCHAR(255), first_name "
                "VARCHAR(255), "
                "middle_name VARCHAR(255), id_spec INT REFERENCES Specializations(id) ON DELETE CASCADE)";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'doctors' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table 'doctors'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableAppointments()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'appointments'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'appointments' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE appointments (id SERIAL "
                "PRIMARY KEY, id_doctor INT REFERENCES Doctors(id) ON DELETE "
                "CASCADE, "
                "id_weekday "
                "INT REFERENCES weekdays(id) ON DELETE CASCADE, begin_date "
                "TIME, end_date "
                "TIME, "
                "office INT, district INT)";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'appointments' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr
                    << "Error while trying to create table 'appointments'."
                    << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTablePatients()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'patients'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'patients' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE patients (id SERIAL "
                "PRIMARY KEY, last_name VARCHAR(255), first_name "
                "VARCHAR(255), "
                "middle_name VARCHAR(255), address VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'patients' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table 'patients'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableVisits()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'visits'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'visits' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE visits (id SERIAL "
                "PRIMARY KEY, id_patient INT REFERENCES patients(id) ON DELETE "
                "CASCADE, "
                "id_doctor INT REFERENCES doctors(id) ON DELETE CASCADE, "
                "complaints VARCHAR(255),"
                "date_visit TIMESTAMP WITHOUT TIME ZONE, date_discharge "
                "TIMESTAMP WITHOUT TIME ZONE, date_close TIMESTAMP WITHOUT "
                "TIME ZONE)";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'visits' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table 'visits'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableMedications()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'medications'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'medications' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] = "CREATE TABLE medications (id SERIAL "
                                         "PRIMARY KEY, name VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'medications' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table 'medications'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTablePrescribedMedications()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] =
        "SELECT 1 FROM information_schema.tables WHERE "
        "table_schema = 'public' AND table_name = 'prescribed_medications'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'prescribed_medications' is already exists."
                      << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE prescribed_medications (id SERIAL "
                "PRIMARY KEY, id_visit INT REFERENCES visits(id) ON DELETE "
                "CASCADE, "
                "id_medication INT REFERENCES medications(id) ON DELETE CASCADE)";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'prescribed_medications' was "
                             "succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table "
                             "'prescribed_medications'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableProcedures()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'procedures'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'procedures' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] = "CREATE TABLE procedures (id SERIAL "
                                         "PRIMARY KEY, name VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'procedures' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table 'procedures'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTablePrescribedProcedures()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] =
        "SELECT 1 FROM information_schema.tables WHERE "
        "table_schema = 'public' AND table_name = 'prescribed_procedures'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'prescribed_procedures' is already exists."
                      << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE prescribed_procedures (id SERIAL "
                "PRIMARY KEY, id_visit INT REFERENCES visits(id) ON DELETE "
                "CASCADE, "
                "id_procedure "
                "INT REFERENCES procedures(id) ON DELETE CASCADE,"
                "count INT)";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'prescribed_medications' was "
                             "succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table "
                             "'prescribed_medications'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableTests()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'tests'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'tests' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] = "CREATE TABLE tests (id SERIAL "
                                         "PRIMARY KEY, name VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'tests' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table 'tests'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableTestResults()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'test_results'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'test_results' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE test_results (id SERIAL "
                "PRIMARY KEY, id_visit INT REFERENCES visits(id) ON DELETE "
                "CASCADE, id_test "
                "INT REFERENCES tests(id) ON DELETE CASCADE, result "
                "VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'test_results' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table "
                             "'test_results'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableDiagnosis()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'diagnosis'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'diagnosis' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE diagnosis (id SERIAL "
                "PRIMARY KEY, id_visit INT REFERENCES visits(id) ON DELETE "
                "CASCADE, description "
                "VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'diagnosis' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table "
                             "'diagnosis'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}