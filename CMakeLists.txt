cmake_minimum_required(VERSION 4.3)
project(Rartificial_Life)

set(CMAKE_CXX_STANDARD 20)

# Автоматическая обработка Qt-специфичных файлов
set(CMAKE_AUTOMOC ON)   # moc: для Q_OBJECT, сигналов и слотов
set(CMAKE_AUTORCC ON)   # rcc: для .qrc ресурсов

find_package(Qt6 REQUIRED Widgets OpenGLWidgets)

# box2d
include(FetchContent)

set(BOX2D_SAMPLES OFF CACHE BOOL "" FORCE)
set(BOX2D_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# баг в обработке столкновений с mingw
set(BOX2D_DISABLE_SIMD ON CACHE BOOL "" FORCE)
set(BOX2D_AVX2 BOOL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v3.2.0
)
FetchContent_MakeAvailable(box2d)

add_executable(Rartificial_Life
        main.cpp
        Core/Brain/Components/include/Neuron.h
        Core/Brain/Components/include/Axon.h
        Core/Brain/Components/include/Synaps.h
        Core/Brain/Components/include/Dendrite.h
        "Core/Brain/Managers/include/Executor.h"
        Core/Brain/Managers/include/Executable.h
        Core/Brain/Managers/include/Storage.h
        Core/Brain/Utils/include/UUIDGenerator.h
        Core/Brain/Managers/include/Command.h
        Core/Brain/Managers/src/Executor.cpp
        Core/Brain/Managers/include/Brain.h
        Core/Brain/Components/include/BrainComponent.h
        my_assert.h
        Core/Brain/Managers/include/Controller.h
        Core/Brain/Managers/src/Controller.cpp
        Core/Brain/Managers/include/BrainMap.h
        Core/Brain/Model/include/ComponentTypes.h
        Core/Brain/Managers/src/BrainMap.cpp
        Core/Brain/Utils/include/BrainPoint.h
        Core/Brain/Managers/include/BrainAnalyzer.h
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
        Core/Brain/Managers/include/DataCollector.h
        Core/Brain/Utils/include/typedefs.h
        BrainUI/Managers/include/UIBatcher.h
        GeneralUI/include/MainWindow.h
        Core/Brain/Model/include/NeuronType.h
        Core/Brain/Managers/src/Storage.cpp
        Core/Agent/include/Agent.h
        Core/Brain/Managers/include/SubMap.h
        Core/Brain/Model/include/Genome.h
        BrainUI/Components/include/ProteinItem.h
        Core/Brain/Model/include/Gen.h
        Core/World/Managers/include/WorldStorage.h
        Core/World/Components/include/WorldObject.h
        Core/World/Components/include/AgentBody.h
        Core/World/Managers/include/WorldController.h
        Core/World/Components/include/WorldEntity.h
        Core/World/Managers/include/World.h
        Core/World/Managers/src/WorldController.cpp
        Core/World/Managers/include/SenseGetter.h
        Core/World/Managers/src/SenseGetter.cpp
        Core/Simulation/include/Simulation.h
        Core/Simulation/include/Model.h
        BrainUI/Utils/include/SizeUtils.h
        WorldUI/Managers/include/WorldView.h
        WorldUI/Model/include/WorldUIDatas.h
        WorldUI/Managers/include/WorldUIMapper.h
        Core/World/Model/include/EntityKind.h
        WorldUI/Managers/include/WorldUIBatcher.h
        WorldUI/Managers/include/WorldUIStorage.h
        WorldUI/Components/include/WorldObjectItem.h
        WorldUI/Model/include/WorldUIConfig.h
        WorldUI/Managers/include/WorldUIController.h
        Core/World/Managers/include/WorldDataCollector.h
        WorldUI/Components/include/AgentBodyItem.h
        WorldUI/Components/include/AgentBodyItem.h
        WorldUI/Utils/include/DrawUtils.h
        Core/Agent/include/AgentConfig.h
        Core/World/Model/include/AgentBodyConfig.h
        Core/Simulation/include/SimulationController.h
        Core/Simulation/include/SimulationStorage.h
        Core/Agent/src/Agent.cpp
        Core/Simulation/include/GenomeRandomizer.h
        RandomGenerator.h
        Core/World/Managers/include/WorldExecutor.h
        Core/World/Managers/src/WorldExecutor.cpp
        Core/World/Components/include/Food.h
        Core/World/Managers/src/WorldStorage.cpp
        WorldUI/Components/include/FoodItem.h
        Core/Brain/Managers/src/SubMap.cpp
        Core/Simulation/src/SimulationController.cpp
        GeneralUI/include/AgentTabs.h
        WorldUI/Managers/src/WorldUIController.cpp
)

target_link_libraries(Rartificial_Life PRIVATE
        Qt6::Widgets
        Qt6::OpenGLWidgets
        box2d
)

include_directories(Rartificial_Life
        ${CMAKE_SOURCE_DIR}
        ${box2d_SOURCE_DIR}/include

        Core/Brain/Components/include
        Core/Brain/Managers/include
        Core/Brain/Model/include
        Core/Brain/Utils/include

        Core/Agent/include

        Core/World/Components/include
        Core/World/Managers/include
        Core/World/Model/include

        Core/Simulation/include

        BrainUI/Managers/include
        BrainUI/Components/include
        BrainUI/Utils/include

        WorldUI/Components/include
        WorldUI/Managers/include
        WorldUI/Model/include
        WorldUI/Utils/include

        GeneralUI/include
)

target_compile_definitions(Rartificial_Life PRIVATE
    BRAIN_DEBUG
)