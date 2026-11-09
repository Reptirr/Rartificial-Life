#pragma once

#include <functional>
#include <mutex>
#include <string>
#include <typedefs.h>
#include <vector>

class WorldDataCollector {
    StringConsumer createConsumer_;
    StringConsumer updateConsumer_;
    std::vector<std::string> creates_;
    std::mutex mtx_;

public:
    void onCreate(const std::string &id) {
        StringConsumer cons;
        {
            std::lock_guard lock(mtx_);
            creates_.push_back(id);
            cons = createConsumer_;      // копия, вызов уже без замка
        }
        if (cons) cons(id);
    }

    void onUpdate(const std::string &id) {
        StringConsumer cons;
        {
            std::lock_guard lock(mtx_);
            cons = updateConsumer_;
        }
        if (cons) cons(id);
    }

    void setCreateConsumer(const StringConsumer &cons) {
        std::vector<std::string> snapshot;
        {
            std::lock_guard lock(mtx_);
            createConsumer_ = cons;
            if (cons) snapshot = creates_;
        }
        if (cons)
            for (const auto &id : snapshot) cons(id);
    }

    void setUpdateConsumer(const StringConsumer &cons) {
        std::lock_guard lock(mtx_);
        updateConsumer_ = cons;
    }

    void resetConsumers() {
        std::lock_guard lock(mtx_);
        createConsumer_ = nullptr;
        updateConsumer_ = nullptr;
    }
};