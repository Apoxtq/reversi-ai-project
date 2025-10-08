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
    std::cout << "\n=== Bitboard 演示 ===" << std::endl;
    
    // 黑白棋初始状态
    uint64_t player = 0x0000000810000000ULL;    // 玩家（白子）
    uint64_t opponent = 0x0000001008000000ULL;  // 对手（黑子）
    
    std::cout << "玩家棋子数量: " << __builtin_popcountll(player) << std::endl;
    std::cout << "对手棋子数量: " << __builtin_popcountll(opponent) << std::endl;
    
    // 显示棋盘（简化版）
    std::cout << "\n初始棋盘状态:" << std::endl;
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
    std::cout << "项目: COMP390 Honours Year Project" << std::endl;
    std::cout << "学生: Tianqixing (201821852)" << std::endl;
    std::cout << "主题: 黑白棋AI算法比较与优化研究" << std::endl;
    std::cout << "========================================" << std::endl;
}

// 打印开发状态
void printStatus() {
    std::cout << "\n当前开发状态:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "✅ Phase 0: 环境配置完成" << std::endl;
    std::cout << "✅ 项目结构创建完成" << std::endl;
    std::cout << "🔄 Phase 1: Week 1 - Bitboard学习中..." << std::endl;
    std::cout << "⏳ Phase 2: 核心引擎开发（待开始）" << std::endl;
    std::cout << "⏳ Phase 3: UI与网络（待开始）" << std::endl;
    std::cout << "⏳ Phase 4: 测试与文档（待开始）" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

// 打印编译信息
void printCompileInfo() {
    std::cout << "\n编译信息:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    // C++ 标准
    std::cout << "C++ 标准: ";
    #if __cplusplus == 201703L
        std::cout << "C++17" << std::endl;
    #elif __cplusplus == 202002L
        std::cout << "C++20" << std::endl;
    #else
        std::cout << "未知 (" << __cplusplus << ")" << std::endl;
    #endif
    
    // 编译器信息
    std::cout << "编译器: ";
    #if defined(__GNUC__)
        std::cout << "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << std::endl;
    #elif defined(_MSC_VER)
        std::cout << "MSVC " << _MSC_VER << std::endl;
    #elif defined(__clang__)
        std::cout << "Clang " << __clang_major__ << "." << __clang_minor__ << std::endl;
    #else
        std::cout << "未知" << std::endl;
    #endif
    
    // 构建类型
    #ifdef NDEBUG
        std::cout << "构建类型: Release" << std::endl;
    #else
        std::cout << "构建类型: Debug" << std::endl;
    #endif
    
    // 平台信息
    std::cout << "平台: ";
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
        std::cout << "未知";
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
    
    // 欢迎信息
    std::cout << "\n📚 下一步:" << std::endl;
    std::cout << "1. 完成 Week 1 Bitboard 学习" << std::endl;
    std::cout << "2. 实现 Board 类（Week 3）" << std::endl;
    std::cout << "3. 实现 Minimax 引擎（Week 4）" << std::endl;
    std::cout << "4. 实现 MCTS 引擎（Week 5）" << std::endl;
    std::cout << "\n🚀 项目启动成功！开始你的学习之旅吧！\n" << std::endl;
    
    return 0;
}

