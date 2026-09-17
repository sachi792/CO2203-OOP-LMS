# CO2203 LMS — Merged Repository

This is the single, integrated codebase: all three members' modules merged
into one `include/` and `src/`, plus one real entry point (`main.cpp`) and
one `CMakeLists.txt`. This is what should actually go to GitHub as the
project's main branch — the three separate module zips were the
build-in-isolation stage; this is the integration stage.

## Folder layout

```
CO2203_LMS/
├── include/        every header from all three modules, flattened
├── src/             every implementation file from all three modules
├── main.cpp         the real program entry point
├── CMakeLists.txt
└── README.md
```

No subfolders per member on purpose — with one class per file and no
filename collisions (see below), a flat layout is simplest for a project
this size. If the group prefers `include/domain/`, `include/attendance/`,
`include/repo/` subfolders for clarity as it grows, that's a safe
mechanical change (update `#include` paths + `CMakeLists.txt`'s
`target_include_directories`).

## How to build and run

**With CMake (recommended for a project this size):**
```bash
mkdir build && cd build
cmake ..
make
./lms
```

**With g++ directly** (works fine too — CMake just automates this):
```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp main.cpp -o lms
./lms
```

Either way, `lms` is one interactive program: it seeds three demo accounts,
prompts for an email/password, logs in via `SystemManager`, shows the
matching `MenuUI` menu for that role, and logs out. Run it in a real
terminal for the interactive experience. Try:

```
Email: alice@uni.lk
Password: pass456
(menu choice) 3
```

Seeded accounts: `smith@uni.lk`/`pass123` (Lecturer), `alice@uni.lk`/`pass456`
(Student), `admin@uni.lk`/`adminpw` (Administrator).

## Why "compile everything at once" works

Every `.cpp` file is compiled independently into an object file, then the
linker combines all of them into one executable — that's what
`g++ ... src/*.cpp main.cpp -o lms` does in a single command, and what
CMake's `add_library(lms_core STATIC ${LMS_SOURCES})` does more
manageably (only recompiles files that changed, tracks include
dependencies automatically). Nothing about a 30-file program is
fundamentally different from a 2-file one — it's the same
compile-each-file-then-link model, just automated instead of typed out by
hand.

## What changed during the merge (beyond just copying files)

Merging three independently-built modules surfaced a few real integration
gaps that only show up once everything has to compile and run *together*.
Each is a genuine fix, not a stylistic tweak:

1. **`TimeSlot` collision resolved.** Both the domain and attendance
   modules had their own `TimeSlot.h`. Kept the attendance version (fuller —
   it has `overlaps()`, which `Timetable::clashesWith()` needs) as the one
   canonical copy, and added `using attendance::TimeSlot;` inside
   `namespace domain` wherever domain code needs it
   (`TimetableClashException.h`, `Course.h`). Unqualified `TimeSlot` and
   qualified `domain::TimeSlot` both keep working without touching any
   existing method bodies.

2. **`Course` gained a `TimeSlot schedule_` field — this wasn't in the
   original UML.** Without it, there was nothing for `Enrollment`'s
   clash check to compare against: the UML never gives a `Course` a
   meeting time. Added `Course::setSchedule()`/`getSchedule()`.
   **Suggest adding this to the UML** — it's a real gap the diagram
   should reflect, not just an implementation detail.

3. **`Student::timetable_` was pointing at the wrong type.** Member 1's
   original `Stubs.h` forward-declared a placeholder `domain::Timetable`
   (a different, never-defined type) so the domain module could compile
   standalone. Once merged, `Student.h` needed to switch to
   `using attendance::Timetable;` — otherwise `Student::getTimetable()`
   would silently return a pointer to the wrong (incomplete, unrelated)
   type and nothing would link. `Stubs.h` has been trimmed to only what's
   still genuinely a placeholder (`Dashboard`, `Menu`, pending Member 3's
   UI work — though `SystemManager`/`MenuUI` now exist too, so even these
   could arguably be retired next).

4. **`Enrollment::checkTimetableClash()` is no longer a stub — and fixing
   it surfaced a second bug.** It now actually calls
   `student->getTimetable()->clashesWith(course->getSchedule())` and
   throws `TimetableClashException` on a real collision. First attempt at
   this still didn't work: nothing was ever adding a newly-registered
   course's slot *into* the student's timetable, so every clash check ran
   against a perpetually-empty timetable. Fixed by having
   `Enrollment::enroll()` call `timetable->addSlot(...)` on success, and
   `Enrollment::drop()` call `timetable->removeSlot(...)`. Verified with a
   standalone test: registering two courses with overlapping Monday slots
   now correctly throws `TimetableClashException` on the second
   registration.

## Known gaps carried over from the individual modules (not fixed here)

These were already flagged in each module's own README and are still open
— merging the code didn't resolve them, since they're group decisions or
larger features, not integration bugs:

- `Administrator::createUser()`/`createCourse()` are still stubs (no
  agreed input format).
- `CourseRepository` only persists each course's own scalar fields, not
  its relationships (lecturer, prerequisites, enrolled students) — no
  second "re-linking" pass after `loadAll()` exists yet.
- `AttendanceRepository` stores `AttendanceRecordDTO`s, not live
  `AttendanceRecord`s — reconstructing a live record after reload needs a
  lookup registry that doesn't exist yet.
- Passwords are not persisted (blanked on save) — no real credential
  storage strategy decided yet.
- Nobody currently constructs and attaches a `Timetable` to a `Student`
  except `main.cpp`'s demo seeding — in a real flow, `SystemManager` or
  `Administrator::createUser()` should probably own that.

## Step 8 integration note
- Member 2 attendance records are now bridged into `AttendanceRepository` after a successful lecturer mark.
- Member 1 `Student::viewAttendance()` now reads those real repository records instead of printing a placeholder.
- This is intentionally a simple university-project integration; no new UML class was introduced.

## Member 2 integration change log

Before replacing or merging `Student`, `Lecturer`, `Course`, `Enrollment`, `MenuUI`, or shared repository/SystemManager files, read [`INTEGRATION_CHANGES.md`](INTEGRATION_CHANGES.md). It records the exact cross-member integration behavior required by the Member 2 Attendance and Timetable modules, so another member's newer file can be merged without losing these connections.
