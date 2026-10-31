#pragma once

#include <typedefs.h>


// ему говорят данные компоненты. а он уже вызывает колбеки, тем самым говоря ui о изменениях
class DataCollector {
    // consumers
    StringConsumer createConsumer;
    StringConsumer updateConsumer;

    // при изменении консьюмера, говорим ему о том что у нас есть
    std::vector<std::string> creates_;
    std::mutex mtx_;

public:
    // events
    void onCreate(const std::string &id) {
        std::lock_guard lock(mtx_);
        creates_.push_back(id);

        if (createConsumer)
            createConsumer(id);
    }

    void onUpdate(const std::string &id) const {
        if (updateConsumer)
            updateConsumer(id);
    }

    // setters
    void setCreateConsumer(const StringConsumer &cons) {
        std::lock_guard lock(mtx_);
        createConsumer = cons;

        for (const auto id : creates_) {
            createConsumer(id);
        }
    }

    void setUpdateConsumer(const StringConsumer &cons) {
        updateConsumer = cons;
    }

    void resetConsumers() {
        createConsumer = nullptr;
        updateConsumer = nullptr;
    }
};