#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include "IStorage.h"

namespace repo {

// Generic repository used to store and load different types of objects.
// The conversion functions handle the type-specific file format.
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

}