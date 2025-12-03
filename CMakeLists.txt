cmake_minimum_required(VERSION 3.20)
project(ReversiAI VERSION 1.0.0 LANGUAGES CXX)

# C++ 标准设置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
if(MSVC)
    # Visual Studio 编译器选项
    add_compile_options(/W4 /WX- /permissive-)
    # Release 优化
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob2")
else()
    # GCC/Clang 编译器选项
    add_compile_options(-Wall -Wextra -Wpedantic)
    # Release 优化
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -DNDEBUG")
    # Debug 选项
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -fsanitize=address")
endif()

# 输出目录设置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ==================== 依赖项配置 ====================

# 查找 SFML (optional for now, required before Week 7)
# Set SFML directory (installed at D:\SFML\SFML-2.5.1)
set(SFML_DIR "D:/SFML/SFML-2.5.1/lib/cmake/SFML" CACHE PATH "SFML installation directory")
find_package(SFML 2.5 COMPONENTS system window graphics network audio)
if(SFML_FOUND)
    message(STATUS "SFML found: ${SFML_VERSION}")
    message(STATUS "SFML libraries: ${SFML_LIBRARIES}")
else()
    message(WARNING "SFML not found. UI and Network features will be disabled.")
    message(WARNING "SFML installed at: D:/SFML/SFML-2.5.1")
    message(WARNING "If CMake cannot find SFML, set SFML_DIR manually:")
    message(WARNING "  cmake -DSFML_DIR=D:/SFML/SFML-2.5.1/lib/cmake/SFML")
endif()

# 查找 Google Test (optional, required before Week 3)
find_package(GTest)
if(NOT GTEST_FOUND)
    message(WARNING "Google Test not found. Unit tests will be disabled.")
    message(WARNING "Install Google Test before Week 3: scoop install gtest")
endif()

# ==================== 项目源文件 ====================

# 核心库源文件
set(CORE_SOURCES
    # Board 相关
    src/core/Board.cpp
    src/core/Move.cpp
    # src/core/GameState.cpp  # To be implemented Week 3
)

# AI 引擎源文件
set(AI_SOURCES
    # Week 3: Minimax engine with evaluation
    src/ai/Evaluator.cpp
    src/ai/MinimaxEngine.cpp
    # Week 5: Transposition table
    src/ai/TranspositionTable.cpp
    # Week 9: MCTS engine
    src/ai/MCTSEngine.cpp
)

# UI 源文件
set(UI_SOURCES
    # Week 7: UI Core Components
    src/ui/UIStyle.cpp
    src/ui/GameState.cpp
    src/ui/UIComponent.cpp
    src/ui/MenuSystem.cpp
    src/ui/BoardRenderer.cpp
    src/ui/EventHandler.cpp
    src/ui/AnimationSystem.cpp
    src/ui/GameUI.cpp
    # Week 8: Network Lobby
    src/ui/NetworkLobbyState.cpp
)

# 网络源文件 (Week 8)
set(NETWORK_SOURCES
    src/network/NetworkProtocol.cpp
    src/network/TCPSocket.cpp
    src/network/RoomManager.cpp
    src/network/NetworkGame.cpp
)

# 研究评估源文件
set(RESEARCH_SOURCES
    # 性能测试框架
    src/research/benchmark/Benchmark.cpp
    # src/research/AlgorithmComparison.cpp  # Week 10-11
)

# ==================== 库目标 ====================

# 核心库（当前为空，后续添加源文件）
if(CORE_SOURCES)
    add_library(reversi_core STATIC ${CORE_SOURCES})
    target_include_directories(reversi_core PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    )
endif()

# AI 引擎库
if(AI_SOURCES)
    add_library(reversi_ai_lib STATIC ${AI_SOURCES})
    target_include_directories(reversi_ai_lib PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    if(TARGET reversi_core)
        target_link_libraries(reversi_ai_lib PUBLIC reversi_core)
    endif()
endif()

# UI 库（依赖 SFML）
if(UI_SOURCES AND SFML_FOUND)
    add_library(reversi_ui STATIC ${UI_SOURCES})
    target_include_directories(reversi_ui PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core
        ${CMAKE_CURRENT_SOURCE_DIR}/src/network
    )
    target_link_libraries(reversi_ui PUBLIC 
        sfml-system sfml-window sfml-graphics
    )
    if(TARGET reversi_core)
        target_link_libraries(reversi_ui PUBLIC reversi_core)
    endif()
    if(TARGET reversi_network)
        target_link_libraries(reversi_ui PUBLIC reversi_network)
    endif()
endif()

# 网络库（依赖 SFML Network）
if(NETWORK_SOURCES AND SFML_FOUND)
    add_library(reversi_network STATIC ${NETWORK_SOURCES})
    target_include_directories(reversi_network PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/network
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    )
    target_link_libraries(reversi_network PUBLIC 
        sfml-system sfml-network
    )
    if(TARGET reversi_core)
        target_link_libraries(reversi_network PUBLIC reversi_core)
    endif()
endif()

# 研究评估库
if(RESEARCH_SOURCES)
    add_library(reversi_research STATIC ${RESEARCH_SOURCES})
    target_include_directories(reversi_research PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/research
    )
    if(TARGET reversi_core)
        target_link_libraries(reversi_research PUBLIC reversi_core)
    endif()
    if(TARGET reversi_ai)
        target_link_libraries(reversi_research PUBLIC reversi_ai)
    endif()
endif()

# ==================== 主程序 ====================

# 主程序（当前创建占位符）
# Main executables
add_executable(reversi_ai src/main.cpp)
if(TARGET reversi_core)
    target_link_libraries(reversi_ai PUBLIC reversi_core)
endif()
if(TARGET reversi_ai_lib)
    target_link_libraries(reversi_ai PUBLIC reversi_ai_lib)
endif()

# Week 7: UI Application
if(UI_SOURCES AND SFML_FOUND)
    add_executable(reversi_ui_app src/main_ui.cpp)
    target_link_libraries(reversi_ui_app PUBLIC 
        reversi_ui reversi_ai_lib reversi_core
        sfml-system sfml-window sfml-graphics
    )
endif()

# 链接所有库（如果存在）
if(TARGET reversi_core)
    target_link_libraries(reversi_ai PRIVATE reversi_core)
endif()
if(TARGET reversi_ai_lib)
    target_link_libraries(reversi_ai PRIVATE reversi_ai_lib)
endif()
if(TARGET reversi_ui)
    target_link_libraries(reversi_ai PRIVATE reversi_ui)
endif()
if(TARGET reversi_network)
    target_link_libraries(reversi_ai PRIVATE reversi_network)
endif()

# 链接 SFML（如果找到）
if(SFML_FOUND)
    target_link_libraries(reversi_ai PRIVATE 
        sfml-system sfml-window sfml-graphics sfml-network
    )
endif()

# ==================== 测试 ====================

# ==================== 测试（无需Google Test）====================
enable_testing()

# Week 2 tests
add_executable(test_undo tests/test_undo.cpp)
target_link_libraries(test_undo PRIVATE reversi_core)
add_test(NAME UndoBasicTest COMMAND test_undo)

add_executable(test_hash tests/test_hash.cpp)
target_link_libraries(test_hash PRIVATE reversi_core)
add_test(NAME HashConsistencyTest COMMAND test_hash)

# Week 3 tests - Minimax engine
if(TARGET reversi_ai_lib)
    add_executable(test_minimax tests/test_minimax.cpp)
    target_link_libraries(test_minimax PRIVATE reversi_core reversi_ai_lib)
    target_include_directories(test_minimax PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )
    add_test(NAME MinimaxTest COMMAND test_minimax)
    
    # Week 5 tests - Transposition table
    add_executable(test_transposition tests/test_transposition.cpp)
    target_link_libraries(test_transposition PRIVATE reversi_core reversi_ai_lib)
    target_include_directories(test_transposition PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )
    add_test(NAME TranspositionTest COMMAND test_transposition)
    
    # Week 6 tests - Advanced search techniques
    add_executable(test_week6 tests/test_week6.cpp)
    target_link_libraries(test_week6 PRIVATE reversi_core reversi_ai_lib)
    target_include_directories(test_week6 PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )
    add_test(NAME Week6AdvancedSearchTest COMMAND test_week6)
    
    # Week 8 tests - Network module
    if(TARGET reversi_network)
        add_executable(test_network tests/test_network.cpp)
        target_link_libraries(test_network PRIVATE reversi_core reversi_network)
        target_include_directories(test_network PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/tests
        )
        add_test(NAME NetworkModuleTest COMMAND test_network)
    endif()
    
    # Week 9 tests - MCTS engine
    add_executable(test_mcts tests/test_mcts.cpp)
    target_link_libraries(test_mcts PRIVATE reversi_core reversi_ai_lib)
    target_include_directories(test_mcts PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )
    add_test(NAME MCTSEngineTest COMMAND test_mcts)
endif()

# MCTS tests (Week 9) - moved above

# ==================== Benchmark ====================

# Micro benchmark for core operations (Week 2)
add_executable(bench_micro tests/bench_micro.cpp)
target_link_libraries(bench_micro PRIVATE reversi_core)

# Week 6 benchmark - Advanced search optimizations
if(TARGET reversi_ai_lib)
    add_executable(bench_week6 tests/bench_week6.cpp)
    target_link_libraries(bench_week6 PRIVATE reversi_core reversi_ai_lib)
    target_include_directories(bench_week6 PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )
endif()

# ==================== 安装配置 ====================

# 安装可执行文件
install(TARGETS reversi_ai
    RUNTIME DESTINATION bin
)

# 安装资源文件（如果有）
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    install(DIRECTORY assets/
        DESTINATION share/reversi-ai/assets
    )
endif()

# ==================== 打包配置 ====================

set(CPACK_PACKAGE_NAME "ReversiAI")
set(CPACK_PACKAGE_VENDOR "Tianqixing")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Reversi AI Algorithm Benchmarking Platform")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

include(CPack)

# ==================== 信息输出 ====================

message(STATUS "")
message(STATUS "========== Reversi AI Project Configuration ==========")
message(STATUS "Project version: ${PROJECT_VERSION}")
message(STATUS "C++ standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "")
message(STATUS "Dependencies:")
message(STATUS "  SFML: ${SFML_FOUND}")
message(STATUS "  Google Test: ${GTEST_FOUND}")
message(STATUS "")
message(STATUS "Build targets:")
if(TARGET reversi_core)
    message(STATUS "  ✓ reversi_core")
endif()
if(TARGET reversi_ai_lib)
    message(STATUS "  ✓ reversi_ai_lib")
endif()
if(TARGET reversi_ui)
    message(STATUS "  ✓ reversi_ui")
endif()
if(TARGET reversi_network)
    message(STATUS "  ✓ reversi_network")
endif()
message(STATUS "  ✓ reversi_ai (main executable)")
message(STATUS "")
message(STATUS "Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "======================================================")
message(STATUS "")

