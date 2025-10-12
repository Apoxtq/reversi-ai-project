# Reversi AI Algorithm Benchmarking and Optimisation Research

> **COMP390 Honours Year Project (2025–26)**  
> **Student**: Tianqixing  
> **Student ID**: 201821852

---

## 🚀 快速开始

### 编译和运行

最简单的方式（推荐用于测试）：
```bash
quick_compile.bat
```

或使用CMake（推荐用于开发）：
```bash
cmake -B build -G "MinGW Makefiles"
cmake --build build
.\build\bin\reversi_ai.exe
```

详细说明请查看 [BUILD_AND_RUN.md](BUILD_AND_RUN.md)

### 系统要求

**必需：**
- C++20 编译器（GCC 13.2+ 或 Visual Studio 2019+）
- CMake 3.20+

**可选（后续阶段需要）：**
- SFML 2.5+（Week 7 - UI）
- Google Test（Week 3 - 单元测试）

---

## 📋 项目概述

本项目旨在构建一个科学的黑白棋（Reversi/Othello）AI算法比较和优化平台，实现并定量比较多种搜索算法和优化技术。项目提供完整的黑白棋游戏实现，支持本地双人、局域网多人对战和人机对战模式，并配备可重复的基准测试框架用于算法性能分析。

### 核心特性

- 🎮 **多种游戏模式**
  - 本地双人对战
  - 局域网多人对战（LAN）
  - 人机对战（可调难度）
  - AI vs AI 对战

- 🤖 **双AI引擎**
  - Minimax/Negamax + Alpha-Beta剪枝
  - Monte Carlo Tree Search (MCTS)
  - 可配置搜索深度和时间限制
  - 7级难度系统

- ⚡ **高性能优化**
  - Bitboard位运算优化
  - 置换表（Zobrist Hashing）
  - 迭代加深搜索
  - Move Ordering优化

- 📊 **科学基准测试**
  - 算法性能对比
  - 标准测试位置套件
  - 可重复的实验框架
  - 详细的性能指标

---

## 🏗️ 项目结构

```
reversi-ai-project/
├── src/
│   ├── core/           # 核心游戏逻辑
│   │   ├── Board.h/cpp      # 棋盘表示（Bitboard）
│   │   ├── Move.h/cpp       # 移动生成
│   │   └── GameState.h/cpp  # 游戏状态
│   ├── ai/             # AI引擎
│   │   ├── MinimaxEngine.h/cpp   # Minimax引擎
│   │   ├── MCTSEngine.h/cpp      # MCTS引擎
│   │   ├── Evaluator.h/cpp       # 评估函数
│   │   └── TranspositionTable.h  # 置换表
│   ├── ui/             # 用户界面（SFML）
│   │   ├── GameUI.h/cpp         # 游戏界面
│   │   ├── BoardRenderer.h/cpp  # 棋盘渲染
│   │   └── MenuSystem.h/cpp     # 菜单系统
│   └── network/        # 网络对战
│       ├── Protocol.h/cpp       # 通信协议
│       ├── NetworkGame.h/cpp    # 网络游戏
│       └── RoomManager.h/cpp    # 房间管理
├── tests/              # 单元测试
│   ├── BoardTest.cpp
│   ├── MinimaxTest.cpp
│   └── MCTSTest.cpp
├── docs/               # 文档
│   ├── notes/          # 学习笔记
│   └── diagrams/       # UML图表
├── assets/             # 资源文件
├── CMakeLists.txt      # CMake配置
└── README.md           # 项目说明

```

---

## 🛠️ 技术栈

| 类别 | 技术 | 版本 | 用途 |
|------|------|------|------|
| **语言** | C++ | 17/20 | 核心开发 |
| **构建工具** | CMake | 3.20+ | 跨平台构建 |
| **图形库** | SFML | 2.5+ | UI和网络 |
| **测试框架** | Google Test | Latest | 单元测试 |
| **编译器** | GCC/Clang/MSVC | 11+/14+/2022 | 编译工具 |

---

## 🚀 快速开始

### 环境要求

- Windows 10+ / Linux / macOS
- C++17兼容编译器（GCC 11+, Clang 14+, MSVC 2022+）
- CMake 3.20+
- SFML 2.5+
- Google Test（可选，用于测试）

### 编译步骤

```bash
# 1. 克隆项目
git clone https://github.com/YourUsername/reversi-ai-project.git
cd reversi-ai-project

# 2. 创建构建目录
mkdir build && cd build

# 3. 配置CMake
cmake ..

# 4. 编译
cmake --build . --config Release

# 5. 运行
./reversi-ai
```

### Windows (MSYS2/MinGW)

```powershell
# 安装依赖
scoop install cmake gcc sfml gtest

# 编译
mkdir build; cd build
cmake .. -G "MinGW Makefiles"
cmake --build . --config Release
```

---

## 📚 开发路线图

### Phase 0: 准备阶段 ✅
- [x] 环境配置（GCC, CMake, VSCode）
- [x] 参考项目研究
- [x] 项目初始化

### Phase 1: 基础学习（Week 1-2）
- [x] **Week 1 完成** ✅ (2025-10-11)
  - [x] Bitboard位运算学习
  - [x] 合法移动生成算法
  - [x] 移动执行与翻转
  - [x] 终局检测
  - [x] 参考代码分析（Egaroucid, Rust Reversi）
  - [x] 完整测试套件（5个测试全部通过）
  - [x] 性能基准测试（超过所有目标）
- [ ] Week 2: 移动执行优化（待开始）
- [ ] Week 2: 撤销移动功能（待开始）

### Phase 2: 核心实现（Week 3-6）
- [x] Board类实现（Bitboard）✅ - Week 1 完成
- [ ] Minimax引擎实现（Week 3-4）
- [ ] MCTS引擎实现（Week 9）
- [ ] 性能优化（Week 5-6）

### Phase 3: UI与集成（Week 7-9）
- [ ] SFML图形界面
- [ ] 网络对战模块
- [ ] 系统集成

### Phase 4: 测试与文档（Week 10-12）
- [ ] Benchmark系统
- [ ] 性能测试报告
- [ ] 完整文档

---

## 🎯 必需需求（Essential）

- ✅ Minimax/Negamax和MCTS算法实现
- ✅ Bitboard棋盘表示和高效移动生成
- ✅ 本地双人和人机对战（可调难度）
- ✅ 局域网多人对战
- ✅ 基准测试框架
- ✅ 基础UI界面

## 🌟 可选需求（Desirable）

- ⏳ 互联网多人对战（NAT穿透）
- ⏳ 并行/迭代加深搜索
- ⏳ 自适应评估函数
- ⏳ 分布式基准测试
- ⏳ 可视化（搜索树、开局/残局分析）

---

## 📊 性能目标

| 指标 | 目标 | 实际 | 状态 |
|------|------|------|------|
| Bitboard操作 | ≥100M flips/s | TBD | 🔄 |
| Minimax吞吐量 | ≥2.0M nodes/s | TBD | 🔄 |
| MCTS模拟速度 | ≥200K sims/s | TBD | 🔄 |
| LAN对战稳定性 | 10/10 games | TBD | 🔄 |
| AI稳定性 | 100 games 0 crash | TBD | 🔄 |

---

## 📖 文档

- [项目提案](docs/CA1_Detailed_Proposal.md)
- [Week 1 学习笔记](docs/notes/Bitboard_Day1.md)
- [架构设计](docs/Architecture_Design.md)（待完成）
- [性能测试报告](docs/Performance_Report.md)（待完成）

---

## 🤝 参考项目

本项目参考了以下优秀开源项目：

1. **[Egaroucid](https://github.com/Nyanyan/Egaroucid)** - 世界级黑白棋引擎（C++）
2. **[alpha-zero-general](https://github.com/suragnair/alpha-zero-general)** - AlphaZero实现（Python）
3. **[AlphaZero_Gomoku](https://github.com/junxiaosong/AlphaZero_Gomoku)** - MCTS教学实现
4. **[reversi (Rust)](https://github.com/edre/nokamute)** - Minimax参考实现
5. **[SFML](https://www.sfml-dev.org/)** - 图形和网络库

---

## 📚 文档导航

### 🚀 新手入门（从这里开始）
1. **[快速参考](QUICK_REFERENCE.md)** ⭐ - 最常用命令和概念（5分钟）
2. **[入门指南](GETTING_STARTED.md)** - 详细的环境设置和开发指南（15分钟）
3. **[构建说明](BUILD_AND_RUN.md)** - 编译、运行和故障排除（10分钟）

### 📋 项目文档
4. **[项目结构](PROJECT_SETUP_SUMMARY.md)** - 完整的架构和模块说明
5. **[技术问题手册](project_docs/TECHNICAL_ISSUES_SOLUTIONS.md)** - 问题解决方案汇总
6. **[文档索引](project_docs/DOCUMENTATION_INDEX.md)** - 完整文档导航系统

### 🔬 技术文档
7. **[Week 1笔记](docs/week1_bitboard_notes.md)** - Bitboard学习笔记
8. **[AI策略接口](src/ai/AIStrategy.hpp)** - AI算法接口文档

### ⚡ 快速开始
```bash
# 1. 克隆项目（如果从GitHub）
git clone [repo_url]

# 2. 编译并运行（最快方式）
quick_compile.bat

# 3. 查看更多帮助
cat QUICK_REFERENCE.md
```

### 📂 文档组织
```
📁 根目录/           → GitHub公开文档（给其他人看）
   ├── README.md
   ├── GETTING_STARTED.md
   └── ...
   
📁 project_docs/     → 本地项目文档（不上传GitHub）
   ├── DOCUMENTATION_INDEX.md
   ├── TECHNICAL_ISSUES_SOLUTIONS.md
   ├── PROJECT_STATUS.md
   └── ...
   
📁 docs/             → 技术文档和学习笔记
   └── week*_notes.md
```

---

## 📄 许可证

本项目采用 **MIT License** 许可证。

---

## 📮 联系方式

- **学生**: Tianqixing
- **学号**: 201821852
- **项目**: COMP390 Honours Year Project
- **学期**: 2025–26

---

## 🏆 项目状态

**当前阶段**: Phase 1 - Week 1 完成！准备 Week 2  
**开始日期**: 2025年10月8日  
**预计完成**: 2025年12月31日  
**进度**: ![](https://img.shields.io/badge/progress-20%25-orange) ✅

### ✅ Week 1 完成情况（2025-10-11）
- ✅ **Bitboard核心系统** (100%)
  - [x] Board类 - 64位棋盘表示（385行核心代码）
  - [x] 合法移动生成算法（8方向扫描）
  - [x] 移动执行和棋子翻转（calc_flip + make_move）
  - [x] 终局检测（双方无合法移动）
  - [x] Zobrist哈希基础实现
  - [x] 完整测试套件（5个测试 100%通过）
  - [x] 性能基准测试（超过所有目标）

### 📊 性能指标（Week 1 基准测试）
```
✅ 合法移动生成: ~11M ops/sec  (目标: >10M) 
✅ 移动执行:      ~11M ops/sec  (目标: >5M)
✅ 哈希计算:      ~544M ops/sec (目标: >20M)
✅ 游戏模拟:      ~124K games/s (目标: >5K)
```

### 已完成
- ✅ **Phase 0:** 环境设置 (100%)
- ✅ **Week 1:** Bitboard核心实现 (100%) 🎉
  - 3个测试程序（simple_test, test_legal_moves, benchmark）
  - 零警告编译通过
  - 完整的游戏规则文档（1694行）

### ⏳ 进行中/待开始
- 🎯 **Week 2:** 移动执行优化 + 撤销移动（下一步）
- ⏳ **Week 3-4:** Minimax引擎 + Alpha-Beta剪枝
- ⏳ **Week 5-6:** 评估函数 + 置换表优化
- ⏳ **Week 7-8:** SFML UI + 网络对战
- ⏳ **Week 9-11:** MCTS引擎 + 性能对比
- ⏳ **Week 12:** 最终测试与文档

### 🎯 关键成果
- ✨ 完整的Bitboard系统（385行核心代码）
- ✨ 高性能移动生成（11M ops/sec）
- ✨ 完整测试覆盖（5个测试用例）
- ✨ 详细的学习笔记和技术文档
- ✨ 可重复的性能基准测试框架

---

**最后更新**: 2025年10月12日

