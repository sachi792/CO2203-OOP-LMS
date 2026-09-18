# Integration Changes for Member 2

This file records changes made outside the normal Member 2-owned files so the Attendance and Timetable modules can work with the full LMS project.

**Purpose:** If another member later updates their own file, do not replace their whole file with this version. Re-apply only the required integration points below, then resolve any Git conflicts normally.

## Member ownership used in this project

From `read this.txt`:
- **Member 1:** Administrator, Courses, Exceptions, Person-side domain work, Enrollment
- **Member 2:** Attendance classes, CardTapCapture, ConsoleCardReader, TimeSlot, Timetable
- **Member 3:** Repositories, MenuUI
- **Member 4:** File storage, FileReplayCapture

---

## 1. `include/Student.h` — Member 1 integration

### Why Member 2 needs it
Attendance must check whether a student is enrolled in a course, and Enrollment must access the student's real timetable. Student also needs to display Member 2 timetable/attendance data.

### Integration points currently present

```cpp
#include "Timetable.h"
using attendance::Timetable;
```

The Student owns the real Member 2 timetable:

```cpp
Timetable timetable_;
```

Helpers used by integration:

```cpp
bool isEnrolledIn(const std::string& courseCode) const;
const std::string& getStudentId() const noexcept;
const std::vector<std::shared_ptr<Enrollment>>& getEnrollments() const noexcept;
Timetable* getTimetable() noexcept;
const Timetable* getTimetable() const noexcept;
```

### Important when Member 1 changes Student.h
Keep the real `attendance::Timetable` connection and keep an equivalent public way for attendance/enrollment code to obtain the student ID, enrollment state, and timetable. Do not create a second timetable object just for Member 2.

---

## 2. `src/Student.cpp` — Member 1 integration

### Timetable display
Current integrated implementation:

```cpp
void Student::viewTimetable() const {
    timetable_.display();
}
```

This replaces placeholder behavior and displays the student's actual Member 2 timetable.

### Attendance display
Current integrated implementation reads the shared `AttendanceRepository`:

```cpp
void Student::viewAttendance() const {
    const auto& records = repo::SystemManager::getInstance()
                              .getAttendanceRepository().getItems();

    int count = 0;
    std::cout << "\n--- Attendance for " << studentId_ << " ---\n";
    for (const auto& record : records) {
        if (record.studentId != studentId_) continue;
        ++count;
        std::cout << record.courseCode
                  << " | Session " << record.sessionId
                  << " | " << record.status
                  << " | " << record.captureMethod << "\n";
    }

    if (count == 0) {
        std::cout << "No attendance records found.\n";
    } else {
        std::cout << "Total attendance records: " << count << "\n";
    }
}
```

Enrollment validation helper:

```cpp
bool Student::isEnrolledIn(const std::string& courseCode) const {
    return std::any_of(enrollments_.begin(), enrollments_.end(),
        [&](const std::shared_ptr<Enrollment>& e) {
            return e->isActive() && e->getCourse()->getCode() == courseCode;
        });
}
```

### Dependency added

```cpp
#include "SystemManager.h"
```

### Important when Member 1 changes Student.cpp
Preserve the real timetable display, enrollment query, and attendance-repository display. The exact formatting can change; the data connection is what Member 2 needs.

---

## 3. `include/Course.h` / `src/Course.cpp` — Member 1 integration, deliberate off-UML addition

This was already deliberately added during team integration and is marked in the source as **not in the original UML**. It is preserved because timetable clash checking needs a course meeting time.

Current interface:

```cpp
void setSchedule(const TimeSlot& slot) noexcept;
const TimeSlot& getSchedule() const noexcept;
```

Current field:

```cpp
TimeSlot schedule_;
```

### Important when Member 1 changes Course files
Do not accidentally remove `schedule_`, `setSchedule()` or `getSchedule()` unless the whole team replaces them with another agreed mechanism. `Enrollment` currently depends on them for timetable clash checking.

---

## 4. `src/Enrollment.cpp` — Member 1 integration

Enrollment now connects registration/drop operations to Member 2's timetable.

On successful enrollment:

```cpp
if (attendance::Timetable* timetable = student_->getTimetable()) {
    timetable->addSlot(course_->getSchedule());
}
```

On drop:

```cpp
if (attendance::Timetable* timetable = student_->getTimetable()) {
    timetable->removeSlot(course_->getSchedule());
}
```

Clash checking:

```cpp
attendance::Timetable* timetable = student_->getTimetable();
if (timetable == nullptr) return;

auto clash = timetable->clashesWith(course_->getSchedule());
if (clash) {
    throw TimetableClashException(*clash, course_->getSchedule());
}
```

### Important when Member 1 changes Enrollment.cpp
Keep these three behaviors: check the new course slot against the student's timetable, add the slot after successful enrollment, and remove it when the course is dropped.

---

## 5. `src/Lecturer.cpp` — Member 1 integration with Member 2 Attendance

This is the main attendance integration file.

### Member 2 dependencies used

```cpp
#include "AttendanceSession.h"
#include "AttendanceRegister.h"
#include "AttendanceCapture.h"
#include "CardTapCapture.h"
#include "StudentCard.h"
#include "FileReplayCapture.h"
#include "SystemManager.h"
```

### Runtime state used by Lecturer attendance operations
Kept inside `Lecturer.cpp` so extra state was not added to the supplied Lecturer UML class declaration:

```cpp
namespace {
std::unique_ptr<attendance::AttendanceSession> activeSession;
attendance::AttendanceRegister attendanceRegister;
std::unique_ptr<attendance::AttendanceCapture> captureMethod;
int nextSessionId = 1;
}
```

### `openAttendance()`
The integrated function:
- shows courses assigned to the lecturer;
- asks for a course code;
- closes an old open session if necessary;
- creates `AttendanceSession`;
- opens the new session.

Core connection:

```cpp
activeSession = std::make_unique<attendance::AttendanceSession>(
    nextSessionId++, *selected, 5);
activeSession->open();
```

### `selectCaptureMethod()`
Runtime selection is through the common `AttendanceCapture` base type:

```cpp
captureMethod = std::make_unique<attendance::CardTapCapture>();
```

or Member 4's File Replay implementation:

```cpp
captureMethod = std::make_unique<attendance::FileReplayCapture>(path);
```

### `markAttendance()`
The integrated function uses the currently open session and its enrolled students. With Card Tap, the console UID is matched to a `StudentCard`/Student. Successful attendance is marked through `AttendanceRegister`.

Core Member 2 call:

```cpp
attendanceRegister.markAttendance(*selected, *activeSession, "CardTap");
```

After a successful mark, a persistable copy is bridged to Member 3's repository so Student can view it later:

```cpp
repo::SystemManager::getInstance().getAttendanceRepository()
    .addFromDomainRecord(attendanceRegister.getRecords().back());
```

There is also a simple manual Student-ID fallback if no capture method was selected.

### Current console-card simplification
For this university demo, the simulated card UID is the Student ID:

```cpp
attendance::StudentCard card(student->getStudentId(), student);
```

This is intentionally simple and should not be confused with a real card database.

### Important when Member 1 changes Lecturer.cpp
Do not replace the integrated `openAttendance()`, `markAttendance()` or `selectCaptureMethod()` with placeholder print statements. If Lecturer.cpp is rewritten, reconnect those three methods to the same Member 2 classes.

---

## 6. `src/MenuUI.cpp` — Member 3 integration

Lecturer menu now loops until logout and dispatches real Lecturer attendance functions:

```cpp
if (choice == "1") lecturer.openAttendance();
else if (choice == "2") lecturer.markAttendance();
else if (choice == "3") lecturer.viewCourses();
else if (choice == "4") lecturer.selectCaptureMethod();
else if (choice == "0") return;
```

The loop matters because the lecturer needs to perform multiple operations in one login, for example:

```text
Select Card Tap -> Open Session -> Mark Attendance -> Logout
```

Student menu also dispatches:

```cpp
if (choice == "3") student.viewTimetable();
else if (choice == "4") student.viewAttendance();
```

### Important when Member 3 changes MenuUI.cpp
Keep the real Lecturer method calls and a way to perform multiple lecturer actions before logout. Student View Timetable and View Attendance should continue calling the actual Student methods.

**Still unfinished outside Member 2 scope:** Student register/drop menu dispatch and Administrator menu operations are still placeholders in the current project.

---

## 7. `include/SystemManager.h` / `src/SystemManager.cpp` — Member 3/shared integration

Current project exposes repository access used by the Student/Lecturer attendance bridge:

```cpp
AttendanceRepository& getAttendanceRepository() noexcept;
```

Member 2 integration relies on this existing shared access path. Do not remove it without updating both Lecturer attendance persistence and Student attendance viewing.

`SystemManager::login()` is also an existing integration addition even though the supplied UML only explicitly listed `logout()`.

---

# Safe Git merge rule

If Member 1, 3, or 4 pushes newer versions of these files:

1. Pull/merge their latest code first.
2. **Do not blindly copy our whole old file over theirs.**
3. Open this document and re-apply only the integration behavior for the affected functions.
4. Compile the full project:

```bash
g++ -std=c++17 -I include main.cpp src/*.cpp -o LMS_Program.exe
```

5. Test at minimum:
   - Student registration adds a timetable slot.
   - Dropping removes the slot.
   - Lecturer selects Card Tap.
   - Lecturer opens a session.
   - Valid enrolled Student UID marks attendance.
   - Duplicate/unknown/non-enrolled attendance is rejected.
   - Student View Attendance shows the stored record.

# Member 2-owned files

These are Member 2's main files and normally should be merged as Member 2's implementation rather than treated as another member's integration edits:

- `include/TimeSlot.h`
- `include/Timetable.h`
- `src/Timetable.cpp`
- Attendance headers/sources for Session, Record, Register and Capture base
- `include/CardTapCapture.h` / `src/CardTapCapture.cpp`
- `include/ConsoleCardReader.h` / `src/ConsoleCardReader.cpp`
- `include/StudentCard.h` / `src/StudentCard.cpp`

`FileReplayCapture` remains Member 4-owned; Member 2 only uses it through the shared capture interface during integration.

## Member 2 final UI integration
- Added Lecturer menu options to close the active attendance session, view the persisted attendance report, and append an attendance correction.
- Student attendance view now collapses append-only corrections to the latest effective status per course/session and displays a per-course attendance percentage.
- These are UI/integration helpers around the existing Member 2 attendance core; they do not replace the existing attendance classes.
