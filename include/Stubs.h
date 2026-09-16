#pragma once
// ============================================================================
// Stubs.h
//
// CO2203 Group Project - Domain Model Layer
//
// Originally this held forward declarations for classes owned by OTHER
// members' subsystems (Timetable, AttendanceRegister), so Member 1's code
// could compile standalone before the modules were merged. Now that all
// three modules live together, Student.h and Course.h bring in the real
// attendance::Timetable directly (see their own comments) - those stubs
// have been removed rather than left to shadow the real types.
//
// What's left here are types genuinely still owned by this layer, or
// still-pending placeholders for Member 3's UI types.
// ============================================================================

namespace domain {

// ---- Placeholder UI return types until Member 3 delivers MenuUI/Dashboard ----
struct Dashboard {
    // Intentionally empty placeholder. Member 3's MenuUI / SystemManager
    // will likely replace this with a real view-model class.
};

struct Menu {
    // Intentionally empty placeholder for the same reason as Dashboard.
};

// ---- Grade is small enough, and specific enough to the Course hierarchy, ----
// ---- that it can genuinely live in the domain layer.                    ----
enum class Grade {
    A, B, C, D, F, INCOMPLETE
};

} // namespace domain
