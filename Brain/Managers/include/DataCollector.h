#pragma once
#include <my_assert.h>
#include <typedefs.h>


// ему говорят данные компоненты. а он уже вызывает колбеки, тем самым говоря ui о изменениях
class DataCollector {
    // consumers
    StringConsumer createConsumer;
    StringConsumer updateConsumer;

public:
    // events
    void onCreate(const std::string &id) const {
        if (createConsumer)
            createConsumer(id);
    }

    void onUpdate(const std::string &id) const {
        if (updateConsumer)
            updateConsumer(id);
    }

    // setters
    void setCreateConsumer(const StringConsumer &cons) {
        createConsumer = cons;
    }

    void setUpdateConsumer(const StringConsumer &cons) {
        updateConsumer = cons;
    }

    void resetConsumers() {
        createConsumer = nullptr;
        updateConsumer = nullptr;
    }
};