cmake_minimum_required(VERSION 4.3)
project(Rartificial_Life)

set(CMAKE_CXX_STANDARD 20)

add_executable(Rartificial_Life
        main.cpp
        Brain/Components/include/Neuron.h
        Brain/Components/include/Axon.h
        Brain/Components/include/Synaps.h
        Brain/Components/include/Dendrite.h
        "Brain/Managers/include/Executor.h"
        Brain/Managers/include/Executable.h
        Brain/Managers/include/Storage.h
        Brain/Utils/include/UUIDGenerator.h
        Brain/Managers/include/Command.h
        Brain/Managers/src/Executor.cpp
        Brain/Managers/include/Brain.h
)


include_directories(Rartificial_Life
        Brain/Components/include
        Brain/Managers/include
        Brain/Utils/include
)