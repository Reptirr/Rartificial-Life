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
        Simulation/Brain/Components/include/Neuron.h
        Simulation/Brain/Components/include/Axon.h
        Simulation/Brain/Components/include/Synaps.h
        Simulation/Brain/Components/include/Dendrite.h
        "Simulation/Brain/Managers/include/Executor.h"
        Simulation/Brain/Managers/include/Executable.h
        Simulation/Brain/Managers/include/Storage.h
        Simulation/Brain/Utils/include/UUIDGenerator.h
        Simulation/Brain/Managers/include/Command.h
        Simulation/Brain/Managers/src/Executor.cpp
        Simulation/Brain/Managers/include/Brain.h
        Simulation/Brain/Components/include/BrainComponent.h
        my_assert.h
        Simulation/Brain/Managers/include/Controller.h
        Simulation/Brain/Managers/src/Controller.cpp
        Simulation/Brain/Managers/include/BrainMap.h
        Simulation/Brain/Model/include/ComponentTypes.h
        Simulation/Brain/Managers/src/BrainMap.cpp
        Simulation/Brain/Utils/include/BrainPoint.h
        Simulation/Brain/Managers/include/BrainAnalyzer.h
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
        Simulation/Brain/Managers/include/DataCollector.h
        Simulation/Brain/Utils/include/typedefs.h
        BrainUI/Managers/include/UIBatcher.h
        GeneralUI/include/MainWindow.h
        Simulation/Brain/Model/include/NeuronType.h
        Simulation/Brain/Managers/src/Storage.cpp
        Simulation/Agent/include/Agent.h
        Simulation/Brain/Managers/include/SubMap.h
        Simulation/Brain/Model/include/Genome.h
        BrainUI/Components/include/ProteinItem.h
        Simulation/Brain/Model/include/Gen.h
        Simulation/World/Managers/include/WorldStorage.h
        Simulation/World/Components/include/WorldObject.h
        Simulation/World/Components/include/AgentBody.h
        Simulation/World/Managers/include/WorldController.h
        Simulation/World/Components/include/WorldEntity.h
        Simulation/World/Managers/include/World.h
        Simulation/World/Managers/src/WorldController.cpp
        Simulation/World/Managers/include/SenseGetter.h
        Simulation/World/Managers/src/SenseGetter.cpp
        Simulation/World/Managers/include/MoveController.h
        Simulation/include/Simulation.h
        Simulation/include/Model.h
        BrainUI/Utils/include/SizeUtils.h
        WorldUI/Managers/include/WorldView.h
        WorldUI/Model/include/WorldUIDatas.h
        WorldUI/Managers/include/WorldUIMapper.h
        Simulation/World/Model/include/EntityKind.h
        WorldUI/Managers/include/WorldUIBatcher.h
        WorldUI/Managers/include/WorldUIStorage.h
        WorldUI/Components/include/WorldObjectItem.h
        WorldUI/Model/include/WorldUIConfig.h
        WorldUI/Managers/include/WorldUIController.h
        Simulation/World/Managers/include/WorldDataCollector.h
)

target_link_libraries(Rartificial_Life PRIVATE
        Qt6::Widgets
        box2d
)

include_directories(Rartificial_Life
        ${CMAKE_SOURCE_DIR}
        ${box2d_SOURCE_DIR}/include

        Simulation/Brain/Components/include
        Simulation/Brain/Managers/include
        Simulation/Brain/Model/include
        Simulation/Brain/Utils/include

        Simulation/Agent/include

        Simulation/World/Components/include
        Simulation/World/Managers/include
        Simulation/World/Model/include

        Simulation/include

        BrainUI/Managers/include
        BrainUI/Components/include
        BrainUI/Utils/include

        WorldUI/Components/include
        WorldUI/Managers/include
        WorldUI/Model/include

        GeneralUI/include
)

target_compile_definitions(Rartificial_Life PRIVATE
#    BRAIN_DEBUG
)