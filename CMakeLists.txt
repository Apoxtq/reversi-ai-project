cmake_minimum_required(VERSION 3.20)
project(ReversiAI VERSION 1.1.0 LANGUAGES CXX)

# Packaging control: enable packaging only when explicitly requested.
# Use: cmake -DENABLE_PACKAGING=ON ...
option(ENABLE_PACKAGING "Enable creating release packages with CPack" OFF)

# C++ 标准设置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
if(MSVC)
    # Visual Studio 编译器选项
    add_compile_options(/W4 /WX- /permissive- /utf-8)
    # Avoid Windows min/max macro collisions
    add_compile_definitions(NOMINMAX)
    # Release 优化
    # Enable higher optimization and architecture-specific instructions
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob2 /arch:AVX2 /GL")
    # Link-time code generation for further optimization
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
else()
    # GCC/Clang 编译器选项
    add_compile_options(-Wall -Wextra -Wpedantic)
    # Release 优化
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -DNDEBUG")
    # Debug 选项
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    
    # MinGW 特定：静态链接运行时库以避免 DLL 依赖
    if(MINGW)
        # 静态链接 libgcc 和 libstdc++，避免需要 libgcc_s_seh-1.dll
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
        message(STATUS "MinGW detected: Using static linking for runtime libraries")
    endif()
endif()

# 输出目录设置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Make project headers available to all targets (simplify test includes)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# ==================== 依赖项配置 ====================

# 查找 SFML (optional for now, required before Week 7)
# Allow SFML_DIR to be provided by user/CI; avoid hardcoded local paths in repository.
if(NOT DEFINED SFML_DIR)
    set(SFML_DIR "" CACHE PATH "SFML installation directory (e.g. D:/SFML/SFML-2.5.1/lib/cmake/SFML)")
else()
    message(STATUS "Using provided SFML_DIR: ${SFML_DIR}")
endif()
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

# 查找 Google Test (可选，默认关闭网络下载)
option(ENABLE_GTEST "Download and build GoogleTest via FetchContent" OFF)
include(FetchContent)
find_package(GTest QUIET)
if(NOT GTEST_FOUND AND ENABLE_GTEST)
    message(STATUS "Google Test not found. Fetching GoogleTest via FetchContent...")
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.14.0.zip
    )
    # Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    find_package(GTest REQUIRED)
    if(GTEST_FOUND)
        message(STATUS "Google Test downloaded and available.")
    endif()
else()
    message(STATUS "Google Test found: ${GTEST_FOUND}")
    if(NOT GTEST_FOUND)
        message(STATUS "Google Test disabled. To enable automatic download set -DENABLE_GTEST=ON")
    endif()
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

# 研究评估源文件 (Week 10)
set(RESEARCH_SOURCES
    # 性能测试框架
    src/research/benchmark/Benchmark.cpp
    src/research/benchmark/MatchEngine.cpp
    src/research/benchmark/Statistics.cpp
    src/research/benchmark/PositionSuite.cpp
    # src/research/AlgorithmComparison.cpp  # Week 10-11
)

# ==================== 库目标 ====================

# 核心库（当前为空，后续添加源文件）
if(CORE_SOURCES)
    add_library(reversi_core STATIC ${CORE_SOURCES})
    target_include_directories(reversi_core PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    )
    # Also expose top-level src directory to consumers so tests can include <core/...>
    target_include_directories(reversi_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
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
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(reversi_ui PUBLIC 
        sfml-system sfml-window sfml-graphics
    )
    if(TARGET reversi_core)
        target_link_libraries(reversi_ui PUBLIC reversi_core)
    endif()
    if(TARGET reversi_ai_lib)
        target_link_libraries(reversi_ui PUBLIC reversi_ai_lib)
    endif()
    if(TARGET reversi_network)
        target_link_libraries(reversi_ui PUBLIC reversi_network)
    endif()
endif()

# 研究评估库 (Week 10) - 暂时禁用，有编译错误
# if(RESEARCH_SOURCES)
#     add_library(reversi_research STATIC ${RESEARCH_SOURCES})
#     target_include_directories(reversi_research PUBLIC 
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/research
#         ${CMAKE_CURRENT_SOURCE_DIR}/src
#     )
#     if(TARGET reversi_core)
#         target_link_libraries(reversi_research PUBLIC reversi_core)
#     endif()
#     if(TARGET reversi_ai_lib)
#         target_link_libraries(reversi_research PUBLIC reversi_ai_lib)
#     endif()
# endif()

# 网络库（依赖 SFML Network）
if(NETWORK_SOURCES AND SFML_FOUND)
    add_library(reversi_network STATIC ${NETWORK_SOURCES})
    target_include_directories(reversi_network PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(reversi_network PUBLIC 
        sfml-system sfml-network
    )
    if(TARGET reversi_core)
        target_link_libraries(reversi_network PUBLIC reversi_core)
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
option(BUILD_UI_APP "Build UI application (requires working SFML)" OFF)
if(BUILD_UI_APP AND UI_SOURCES AND SFML_FOUND)
    add_executable(reversi_ui_app src/main_ui.cpp)
    target_include_directories(reversi_ui_app PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    if(TARGET SFML::Graphics)
        target_link_libraries(reversi_ui_app PUBLIC 
            reversi_ui reversi_network reversi_ai_lib reversi_core
            SFML::Graphics SFML::Window SFML::System SFML::Network
        )
    else()
        target_link_libraries(reversi_ui_app PUBLIC 
            reversi_ui reversi_network reversi_ai_lib reversi_core
            sfml-system sfml-window sfml-graphics sfml-network
        )
    endif()
    
    # Windows: 链接 Winsock 库（网络功能需要）
    if(WIN32)
        target_link_libraries(reversi_ui_app PUBLIC ws2_32)
    endif()
    
    # MinGW: 完全静态链接所有运行时库（消除 DLL 依赖）
    if(MINGW)
        target_link_options(reversi_ui_app PRIVATE 
            -static 
            -static-libgcc 
            -static-libstdc++ 
            -Wl,-Bstatic 
            -lstdc++ 
            -lpthread 
            -Wl,-Bdynamic
        )
        message(STATUS "Configured for full static linking of MinGW runtime")
    endif()
    
    # 复制 SFML DLL 到输出目录（Windows）
    if(WIN32)
        set(SFML_BIN_DIR "D:/SFML/SFML-2.5.1/bin")
        add_custom_command(TARGET reversi_ui_app POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SFML_BIN_DIR}/sfml-graphics-2.dll"
            "$<TARGET_FILE_DIR:reversi_ui_app>/sfml-graphics-2.dll"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SFML_BIN_DIR}/sfml-window-2.dll"
            "$<TARGET_FILE_DIR:reversi_ui_app>/sfml-window-2.dll"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SFML_BIN_DIR}/sfml-system-2.dll"
            "$<TARGET_FILE_DIR:reversi_ui_app>/sfml-system-2.dll"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SFML_BIN_DIR}/sfml-network-2.dll"
            "$<TARGET_FILE_DIR:reversi_ui_app>/sfml-network-2.dll"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SFML_BIN_DIR}/openal32.dll"
            "$<TARGET_FILE_DIR:reversi_ui_app>/openal32.dll"
            COMMENT "Copying SFML DLLs to output directory"
        )
        
        # 复制 MinGW 运行时 DLL（SFML DLL 依赖这些）
        if(MINGW)
            # 查找 g++ 编译器路径
            get_filename_component(MINGW_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
            message(STATUS "MinGW bin directory: ${MINGW_BIN_DIR}")
            
            # 复制必需的 MinGW 运行时 DLL
            set(MINGW_DLLS
                "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
                "${MINGW_BIN_DIR}/libstdc++-6.dll"
                "${MINGW_BIN_DIR}/libwinpthread-1.dll"
            )
            
            foreach(DLL_FILE ${MINGW_DLLS})
                if(EXISTS "${DLL_FILE}")
                    add_custom_command(TARGET reversi_ui_app POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${DLL_FILE}"
                        "$<TARGET_FILE_DIR:reversi_ui_app>/"
                        COMMENT "Copying MinGW runtime: ${DLL_FILE}"
                    )
                else()
                    message(WARNING "MinGW DLL not found: ${DLL_FILE}")
                endif()
            endforeach()
        endif()
    endif()
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
# Ensure tests can find project headers
target_include_directories(test_undo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)
target_include_directories(test_hash PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

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
    
    # Week 10: Benchmark framework tests
    if(TARGET reversi_research)
        add_executable(test_benchmark_week10 tests/test_benchmark_week10.cpp)
        target_link_libraries(test_benchmark_week10 PRIVATE 
            reversi_core reversi_ai_lib reversi_research
        )
    else()
        message(STATUS "Skipping test_benchmark_week10: reversi_research target not available")
    endif()
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

# Parameter search tool (runtime tuning helper)
if(TARGET reversi_ai_lib)
    add_executable(param_search tests/param_search.cpp)
    target_link_libraries(param_search PRIVATE reversi_core reversi_ai_lib)
    target_include_directories(param_search PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )
endif()
if(TARGET reversi_ai_lib)
    add_executable(param_random tests/param_random.cpp)
    target_link_libraries(param_random PRIVATE reversi_core reversi_ai_lib)
    target_include_directories(param_random PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )
endif()
if(TARGET reversi_ai_lib)
    add_executable(param_opt tests/param_opt.cpp)
    target_link_libraries(param_opt PRIVATE reversi_core reversi_ai_lib)
    target_include_directories(param_opt PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )
endif()
if(TARGET reversi_ai_lib)
    add_executable(opt_matrix tests/opt_matrix.cpp)
    target_link_libraries(opt_matrix PRIVATE reversi_core reversi_ai_lib)
    target_include_directories(opt_matrix PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )
endif()
if(TARGET reversi_ai_lib)
    add_executable(stepwise_rollout tests/stepwise_rollout.cpp)
    target_link_libraries(stepwise_rollout PRIVATE reversi_core reversi_ai_lib)
    target_include_directories(stepwise_rollout PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )
endif()

# Week 10: Benchmark experiments
if(TARGET reversi_research)
    # Minimax vs MCTS comparison
    add_executable(minimax_vs_mcts src/research/minimax_vs_mcts.cpp)
    target_link_libraries(minimax_vs_mcts PRIVATE 
        reversi_core reversi_ai_lib reversi_research
    )
    
    # Difficulty level testing (if reversi_research target exists this will be added earlier)

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
# Configure CPack variables; include CPack only when ENABLE_PACKAGING is ON
set(CPACK_PACKAGE_NAME "ReversiAI")
set(CPACK_PACKAGE_VENDOR "Tianqixing")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Reversi AI Algorithm Benchmarking Platform")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

if(ENABLE_PACKAGING)
    message(STATUS "Packaging enabled: packages will be written to ${CMAKE_SOURCE_DIR}/dist")
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/dist")
    # prefer ZIP packages for Windows builds; scripts will rename with timestamp
    set(CPACK_GENERATOR "ZIP" CACHE STRING "CPack generator")
    # CPack output directory
    set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/dist" CACHE PATH "CPack output directory")
    include(CPack)
else()
    message(STATUS "Packaging disabled (default). To enable, configure with -DENABLE_PACKAGING=ON")
endif()

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

# (Duplicate packaging block removed - packaging handled earlier conditionally)
 
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


    # Difficulty level testing
    add_executable(difficulty_test src/research/difficulty_test.cpp)
    target_link_libraries(difficulty_test PRIVATE 
        reversi_core reversi_ai_lib reversi_research
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

