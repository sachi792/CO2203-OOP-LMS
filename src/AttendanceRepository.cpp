#include "AttendanceRepository.h"
#include "FileStorage.h"
#include "AttendanceRecord.h"
#include "AttendanceSession.h"
#include "Student.h"
#include "Course.h"
#include <sstream>
#include <vector>
#include <iostream>

namespace repo {

namespace {
std::vector<std::string> splitFields(const std::string& record) {
    std::vector<std::string> fields;
    std::stringstream ss(record);
    std::string field;
    while (std::getline(ss, field, '|')) {
        fields.push_back(field);
    }
    return fields;
}
} // namespace

AttendanceRepository::AttendanceRepository(const std::string& filePath)
    : Repository(std::make_shared<FileStorage>(filePath), &AttendanceRepository::toRecord,
                 &AttendanceRepository::fromRecord) {}

void AttendanceRepository::addFromDomainRecord(const attendance::AttendanceRecord& record) {
    AttendanceRecordDTO dto;
    dto.sessionId = record.getSession()->getSessionId();
    dto.courseCode = record.getSession()->getCourse().getCode();
    dto.studentId = record.getStudent()->getStudentId();
    dto.status = record.getStatus();
    dto.timestamp = record.getTimestamp();
    dto.captureMethod = record.getCaptureMethod();
    add(dto);
    // Persist immediately so a normal logout/program restart does not lose attendance.
    saveAll();
}

// Record format: sessionId|courseCode|studentId|status|timestamp|captureMethod
std::string AttendanceRepository::toRecord(const AttendanceRecordDTO& dto) {
    return std::to_string(dto.sessionId) + "|" + dto.courseCode + "|" + dto.studentId + "|" +
           dto.status + "|" + std::to_string(static_cast<long long>(dto.timestamp)) + "|" + dto.captureMethod;
}

AttendanceRecordDTO AttendanceRepository::fromRecord(const std::string& record) {
    AttendanceRecordDTO dto;
    auto fields = splitFields(record);
    try {
        if (fields.size() >= 6) {
            dto.sessionId = std::stoi(fields[0]);
            dto.courseCode = fields[1];
            dto.studentId = fields[2];
            dto.status = fields[3];
            dto.timestamp = static_cast<std::time_t>(std::stoll(fields[4]));
            dto.captureMethod = fields[5];
        } else {
            std::cerr << "[AttendanceRepository] Malformed record, skipping: " << record << "\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "[AttendanceRepository] Malformed record, skipping: " << record << " (" << ex.what() << ")\n";
    }
    return dto;
}

} // namespace repo
