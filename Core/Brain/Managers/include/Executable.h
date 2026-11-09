#pragma once

class Executor;

class Executable {
protected:
    Executor *executor_{};

    explicit Executable(Executor *executor) {
        executor_ = executor;
    }
};
