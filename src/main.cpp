/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Main Entry Point
 */

#include <iostream>
#include <string>
#include <cstdint>

// 项目版本信息
#define PROJECT_VERSION "1.0.0"
#define PROJECT_NAME "Reversi AI"

// 简单的Bitboard演示
void demonstrateBitboard() {
    std::cout << "\n=== Bitboard Demo ===" << std::endl;
    
    // 黑白棋初始状态
    uint64_t player = 0x0000000810000000ULL;    // 玩家（白子）
    uint64_t opponent = 0x0000001008000000ULL;  // 对手（黑子）
    
    std::cout << "Player pieces: " << __builtin_popcountll(player) << std::endl;
    std::cout << "Opponent pieces: " << __builtin_popcountll(opponent) << std::endl;
    
    // 显示棋盘（简化版）
    std::cout << "\nInitial Board State:" << std::endl;
    std::cout << "  A B C D E F G H" << std::endl;
    
    for (int row = 0; row < 8; ++row) {
        std::cout << (8 - row) << " ";
        for (int col = 0; col < 8; ++col) {
            int pos = row * 8 + col;
            uint64_t mask = 1ULL << pos;
            
            if (player & mask) {
                std::cout << "○ ";  // 玩家的棋子（白）
            } else if (opponent & mask) {
                std::cout << "● ";  // 对手的棋子（黑）
            } else {
                std::cout << ". ";  // 空位
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// 打印项目信息
void printProjectInfo() {
    std::cout << "========================================" << std::endl;
    std::cout << "  " << PROJECT_NAME << " v" << PROJECT_VERSION << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Project: COMP390 Honours Year Project" << std::endl;
    std::cout << "Student: Tianqixing (201821852)" << std::endl;
    std::cout << "Topic: Reversi AI Algorithm Benchmarking" << std::endl;
    std::cout << "========================================" << std::endl;
}

// 打印开发状态
void printStatus() {
    std::cout << "\nDevelopment Status:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "[DONE] Phase 0: Environment Setup" << std::endl;
    std::cout << "[DONE] Project Structure Created" << std::endl;
    std::cout << "[ACTIVE] Phase 1: Week 1 - Bitboard Learning" << std::endl;
    std::cout << "[PENDING] Phase 2: Core Engine Development" << std::endl;
    std::cout << "[PENDING] Phase 3: UI & Network" << std::endl;
    std::cout << "[PENDING] Phase 4: Testing & Documentation" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

// 打印编译信息
void printCompileInfo() {
    std::cout << "\nBuild Information:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    // C++ Standard
    std::cout << "C++ Standard: ";
    #if __cplusplus == 201703L
        std::cout << "C++17" << std::endl;
    #elif __cplusplus == 202002L
        std::cout << "C++20" << std::endl;
    #else
        std::cout << "Unknown (" << __cplusplus << ")" << std::endl;
    #endif
    
    // Compiler Info
    std::cout << "Compiler: ";
    #if defined(__GNUC__)
        std::cout << "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << std::endl;
    #elif defined(_MSC_VER)
        std::cout << "MSVC " << _MSC_VER << std::endl;
    #elif defined(__clang__)
        std::cout << "Clang " << __clang_major__ << "." << __clang_minor__ << std::endl;
    #else
        std::cout << "Unknown" << std::endl;
    #endif
    
    // Build Type
    #ifdef NDEBUG
        std::cout << "Build Type: Release" << std::endl;
    #else
        std::cout << "Build Type: Debug" << std::endl;
    #endif
    
    // Platform
    std::cout << "Platform: ";
    #if defined(_WIN32) || defined(_WIN64)
        std::cout << "Windows";
        #ifdef _WIN64
            std::cout << " 64-bit";
        #else
            std::cout << " 32-bit";
        #endif
    #elif defined(__linux__)
        std::cout << "Linux";
    #elif defined(__APPLE__)
        std::cout << "macOS";
    #else
        std::cout << "Unknown";
    #endif
    std::cout << std::endl;
    
    std::cout << "----------------------------------------" << std::endl;
}

// 主函数
int main(int argc, char* argv[]) {
    // 打印项目信息
    printProjectInfo();
    
    // 打印编译信息
    printCompileInfo();
    
    // 打印开发状态
    printStatus();
    
    // Bitboard 演示
    demonstrateBitboard();
    
    // Next Steps
    std::cout << "\nNext Steps:" << std::endl;
    std::cout << "1. Complete Week 1 Bitboard Learning" << std::endl;
    std::cout << "2. Implement Board Class (Week 3)" << std::endl;
    std::cout << "3. Implement Minimax Engine (Week 4)" << std::endl;
    std::cout << "4. Implement MCTS Engine (Week 5)" << std::endl;
    std::cout << "\nProject initialization successful!\n" << std::endl;
    
    return 0;
}

