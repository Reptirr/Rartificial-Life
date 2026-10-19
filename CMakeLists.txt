cmake_minimum_required(VERSION 4.3)
project(Rartificial_Life)

set(CMAKE_CXX_STANDARD 20)

# Автоматическая обработка Qt-специфичных файлов
set(CMAKE_AUTOMOC ON)   # moc: для Q_OBJECT, сигналов и слотов
set(CMAKE_AUTORCC ON)   # rcc: для .qrc ресурсов

find_package(Qt6 REQUIRED Widgets)


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
        Brain/Components/include/BrainComponent.h
        my_assert.h
        Brain/Managers/include/Controller.h
        Brain/Managers/src/Controller.cpp
        Brain/Managers/include/BrainMap.h
        Brain/Components/include/ComponentTypes.h
        Brain/Managers/src/BrainMap.cpp
        Brain/Utils/include/BrainPoint.h
        Brain/Managers/include/BrainAnalyzer.h
        UI/Managers/include/BrainView.h
        UI/Components/include/NeuronItem.h
        UI/Components/include/UIConfig.h
        UI/Components/include/SynapsItem.h
        UI/Components/include/AxonItem.h
        UI/Components/include/DendriteItem.h
        UI/Components/include/UIComponent.h
)

target_link_libraries(Rartificial_Life PRIVATE Qt6::Widgets)

include_directories(Rartificial_Life
        Brain/Components/include
        Brain/Managers/include
        Brain/Utils/include

        UI/Managers/include
        UI/Components/include
)