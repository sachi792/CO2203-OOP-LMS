#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include "IStorage.h"

namespace repo {

// Generic Repository<T>, matching the UML's fields (items, storage) and
// methods (add, remove, saveAll, loadAll).
//
// T is expected to be a pointer-like handle to a domain object (this
// project uses std::shared_ptr<Base> everywhere - see UserRepository /
// CourseRepository / AttendanceRepository). Repository itself has NO
// domain knowledge: converting a T to/from a persisted string record is
// supplied by the caller as two small functions, which is what lets a
// single Repository<T> template serve three very different domain types
// without needing three different persistence implementations.
template <typename T>
class Repository {
public:
    using ToRecordFn = std::function<std::string(const T&)>;
    using FromRecordFn = std::function<T(const std::string&)>;

    Repository(std::shared_ptr<IStorage> storage, ToRecordFn toRecord, FromRecordFn fromRecord)
        : storage_(std::move(storage)), toRecord_(std::move(toRecord)), fromRecord_(std::move(fromRecord)) {}

    virtual ~Repository() = default;

    void add(const T& item) {
        items_.push_back(item);
    }

    void remove(const T& item) {
        items_.erase(std::remove(items_.begin(), items_.end(), item), items_.end());
    }

    void saveAll() {
        std::vector<std::string> records;
        records.reserve(items_.size());
        for (const auto& item : items_) {
            records.push_back(toRecord_(item));
        }
        storage_->save(records);
    }

    void loadAll() {
        items_.clear();
        for (const auto& record : storage_->load()) {
            items_.push_back(fromRecord_(record));
        }
    }

    const std::vector<T>& getItems() const noexcept { return items_; }

protected:
    std::vector<T> items_;
    std::shared_ptr<IStorage> storage_;
    ToRecordFn toRecord_;
    FromRecordFn fromRecord_;
};

} // namespace repo
