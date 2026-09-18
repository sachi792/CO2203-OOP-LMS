```cpp
#pragma once
#include "Repository.h"
#include "AttendanceRecordDTO.h"
#include <string>

// Only used by the bridge function below.
namespace attendance { class AttendanceRecord; }

namespace repo {

// Stores AttendanceRecordDTO objects instead of the live attendance record.
class AttendanceRepository : public Repository<AttendanceRecordDTO> {
public:
    explicit AttendanceRepository(const std::string& filePath = "attendance.dat");

    // Converts the live record to a DTO before storing it.
    void addFromDomainRecord(const attendance::AttendanceRecord& record);

private:
    static std::string toRecord(const AttendanceRecordDTO& dto);
    static AttendanceRecordDTO fromRecord(const std::string& record);
};

} 
```
