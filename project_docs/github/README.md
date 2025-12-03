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

### Phase 1: 基础学习（Week 1-2）✅
- [x] Bitboard位运算学习 ✅
- [x] 参考代码分析 ✅

### Phase 2: 核心实现（Week 3-6）✅
- [x] Board类实现（Bitboard）✅
- [x] Minimax引擎实现 ✅
- [x] 性能优化 ✅

### Phase 3: UI与集成（Week 7-8）✅
- [x] SFML图形界面 ✅
- [x] 网络对战模块 ✅
- [x] 系统集成 ✅

### Phase 4: 研究与优化（Week 9-11）🔄
- [x] MCTS引擎实现 ✅ (Week 9完成)
- [ ] 性能基准测试 (Week 10计划)
- [ ] 算法对比研究 (Week 10-11计划)

### Phase 5: 测试与文档（Week 12）⏳
- [ ] Benchmark系统完善
- [ ] 性能测试报告
- [ ] 完整文档

---

## 🎯 必需需求（Essential）

- ✅ **Minimax/Negamax算法实现** (Week 3-6完成)
- ✅ **MCTS算法实现** (Week 9完成) ✅
- ✅ **Bitboard棋盘表示和高效移动生成** (Week 1-2完成)
- ✅ **本地双人和人机对战（可调难度）** (Week 3完成)
- ✅ **局域网多人对战** (Week 8完成) ✅
- ✅ **基准测试框架** (Week 2-6完成)
- ✅ **基础UI界面** (Week 7-8完成) ✅

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
| Bitboard操作 | ≥10M ops/s | 19.6M ops/s | ✅ 196% |
| Minimax吞吐量 | ≥2.0M nodes/s | ~3.0M nodes/s | ✅ 150% |
| MCTS模拟速度 | ≥200K sims/s | 待测试 | ⏳ Week 10 |
| LAN对战稳定性 | 10/10 games | 待测试 | ⏳ Week 10 |
| AI稳定性 | 100 games 0 crash | 待测试 | ⏳ Week 10 |

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

**当前阶段**: Phase 4 - Week 9完成，Week 10进行中  
**开始日期**: 2025年10月8日  
**预计完成**: 2025年12月31日  
**进度**: ![](https://img.shields.io/badge/progress-75%25-green) ✅

### 完成情况
- ✅ **Phase 0:** 环境设置 (100%)
- ✅ **Phase 1:** 基础学习 Week 1-2 (100%)
- ✅ **Phase 2:** 核心实现 Week 3-6 (100%)
- ✅ **Phase 3:** UI与集成 Week 7-8 (100%)
- 🔄 **Phase 4:** 研究与优化 Week 9-11 (Week 9完成，Week 10进行中)
- ⏳ **Phase 5:** 完成交付 Week 12 (待开始)

### 最新完成（2025-12-03）
- ✅ **Week 9:** MCTS引擎实现全部完成
  - ✅ 四个阶段完整实现（Selection, Expansion, Simulation, Backpropagation）
  - ✅ UCB1选择策略（可配置探索常数）
  - ✅ 启发式对局策略（位置评估+机动性）
  - ✅ AIStrategy接口实现（与Minimax可互换）
  - ✅ 单元测试（8个测试用例，全部通过）

---

**最后更新**: 2025年12月3日

