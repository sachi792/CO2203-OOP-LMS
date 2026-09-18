# Fixes applied after interactive testing

1. Student menu now loops until explicit logout instead of exiting after one action.
2. Student Register/Drop menu choices now dispatch to the real Student methods and CourseRepository.
3. Main application now returns to the login prompt after logout, so different roles can log in during one run.
4. AttendanceRepository loads persisted attendance at application startup.
5. New attendance records are saved immediately, so Student::viewAttendance() still works after restart.
6. Attendance session IDs advance past persisted session IDs after restart to avoid reusing old IDs.

Scope note: persistence/menu changes are integration changes in Member 3/shared files; the Member 2 attendance core remains the domain implementation.

## Final integration polish (2026-09-17)
- Administrator course creation/editing now accepts either Lecturer Person ID (for example P100) or Staff ID (for example S001).
- Optional lecturer/prerequisite prompts now accept `0` as well as blank input where appropriate.
- Recompiled successfully with C++17 after the changes.
- Scripted smoke test confirmed a course assigned using Staff ID S001 appears under Dr. Smith's taught courses.
