cmake_minimum_required(VERSION 4.3)
project(Rartificial_Life)

set(CMAKE_CXX_STANDARD 20)

# Автоматическая обработка Qt-специфичных файлов
set(CMAKE_AUTOMOC ON)   # moc: для Q_OBJECT, сигналов и слотов
set(CMAKE_AUTORCC ON)   # rcc: для .qrc ресурсов

find_package(Qt6 REQUIRED Widgets)

# box2d
include(FetchContent)

set(BOX2D_SAMPLES OFF CACHE BOOL "" FORCE)
set(BOX2D_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v3.2.0
)
FetchContent_MakeAvailable(box2d)

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
        Brain/Model/include/ComponentTypes.h
        Brain/Managers/src/BrainMap.cpp
        Brain/Utils/include/BrainPoint.h
        Brain/Managers/include/BrainAnalyzer.h
        BrainUI/Managers/include/BrainView.h
        BrainUI/Components/include/NeuronItem.h
        BrainUI/Components/include/UIConfig.h
        BrainUI/Components/include/SynapsItem.h
        BrainUI/Components/include/AxonItem.h
        BrainUI/Components/include/DendriteItem.h
        BrainUI/Components/include/UIComponent.h
        BrainUI/Managers/include/UIController.h
        BrainUI/Managers/include/UIStorage.h
        BrainUI/Components/include/UIDatas.h
        BrainUI/Utils/include/SceneUtils.h
        BrainUI/Managers/include/UIMapper.h
        Brain/Managers/include/DataCollector.h
        Brain/Utils/include/typedefs.h
        BrainUI/Managers/include/UIBatcher.h
        GeneralUI/include/MainWindow.h
        Brain/Model/include/NeuronType.h
        Brain/Managers/src/Storage.cpp
        Agent/include/Agent.h
        Brain/Managers/include/SubMap.h
        Brain/Model/include/Genome.h
        BrainUI/Components/include/ProteinItem.h
        Brain/Model/include/Gen.h
        World/Managers/include/WorldStorage.h
        World/Components/include/WorldObject.h
        World/Components/include/AgentBody.h
        World/Managers/include/WorldController.h
        World/Components/include/WorldEntity.h
        World/Managers/include/World.h
        World/Managers/src/WorldController.cpp
        World/Managers/include/SenseGetter.h
        World/Managers/src/SenseGetter.cpp
)

target_link_libraries(Rartificial_Life PRIVATE
        Qt6::Widgets
        box2d
)

include_directories(Rartificial_Life
        ${CMAKE_SOURCE_DIR}
        ${box2d_SOURCE_DIR}/include

        Brain/Components/include
        Brain/Managers/include
        Brain/Model/include
        Brain/Utils/include

        BrainUI/Managers/include
        BrainUI/Components/include
        BrainUI/Utils/include

        World/Components/include
        World/Managers/include

        GeneralUI/include
)

target_compile_definitions(Rartificial_Life PRIVATE
    BRAIN_DEBUG
)