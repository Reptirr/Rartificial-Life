#pragma once

#include <functional>
#include <mutex>
#include <string>
#include <typedefs.h>
#include <vector>
#include <Uuid.h>

class WorldDataCollector {
    IdConsumer createConsumer_;
    IdConsumer updateConsumer_;
    IdConsumer removeConsumer_;
    std::vector<Uuid> creates_;
    std::mutex mtx_;

public:
    void onCreate(const Uuid &id) {
        IdConsumer cons;
        {
            std::lock_guard lock(mtx_);
            creates_.push_back(id);
            cons = createConsumer_;      // копия, вызов уже без замка
        }
        if (cons) cons(id);
    }

    void onUpdate(const Uuid &id) {
        IdConsumer cons;
        {
            std::lock_guard lock(mtx_);
            cons = updateConsumer_;
        }
        if (cons) cons(id);
    }
    void onRemove(const Uuid &id) {
        IdConsumer cons;
        {
            std::lock_guard lock(mtx_);
            cons = removeConsumer_;
        }
        if (cons) cons(id);
    }

    void setCreateConsumer(const IdConsumer &cons) {
        std::vector<Uuid> snapshot;
        {
            std::lock_guard lock(mtx_);
            createConsumer_ = cons;
            if (cons) snapshot = creates_;
        }
        if (cons)
            for (const auto &id : snapshot) cons(id);
    }

    void setUpdateConsumer(const IdConsumer &cons) {
        std::lock_guard lock(mtx_);
        updateConsumer_ = cons;
    }
    void setRemoveConsumer(const IdConsumer &cons) {
        std::lock_guard lock(mtx_);
        removeConsumer_ = cons;
    }

    void resetConsumers() {
        std::lock_guard lock(mtx_);
        createConsumer_ = nullptr;
        updateConsumer_ = nullptr;
        removeConsumer_ = nullptr;
    }
};