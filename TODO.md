# Reversi AI Project - TODO List

> **项目周期**: 12周（2025年10月8日 - 12月31日）  
> **当前进度**: Week 1 Day 1  
> **最后更新**: 2025年10月8日

---

## ✅ Phase 0: 准备阶段 (COMPLETED)
- [x] 环境配置（GCC, CMake, VSCode）
- [x] 参考项目下载和研究
- [x] CA1提案完成
- [x] GitHub仓库创建
- [x] 项目初始化

---

## 📚 Week 1: Bitboard学习 (10/8 - 10/14)

**目标**: 理解Bitboard的64位表示法和翻转算法

### Day 1 (10/8 周二) 🔄
- [ ] 阅读Egaroucid的`bit.hpp`前200行（2小时）
- [ ] 创建学习笔记：`docs/notes/Bitboard_Day1.md`
- [ ] 理解Bitboard基本概念（64位棋盘表示）
- [ ] Git提交：学习笔记Day1

### Day 2 (10/9 周三)
- [ ] 阅读Egaroucid的`bit.hpp` Line 200-500（3小时）
- [ ] 更新学习笔记：理解位运算操作
- [ ] 记录关键代码片段和注释
- [ ] Git提交：学习笔记Day2

### Day 3 (10/10 周四)
- [ ] 阅读Egaroucid的`flip.hpp`前400行（3小时）
- [ ] 学习笔记：8个方向的翻转算法
- [ ] 绘制翻转方向示意图
- [ ] Git提交：翻转算法分析

### Day 4 (10/11 周五)
- [ ] 阅读Egaroucid的`flip.hpp` Line 400-800（3小时）
- [ ] 学习笔记：边界处理技巧
- [ ] 完成flip.hpp完整分析
- [ ] Git提交：完成flip.hpp学习

### Day 5 (10/12 周六)
- [ ] 编写测试代码：`tests/bitboard_test.cpp`（4小时）
- [ ] 验证位运算操作的正确性
- [ ] 测试翻转算法
- [ ] Git提交：添加Bitboard测试

### Day 6 (10/13 周日)
- [ ] 完善学习笔记：整合Day1-5的内容（3小时）
- [ ] 编写Week 1总结：`docs/notes/Week1_Summary.md`
- [ ] 回顾本周学习重点
- [ ] Git提交：完成Week 1学习

### Week 1 检查清单
- [ ] 学习笔记：2000字+
- [ ] 理解Bitboard的64位表示
- [ ] 理解翻转算法的8个方向
- [ ] 能手写简单的位运算操作
- [ ] 总学习时间：15-18小时
- [ ] Git提交：至少6次

---

## 📚 Week 2: MCTS算法学习 (10/15 - 10/21)

**目标**: 理解MCTS的四个核心步骤（Selection/Expansion/Simulation/Backpropagation）

### 学习任务
- [ ] 阅读AlphaZero_Gomoku的`mcts_pure.py`（约200行）
- [ ] 逐行注释，理解每个步骤
- [ ] 阅读alpha-zero-general的`MCTS.py`（107行）
- [ ] 对比两种实现的差异
- [ ] 绘制MCTS搜索树流程图

### Week 2 产出
- [ ] 学习笔记：`docs/notes/MCTS_Learning.md`（3000字+）
- [ ] 流程图：`docs/diagrams/MCTS_Tree.png`
- [ ] 对比分析：`docs/notes/MCTS_Comparison.md`（1500字）
- [ ] 总学习时间：15-18小时

---

## 💻 Week 3: Board类实现 (10/22 - 10/28)

**目标**: 实现完整的Board类和Move生成

### 开发任务
- [ ] 创建`src/core/Board.h`和`Board.cpp`
- [ ] 实现Bitboard表示（参考Egaroucid）
- [ ] 实现`getLegalMoves()`（合法移动生成）
- [ ] 实现`makeMove()`和`undoMove()`
- [ ] 实现`isTerminal()`和`getResult()`
- [ ] 编写单元测试：`tests/BoardTest.cpp`

### Week 3 产出
- [ ] 源文件：`Board.h`, `Board.cpp`（约500行）
- [ ] 测试文件：`BoardTest.cpp`（约200行）
- [ ] 测试通过率：100%
- [ ] 开发时间：20-25小时

---

## 🎮 Week 4: Minimax引擎 (10/29 - 11/4)

**目标**: 实现完整的Minimax搜索引擎

### 开发任务
- [ ] 创建`src/ai/MinimaxEngine.h`和`.cpp`
- [ ] 实现基础Minimax算法
- [ ] 实现Alpha-Beta剪枝
- [ ] 实现Iterative Deepening
- [ ] 实现简单评估函数（位置权重）
- [ ] 性能测试：搜索深度6层的耗时

### Week 4 产出
- [ ] 源文件：`MinimaxEngine.h`, `MinimaxEngine.cpp`（约400行）
- [ ] 评估函数：`Evaluator.h`, `Evaluator.cpp`（约300行）
- [ ] 性能报告：`docs/Minimax_Performance.md`
- [ ] 开发时间：25-30小时

---

## 🌳 Week 5: MCTS引擎 (11/5 - 11/11)

**目标**: 实现C++版MCTS引擎

### 开发任务
- [ ] 创建`src/ai/MCTSEngine.h`和`.cpp`
- [ ] 实现MCTSNode类（搜索树节点）
- [ ] 实现Selection（UCB1公式）
- [ ] 实现Expansion
- [ ] 实现Simulation（随机rollout）
- [ ] 实现Backpropagation
- [ ] 性能测试：1000次模拟的耗时

### Week 5 产出
- [ ] 源文件：`MCTSEngine.h`, `MCTSEngine.cpp`（约600行）
- [ ] 测试对比：MCTS vs Minimax胜率测试
- [ ] 性能报告：`docs/MCTS_Performance.md`
- [ ] 开发时间：30-35小时

---

## ⚡ Week 6: 性能优化 (11/12 - 11/18)

**目标**: 性能优化和Bug修复

### 优化任务
- [ ] 实现置换表（Transposition Table）
- [ ] 实现Move Ordering
- [ ] 实现Killer Move启发
- [ ] 内存泄漏检测（Valgrind/AddressSanitizer）
- [ ] 性能分析（gprof/Perf）
- [ ] 代码重构和优化

### 性能目标
- [ ] Bitboard操作：> 1M ops/sec
- [ ] Minimax搜索（深度6）：< 5秒
- [ ] MCTS搜索（1000次）：< 2秒

### Week 6 产出
- [ ] 优化报告：`docs/Optimization_Report.md`
- [ ] 性能对比：优化前后速度提升
- [ ] Bug修复：所有已知Bug清零
- [ ] 开发时间：20-25小时

---

## 🎨 Week 7: SFML UI (11/19 - 11/25)

**目标**: 完整的图形界面

### 开发任务
- [ ] 学习SFML基础（窗口、形状、事件）
- [ ] 实现棋盘绘制
- [ ] 实现棋子绘制（黑/白/提示）
- [ ] 实现点击交互
- [ ] 实现动画效果（翻转动画）
- [ ] 实现菜单系统

### Week 7 产出
- [ ] 源文件：`src/ui/GameUI.h`, `GameUI.cpp`（约800行）
- [ ] 资源文件：棋盘背景、音效（可选）
- [ ] 用户体验：流畅的交互和美观的界面
- [ ] 开发时间：25-30小时

---

## 🌐 Week 8: 网络对战 (11/26 - 12/2)

**目标**: 实现LAN局域网对战

### 开发任务
- [ ] 学习SFML Network模块
- [ ] 实现TCP Socket通信
- [ ] 实现客户端/服务器架构
- [ ] 实现Move同步协议
- [ ] 实现断线重连机制
- [ ] 测试局域网对战

### Week 8 产出
- [ ] 源文件：`src/network/Network.h`, `Network.cpp`（约600行）
- [ ] 协议文档：`docs/Network_Protocol.md`
- [ ] 测试报告：局域网对战测试（2台电脑）
- [ ] 开发时间：20-25小时

---

## 🔧 Week 9: 系统集成 (12/3 - 12/9)

**目标**: 整合所有模块

### 开发任务
- [ ] 集成Board + MinimaxEngine + MCTSEngine + UI
- [ ] 实现游戏状态管理（State Machine）
- [ ] 实现存档/读档功能
- [ ] 实现游戏回放功能（Desirable）
- [ ] 完整游戏流程测试
- [ ] 用户体验优化

### Week 9 产出
- [ ] 源文件：`src/Game.h`, `Game.cpp`（约500行）
- [ ] 完整可玩的游戏Demo
- [ ] 功能测试报告：所有Essential需求验证
- [ ] 开发时间：25-30小时

---

## 📊 Week 10: Benchmark系统 (12/10 - 12/16)

**目标**: 实现AI性能评估系统

### 开发任务
- [ ] 参考alpha-zero-general的`Arena.py`
- [ ] 实现AI vs AI对战框架
- [ ] 实现统计分析（胜率、平均步数）
- [ ] 对比Minimax vs MCTS
- [ ] 对比不同难度级别
- [ ] （可选）GTP协议实现，与Egaroucid对战

### Benchmark测试用例
- [ ] Test Suite 1: Minimax vs Random (100局)
- [ ] Test Suite 2: MCTS vs Random (100局)
- [ ] Test Suite 3: Minimax vs MCTS (100局)
- [ ] Test Suite 4: 不同难度级别对战 (50局/难度)
- [ ] Test Suite 5: Egaroucid对战 (20局) - 可选

### Week 10 产出
- [ ] 源文件：`Benchmark.h`, `Benchmark.cpp`（约400行）
- [ ] 测试报告：`docs/Benchmark_Report.md`（3000字+）
- [ ] 数据可视化：胜率图表、性能曲线
- [ ] 开发时间：20-25小时

---

## 📝 Week 11: CA2文档 (12/17 - 12/23)

**目标**: 撰写CA2项目报告

### 文档任务
- [ ] 项目架构设计文档
- [ ] 核心算法实现说明
- [ ] 性能测试与优化报告
- [ ] 用户手册（使用说明）
- [ ] 代码注释完善（Doxygen）
- [ ] UML类图和序列图

### Week 11 产出
- [ ] CA2完整报告（10,000字+）
- [ ] UML图表（5-8张）
- [ ] 界面截图（10+张）
- [ ] 代码文档（Doxygen生成）
- [ ] 文档时间：25-30小时

---

## ✅ Week 12: 最终测试与提交 (12/24 - 12/31)

**目标**: 最终验证与提交

### 测试任务
- [ ] 完整功能回归测试
- [ ] 用户验收测试（找同学试玩）
- [ ] 性能压力测试
- [ ] Bug修复和优化
- [ ] 代码格式化（clang-format）
- [ ] 提交前检查清单

### 提交清单
- [ ] 源代码（完整、可编译）
- [ ] 可执行文件（Windows/Linux）
- [ ] CA2报告（PDF格式）
- [ ] README.md（项目说明）
- [ ] 构建脚本（CMakeLists.txt）
- [ ] 演示视频（可选）

### Week 12 产出
- [ ] 最终可提交的完整项目
- [ ] 通过率：100%功能正常
- [ ] 文档完整性：100%
- [ ] 开发时间：20-25小时

---

## 📌 关键截止日期

| 日期 | 里程碑 | 状态 |
|------|--------|------|
| **2025-10-29** | CA1提案截止 | 🔴 重要 |
| **2025-11-04** | Week 4验收点（Minimax完成） | 🟡 中等 |
| **2025-12-02** | Week 8验收点（网络对战完成） | 🟡 中等 |
| **2025-12-31** | 最终提交 | 🔴 重要 |

---

## 📊 时间分配总览

| 阶段 | 周数 | 总时间 | 平均每周 |
|------|------|--------|---------|
| Phase 1: 基础学习 | 2周 | 30小时 | 15小时/周 |
| Phase 2: 核心实现 | 4周 | 110小时 | 27.5小时/周 |
| Phase 3: UI与集成 | 3周 | 75小时 | 25小时/周 |
| Phase 4: Benchmark与文档 | 3周 | 70小时 | 23小时/周 |
| **总计** | **12周** | **285小时** | **23.75小时/周** |

**时间弹性**: 预留10%缓冲时间（约30小时）用于应对意外问题

---

## 💡 学习技巧

### 1. 不要追求完美
- 第一遍阅读，理解50-60%就够了
- 不懂的地方先标记，继续往后读
- 后面会越来越清晰

### 2. 做笔记的方法
```markdown
### 关键代码
\`\`\`cpp
uint64_t board = 0x0000001818000000ULL;
\`\`\`
**理解**: ...
**疑问**: ❓ ...
```

### 3. 时间管理
- 严格遵守番茄工作法（25分钟工作 + 5分钟休息）
- 4个番茄后，休息15分钟
- 不要跳过休息时间！

### 4. 遇到困难
- 超过30分钟无进展 → 先跳过，标记❓
- 想放弃 → 休息10分钟，降低目标
- 完全看不懂 → 先看简单的教程

---

## 🎯 本周重点（Week 1）

**核心任务**: 理解Bitboard和翻转算法

**学习材料**:
- `D:\大三390毕业选课黑白棋项目\他人成熟黑白棋项目参考\Egaroucid\src\engine\bit.hpp`
- `D:\大三390毕业选课黑白棋项目\他人成熟黑白棋项目参考\Egaroucid\src\engine\flip.hpp`

**期望产出**:
- 学习笔记 2000字+
- 理解64位棋盘表示
- 理解8个方向翻转
- 完成测试代码

---

**创建日期**: 2025年10月8日  
**下次更新**: 每周日晚  
**当前状态**: Week 1 Day 1 🚀

