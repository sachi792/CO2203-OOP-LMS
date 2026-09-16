#pragma once
#include "Repository.h"
#include "AttendanceRecordDTO.h"
#include <string>

// Forward declaration of Member 2's live record type, for the bridge
// function only - AttendanceRepository itself never stores this type.
namespace attendance { class AttendanceRecord; }

namespace repo {

// Repository<AttendanceRecordDTO> - see AttendanceRecordDTO.h for why this
// repository stores DTOs rather than Member 2's live AttendanceRecord.
class AttendanceRepository : public Repository<AttendanceRecordDTO> {
public:
    explicit AttendanceRepository(const std::string& filePath = "attendance.dat");

    // Bridge from Member 2's live object to a persistable DTO, then adds it.
    void addFromDomainRecord(const attendance::AttendanceRecord& record);

private:
    static std::string toRecord(const AttendanceRecordDTO& dto);
    static AttendanceRecordDTO fromRecord(const std::string& record);
};

} // namespace repo
