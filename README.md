# CO2203 University Course Registration, Timetable & Attendance System

Integrated C++17 console application for the CO2203 OOP group project.

## Build

### g++
```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp main.cpp -o lms
./lms
```
On Windows PowerShell with MSYS2/MinGW, run `./lms.exe` (or `.\\lms.exe`).

### CMake
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Demo logins
- Lecturer: `smith@uni.lk` / `pass123`
- Student: `alice@uni.lk` / `pass456`
- Administrator: `admin@uni.lk` / `adminpw`

On a clean first run the program seeds CO2203, assigns Dr. Smith, and enrols Alice. Data is then persisted in the repository data files.

## Main features
- Three role-specific menus: Student, Lecturer, Administrator.
- Admin user/course create, edit, remove, list, lecturer assignment, prerequisites, schedules, and enrolment-summary report.
- Lecture/Lab/Project course hierarchy and Person hierarchy.
- Student enrol/drop with capacity, prerequisite and timetable-clash validation.
- Personal timetable with operator-based clash detection.
- Lecturer can view only assigned courses and their enrolment lists.
- Attendance sessions, expiry/close, duplicate/not-enrolled/session validation, append-only corrections, reports and persistence.
- Runtime-selectable AttendanceCapture implementations: CardTapCapture and FileReplayCapture.
- FileReplayCapture accepts plain UID events such as `ST001`; malformed lines (spaces/punctuation/empty events) raise a capture error and are skipped by the lecturer workflow.
- Generic `Repository<T>` and storage abstraction for persistence.
- Overloaded stream insertion operators for Person, Course and Timetable; TimeSlot comparison operators support scheduling logic.

## File replay demo
Create a file such as:
```text
ST001
BAD EVENT!
UNKNOWN999
```
Select **File Replay** in the Lecturer menu, provide the path, open an attendance session and mark attendance. `ST001` is a valid UID, `BAD EVENT!` demonstrates malformed-event exception handling, and an unknown but well-formed UID is reported as unknown.

## Design changes
Implementation changes from the original UML (including Course schedule integration and persistence/integration additions) are recorded in `INTEGRATION_CHANGES.md` / `FIXES_APPLIED.md`. The final UML should be updated to truthfully match the submitted code.

## Submission note
Do not submit generated executables or temporary `.dat` test files unless they are intentionally included as sample data. Include the final UML, design change log, project report, contribution table and testing summary required by the project specification.
