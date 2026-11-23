#pragma once

#include <typedefs.h>
#include <Uuid.h>


// ему говорят данные компоненты. а он уже вызывает колбеки, тем самым говоря ui о изменениях
class DataCollector {
    // consumers
    IdConsumer createConsumer;
    IdConsumer updateConsumer;

    // при изменении консьюмера, говорим ему о том что у нас есть
    std::vector<Uuid> creates_;
    std::mutex mtx_;

public:
    // events
    void onCreate(const Uuid &id) {
        std::lock_guard lock(mtx_);
        creates_.push_back(id);

        if (createConsumer)
            createConsumer(id);
    }

    void onUpdate(const Uuid &id) {
        std::lock_guard lock(mtx_);

        if (updateConsumer)
            updateConsumer(id);
    }

    // setters
    void setCreateConsumer(const IdConsumer &cons) {
        std::lock_guard lock(mtx_);
        createConsumer = cons;

        for (const auto id : creates_) {
            createConsumer(id);
        }
    }

    void setUpdateConsumer(const IdConsumer &cons) {
        std::lock_guard lock(mtx_);

        updateConsumer = cons;
    }

    void resetConsumers() {
        std::lock_guard lock(mtx_);

        createConsumer = nullptr;
        updateConsumer = nullptr;
    }
};