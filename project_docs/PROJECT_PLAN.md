# Reversi AI 项目计划与进度

**学生：** 田祺星 (Tianqixing) - 201821852  
**学年：** 2025-26  
**项目周期：** 12周 (2025年10月8日 - 12月31日)  
**最后更新：** 2025年11月28日

---

## 📊 项目总览

| Phase | 状态 | 进度 | 时间 |
|-------|------|------|------|
| **Phase 0: 环境设置** | ✅ 完成 | 100% | Week 0 |
| **Phase 1: 核心开发 (Week 1-6)** | ✅ 完成 | 100% | ~146h |
| **Phase 2: UI与网络 (Week 7-8)** | ⏳ 待开始 | 0% | ~75h |
| **Phase 3: 研究优化 (Week 9-11)** | ⏳ 待开始 | 0% | ~70h |
| **Phase 4: 完成交付 (Week 12)** | ⏳ 待开始 | 0% | ~30h |

**整体进度：** 52% (Week 1-6完成) | **已用时间：** ~146小时 | **预估总时间：** 285小时

**当前状态：** Week 5 置换表 ✅ 已完成 | Week 6 高级搜索 ✅ 已完成

---

## ✅ Phase 0: 环境设置 (已完成)

### 开发环境 ✅
- [x] GCC 13.2.0 (C++20支持)
- [x] CMake 4.1.2
- [x] Git 2.47.0
- [x] Scoop 包管理器
- [x] 项目结构搭建

### 核心文件 ✅
- [x] Board.hpp/cpp - 棋盘类
- [x] Move.hpp/cpp - 移动类
- [x] AIStrategy.hpp - AI接口
- [x] Benchmark.hpp/cpp - 性能测试
- [x] Build scripts (quick_compile.bat, build.bat, clean.bat)

### 文档系统 ✅
- [x] README.md
- [x] 项目结构文档
- [x] .gitignore配置

---

## 🔄 Phase 1: 核心开发 (Week 1-6) - 进行中

### Week 1: Bitboard基础 (10/8-10/14) - ✅ 100%完成

#### 已完成 ✅
- [x] Bitboard结构实现
- [x] 初始位置设置
- [x] 棋子计数 (std::popcount)
- [x] 棋盘显示
- [x] 基础Move类
- [x] 项目成功编译运行
- [x] **合法移动生成算法** ✨
  - [x] 8个方向的扫描
  - [x] 边界处理 (防止wraparound)
  - [x] 空位检测与验证
- [x] **翻转棋子计算** ✨
  - [x] calc_flip() 函数实现
  - [x] 方向扫描累积候选翻转
- [x] **移动执行** ✨
  - [x] make_move() 函数实现
  - [x] 自动交换player/opponent
- [x] **终局检测** ✨
  - [x] is_terminal() 函数实现
  - [x] 检测双方都无合法移动
- [x] **测试套件** ✨
  - [x] simple_test.cpp 创建
  - [x] 验证初始位置4个合法移动
  - [x] 验证移动执行和翻转
  - [x] 所有测试通过

**完成日期：** 2025年10月11日  
**实际时间：** ~22小时  
**参考资料：**
- Egaroucid项目 (flip_generic.hpp)
- Rust Reversi项目 (rules.rs)
- 自主实现的方向扫描算法

**测试结果：**
```
✅ 初始位置: 4个合法移动 (d6, c5, f4, e3)
✅ 移动执行: 正确翻转1个棋子
✅ 棋盘更新: player/opponent正确交换
✅ 合法移动重新计算: 对手3个合法移动
✅ 终局检测: 正确识别满盘状态
✅ 测试套件: 5个测试全部通过
```

**性能指标：**
```
✅ 合法移动生成: ~11M ops/sec (目标: >10M)
✅ 移动执行: ~11M ops/sec (目标: >5M)
✅ 哈希计算: ~544M ops/sec (目标: >20M)
✅ 游戏模拟: ~124K games/sec (目标: >5K)
```

**交付成果：**
- ✅ 3个测试程序（simple_test, test_legal_moves, benchmark）
- ✅ 完整的性能基准测试
- ✅ 代码零警告编译通过
- ✅ 详细的学习笔记（Bitboard Day1）
- ✅ 完整的游戏规则文档（1694行）

---

### Week 2: 移动执行与优化 (10/15-10/21) - ✅ 已完成

- [x] ✅ 撤销移动功能 (undo_move)
- [x] ✅ Zobrist哈希基础
- [x] ✅ 移动执行优化
- [x] ✅ 性能优化与测试（微基准）
- [x] ✅ 代码重构与文档

**计划时间：** ~18-20小时  
**实际时间：** 18小时（周二-周四，共三天）  
**完成日期：** 2025年10月16日  
**注：** Week 1已提前完成移动执行、翻转、终局检测

#### Week 2 性能基准测试结果（Release -O3, Windows 11, GCC 13.2）

**核心操作性能（微基准）：**
```
✅ legal_moves生成:      19.4 M ops/sec  (目标: >15M, 超额29%)
✅ make_move+undo:        9.4 M ops/sec  (目标: >5M, 超额88%)
✅ pass_pair (双Pass):    26.1 M ops/sec (目标: >10M, 超额161%)
✅ random_playout:        5.3 M steps/sec (目标: >3M, 超额77%)
✅ Zobrist哈希:          544 M ops/sec  (目标: >100M, 超额444%)
```

**内存与缓存效率：**
```
✅ Board对象大小:        24 bytes (< 1个cache line)
✅ 移动列表预分配:       64 bytes (std::array, 零动态分配)
✅ 缓存友好度:           优秀 (所有关键数据结构对齐)
```

**功能验证：**
```
✅ 单元测试:            45/45 通过 (100%)
✅ 合法移动正确性:      所有FForum测试位置验证通过
✅ 移动执行/撤销:       往返一致性验证通过
✅ Zobrist哈希:         碰撞率 < 0.01%
```

**性能优化说明：**
- `pass_pair` 当前实现采用哈希重算（未做增量异或优化），作为后续优化的基线
- `make_move+undo` 展示了历史栈 + 哈希缓存一致性维护的开销
- 后续可通过增量Zobrist更新减少重算成本（预计再提升15-20%）

**Week 2交付成果：**
- ✅ 完整的移动生成/执行/撤销系统
- ✅ Zobrist哈希增量更新
- ✅ 45个单元测试 (100%通过率)
- ✅ 完整的性能基准测试框架
- ✅ 代码重构与优化 (零警告编译)
- ✅ 详细的性能分析报告


---

### Week 3: Minimax引擎 + 测试 (10/18-10/22) ✅ **已完成**

- [x] Minimax算法实现 (递归搜索框架)
- [x] Alpha-Beta剪枝优化
- [x] 位置评估函数 (材料+行动力+角控制)
- [x] AI vs Human交互程序 (`ai_play.cpp`)
- [x] 核心功能单元测试 (`test_evaluator.cpp`, `test_minimax.cpp`)
- [x] 性能基准测试 (`bench_week3.cpp`)

**Week 3性能指标：**
```
✅ Minimax搜索深度:    Depth 1-9 可用
✅ 搜索速度:          1.91 M nodes/sec (目标: 2M, 达成95%)
✅ Alpha-Beta剪枝:    67.5%节点减少, 3.07x加速 (目标: >50%, 超额35%)
✅ 搜索稳定性:        深度6: 2176节点, 深度5: 707节点 (Alpha-Beta)
✅ 实时响应:          深度5: ~1ms, 深度7: ~12ms (可接受延迟)
```

**评估函数组件：**
```
✅ 材料评分:          棋子数量差异 (权重: 1x)
✅ 行动力评分:        合法移动数差异 (权重: 3x)
✅ 角控制:            每个角 +25分 (关键战略位置)
✅ 位置权重:          边缘/中心/危险区策略评分
✅ 对称性:            黑白视角对称, 零和博弈保证
```

**交互AI程序特性：**
```
✅ 难度等级:          Easy/Medium/Hard/Expert (深度1/3/5/7+)
✅ 实时棋盘显示:      ASCII艺术棋盘, 代数记号法
✅ 移动验证:          自动检查合法性, 友好错误提示
✅ 性能统计:          搜索节点数, 时间, 加速比显示
✅ 游戏结束:          自动检测获胜/平局/无移动
```

**测试覆盖：**
```
✅ 评估函数测试:      材料/行动力/角/对称性验证
✅ Minimax正确性:     2-3层简单位置验证
✅ Alpha-Beta等价:    与普通Minimax结果一致性
✅ 游戏规则:          移动合法性, Pass处理, 终局检测
✅ 性能基准:          深度缩放, 分支因子, 剪枝效率
```

**Week 3交付成果：**
- ✅ 完整的Minimax搜索引擎 (支持深度1-9)
- ✅ Alpha-Beta剪枝优化 (3x加速)
- ✅ 多组件位置评估器
- ✅ 交互式AI对战程序 (4个难度级别)
- ✅ 全面的单元测试套件
- ✅ 性能基准测试框架
- ✅ 详细学习笔记 (`docs/week3_minimax_ai_notes.md`)

**时间：** ~22小时实际完成

---

### Week 4: 高级评估函数 (11/3-11/10) ✅ **已完成**

**说明：** Week 3已实现基础评估函数（位置权重+行动力+角控制），Week 4实现高级特性。

**核心任务：**
- [x] **稳定性分析框架** (StabilityAnalyzer) ✨ 已完成
  - [x] 角稳定性检测 (Corner stability)
  - [x] 边缘稳定性传播 (Edge stability)
  - [x] 前沿棋子计算 (Frontier discs)
  - [x] 完整稳定性分析系统
- [x] **阶段化权重系统** (PhaseWeights) ✨ 已完成
  - [x] 游戏阶段检测 (Opening/Midgame/Endgame/Final)
  - [x] 动态权重配置
  - [x] 平滑权重过渡
- [x] **增强型评估器** (Evaluator_Week4) ✨ 已完成
  - [x] 集成稳定性分析
  - [x] 阶段化评估策略
  - [x] 潜在行动力 (Potential mobility)
  - [x] 角控制加成优化
- [x] **自对弈测试系统** 🔧 已完成
  - [x] AI vs AI对战框架 (self_play.cpp)
  - [x] 统计数据收集
  - [x] Win rate分析工具
- [x] **性能优化** ✅ 已完成
  - [x] 稳定性计算优化
  - [x] 评估函数性能测试
  - [x] 目标: >30M evals/sec（达成）

**已完成交付成果：**
- ✅ StabilityAnalyzer.hpp/cpp (~200行)
- ✅ PhaseWeights.hpp (~174行)
- ✅ Evaluator_Week4.hpp/cpp (~264行)
- ✅ self_play.cpp (~268行)
- ✅ test_stability.cpp (~420行)
- ✅ 编译脚本 (build_week4.bat/ps1)

**时间：** ~24小时已用 / ~25小时计划

**进度：** 已完成 (100%)

---

### Week 5: 置换表与优化 (11/10-11/17) ✅ **已完成**

**说明：** 原Week 2已完成Zobrist哈希基础，Week 5将实现完整的置换表系统。

**核心任务：**
- [x] **置换表数据结构** (TranspositionTable) ✨ 已完成
  - [x] TTEntry结构（hash, score, depth, flag, best_move）
  - [x] 哈希表实现（2^N大小，快速取模）
  - [x] 内存对齐优化（16字节对齐）
- [x] **表项替换策略** ✨ 已完成
  - [x] 深度优先替换（Depth-preferred）
  - [x] 空槽检测和碰撞处理
- [x] **哈希碰撞处理** ✨ 已完成
  - [x] 64位哈希值验证
  - [x] 碰撞检测机制（hash匹配）
- [x] **MinimaxEngine集成** ✨ 已完成
  - [x] 搜索前查询置换表
  - [x] 搜索后存储结果
  - [x] Alpha-Beta边界处理（EXACT/LOWER/UPPER）
- [x] **Move Ordering优化** ✨ 已完成
  - [x] 置换表移动优先排序
  - [x] 与现有评估函数结合
- [x] **性能基准测试** ✅ 已完成
  - [x] 缓存命中率统计（命中率 41-58% @ d=6-8）
  - [x] 搜索速度对比（+2.1x ~ +3.4x）
  - [x] 内存使用监控（可配置表大小）
- [x] **单元测试** ✅ 已完成
  - [x] test_transposition.cpp
  - [x] 置换表存储/检索测试
  - [x] 深度边界测试

**已完成交付成果：**
- ✅ TranspositionTable.hpp (~200行)
- ✅ TranspositionTable.cpp (~100行)
- ✅ MinimaxEngine.hpp/cpp 更新（集成置换表与Move Ordering）
- ✅ tests/test_transposition.cpp（全部通过）
- ✅ docs/week5_transposition_notes.md 学习笔记
- ✅ build_test_week5.bat（测试构建脚本）

**时间：** ~20小时已用 / ~20-25小时计划

**进度：** 全部完成 (100%)

---

### Week 6: 高级搜索技术 (11/6-11/12) ✅ **已完成**

**说明：** Week 6实现了所有高级搜索优化技术，显著提升了搜索引擎的性能和可控性。

**核心任务：**
- [x] **迭代加深搜索** (Iterative Deepening) ✨ 已完成
  - [x] 从深度1逐步增加到max_depth
  - [x] 利用置换表加速深层搜索
  - [x] 支持时间限制中断
  - [x] 实现位置：`iterative_deepening_search()`
- [x] **时间管理系统** ✨ 已完成
  - [x] 基于游戏阶段的动态时间分配
  - [x] 开局15%、中局30%、残局55%时间分配
  - [x] 定期时间检查（每1000节点）
  - [x] 实现位置：`calculate_time_limit()`, `time_exceeded()`
- [x] **Principal Variation Search (PVS)** ✨ 已完成
  - [x] 第一个移动完整窗口搜索
  - [x] 后续移动零窗口搜索
  - [x] 失败时重新搜索完整窗口
  - [x] 保持向后兼容（使用重载）
  - [x] 实现位置：`pvs()`
- [x] **搜索窗口优化** (Aspiration Windows) ✨ 已完成
  - [x] 使用预测分数的小窗口搜索
  - [x] 失败时扩大窗口重新搜索
  - [x] 与置换表集成
  - [x] 实现位置：`aspiration_search()`
- [x] **Killer Move启发** ✨ 已完成
  - [x] 每层记录两个killer move（修正设计：使用MAX_DEPTH）
  - [x] 集成到移动排序中
  - [x] Beta剪枝时自动更新
  - [x] 实现位置：`update_killer()`, `get_killer_bonus()`, `order_moves()`
- [x] **增强的移动排序** ✅ 已完成
  - [x] TT best move优先
  - [x] Killer move加成
  - [x] 评估函数辅助排序
- [x] **单元测试** ✅ 已完成
  - [x] test_week6.cpp（8个测试用例）
  - [x] 测试所有新功能正确性
- [x] **性能基准测试** ✅ 已完成
  - [x] bench_week6.cpp（5个基准测试）
  - [x] 量化各优化技术的性能贡献

**已完成交付成果：**
- ✅ MinimaxEngine.hpp/cpp 更新（新增约550行代码）
- ✅ tests/test_week6.cpp（单元测试，8个测试用例）
- ✅ tests/bench_week6.cpp（性能基准测试，5个基准）
- ✅ 所有功能可独立配置开关
- ✅ 完整的Doxygen注释

**时间：** ~28小时已用 / ~25-30小时计划

**进度：** 全部完成 (100%)

**性能提升：**
- 迭代加深：时间可控，利用置换表加速
- PVS：搜索节点减少15-25%
- Aspiration Windows：搜索速度提升20-30%
- Killer Moves：移动排序提升，剪枝效率提升10-15%
- 综合：整体搜索速度提升30-50%

**里程碑：高性能Minimax引擎完成** 🎯 ✅

---

## ⏳ Phase 2: UI与网络 (Week 7-8)

### Week 7: SFML UI (11/19-11/25)

- [ ] 学习SFML基础
- [ ] 棋盘渲染
- [ ] 棋子绘制
- [ ] 鼠标交互
- [ ] 移动动画
- [ ] 菜单系统

**时间：** ~25-30小时

---

### Week 8: 网络对战 (11/26-12/2)

- [ ] SFML Network模块
- [ ] TCP Socket通信
- [ ] 客户端/服务器架构
- [ ] 移动同步协议
- [ ] 断线重连
- [ ] 局域网测试

**里程碑：完整游戏可玩** 🎯

**时间：** ~20-25小时

---

## ⏳ Phase 3: 研究与优化 (Week 9-11)

### Week 9: MCTS实现 (12/3-12/9)

- [ ] MCTS基础算法
- [ ] UCB1选择策略
- [ ] 树策略
- [ ] 默认策略
- [ ] MCTS vs Minimax对比

**时间：** ~30-35小时

---

### Week 10: Benchmark系统 (12/10-12/16)

- [ ] AI vs AI对战框架
- [ ] 统计分析
- [ ] Minimax vs MCTS对比
- [ ] 不同难度级别测试
- [ ] (可选) GTP协议对战Egaroucid

**时间：** ~20-25小时

---

### Week 11: CA2文档 (12/17-12/23)

- [ ] 项目架构设计文档
- [ ] 核心算法实现说明
- [ ] 性能测试报告
- [ ] 用户手册
- [ ] UML类图和序列图
- [ ] 代码注释完善 (Doxygen)

**里程碑：研究数据与文档完成** 🎯

**时间：** ~25-30小时

---

## ⏳ Phase 4: 完成交付 (Week 12)

### Week 12: 最终测试与提交 (12/24-12/31)

- [ ] 完整功能回归测试
- [ ] 用户验收测试
- [ ] Bug修复
- [ ] 代码格式化 (clang-format)
- [ ] 最终报告撰写
- [ ] 演示准备

**里程碑：项目交付** 🎯

**时间：** ~20-25小时

---

## 🎯 关键里程碑

| # | 里程碑 | 时间 | 状态 |
|---|--------|------|------|
| 0 | 项目初始化 | Week 0 | ✅ |
| 1 | Bitboard实现 | Week 1-2 | ✅ |
| 2 | 基础AI引擎 | Week 3-4 | ✅ |
| 3 | 优化特性 | Week 5-6 | 🔄 |
| 4 | UI与网络 | Week 7-8 | ⏳ |
| 5 | 研究分析 | Week 9-11 | ⏳ |
| 6 | 项目完成 | Week 12 | ⏳ |

---

## 📝 代码统计

### 当前状态 (Week 1)
```
src/core/Board.cpp          : ~140 lines
src/core/Board.hpp          : ~70 lines
src/core/Move.cpp           : ~60 lines
src/core/Move.hpp           : ~55 lines
src/ai/AIStrategy.hpp       : ~115 lines
src/research/benchmark/     : ~90 lines
src/main.cpp                : ~160 lines
----------------------------------------
总计                         : ~690 lines
```

### 预期最终规模
- **核心代码：** ~5,000 lines
- **AI引擎：** ~2,000 lines
- **UI代码：** ~1,500 lines
- **测试代码：** ~1,000 lines
- **总计：** ~10,000 lines

---

## 📚 学习进度

### 已掌握 ✅
- [x] Bitboard表示
- [x] C++20位操作函数
- [x] 基础棋盘状态管理
- [x] CMake项目结构
- [x] 现代C++项目组织
- [x] **Bitboard移动生成算法** ✨
- [x] **方向位移操作** ✨
- [x] **边界处理与掩码** ✨
- [x] **翻转棋子计算** ✨
- [x] **终局检测** ✨

### 已掌握 ✅（更新）
- [x] Bitboard表示 ✅
- [x] Zobrist哈希 ✅
- [x] 撤销移动机制 ✅
- [x] Minimax/Negamax算法 ✅
- [x] Alpha-Beta剪枝 ✅
- [x] 评估函数设计 ✅
- [x] 稳定性分析 ✅
- [x] 阶段化权重系统 ✅
- [x] 置换表优化 ✅
- [x] **迭代加深搜索** ✨ (Week 6)
- [x] **时间管理系统** ✨ (Week 6)
- [x] **Principal Variation Search** ✨ (Week 6)
- [x] **Aspiration Windows** ✨ (Week 6)
- [x] **Killer Move启发** ✨ (Week 6)

### 待学习 📅
- [ ] MCTS算法 (Week 9)
- [ ] 性能分析与优化 (Week 10-11)

---

## 🐛 已知问题

### 当前问题
- 无 - Week 1核心功能已全部完成并通过测试

### 技术债务
1. **待实现功能**
   - 撤销移动 (undo_move) - Week 2
   - Zobrist哈希 - Week 2
   - 评估函数 - Week 4
   - AI搜索引擎 - Week 3-6

2. **优化空间**
   - 合法移动生成可进一步优化（预计算表）
   - 翻转计算可使用SIMD指令
   - 内存管理可优化（对象池）

### 构建系统备注
- CMake有PowerShell输出问题
- 解决方案: 使用 `quick_compile.bat` 进行测试
- 直接g++编译正常工作

---

## ⏰ 时间追踪

### 已用时间
- Week 0 (设置): 7小时 ✅
- Week 1 (Bitboard): 22小时 ✅
- Week 2 (执行与优化): 18小时 ✅
- Week 3 (Minimax引擎): 22小时 ✅
- Week 4 (高级评估): 24小时 ✅
- Week 5 (置换表): 20小时 ✅
- Week 6 (高级搜索): 28小时 ✅
- **总计: 141小时 / 285小时 (49.5%)**

### 时间预算
| Phase | 预估时间 | 平均/周 |
|-------|---------|---------|
| Phase 1 (Week 1-6) | 110小时 | ~18h/周 |
| Phase 2 (Week 7-8) | 75小时 | ~37h/周 |
| Phase 3 (Week 9-11) | 70小时 | ~23h/周 |
| Phase 4 (Week 12) | 30小时 | ~30h/周 |
| **总计** | **285小时** | **~24h/周** |

**缓冲时间：** 30小时 (10%)

---

## 🎓 可交付成果

### 代码交付
- [x] ✅ 源代码仓库 (Git)
- [ ] 完整的构建系统
- [ ] 单元测试套件
- [ ] 性能基准测试
- [ ] 代码文档 (注释)

### 文档交付
- [x] ✅ README和入门指南
- [x] ✅ 项目结构文档
- [x] ✅ 学习笔记 (Week 1)
- [ ] 算法文档
- [ ] API文档
- [ ] 用户手册
- [ ] 性能分析报告
- [ ] 最终研究报告

### 功能交付
- [x] ✅ Bitboard棋盘表示
- [ ] 完整的游戏逻辑
- [ ] Minimax AI引擎
- [ ] MCTS AI引擎
- [ ] 图形用户界面
- [ ] 网络对战功能
- [ ] 性能基准框架

---

## 🚀 下一步行动 (Week 7准备)

### Week 6回顾 ✅ (2025年11月28日完成)
Week 6已完成，所有高级搜索优化技术实现并测试：
- ✅ 迭代加深搜索（Iterative Deepening）
- ✅ 基于游戏阶段的时间管理系统
- ✅ Principal Variation Search (PVS)
- ✅ 搜索窗口优化（Aspiration Windows）
- ✅ Killer Move启发（修正设计）
- ✅ 增强移动排序系统
- ✅ 完整单元测试（test_week6.cpp，8个测试）
- ✅ 性能基准测试（bench_week6.cpp，5个基准）

**关键成果：**
- 综合性能提升: 50%速度提升，40%节点减少
- PVS效率: 18%节点减少
- 时间控制: <5%开销
- 搜索速度: ~3.0 M nodes/sec（从1.91M提升）
- 学习笔记: week6_advanced_search_notes.md完整文档

### 立即任务 (Week 7准备) 🎯

**Week 7将开始UI开发，主要任务：**

1. **SFML基础学习**
   - 学习SFML图形库基础
   - 窗口创建和事件处理
   - 基本图形绘制

2. **棋盘渲染**
   - 8x8棋盘绘制
   - 棋子绘制（黑白圆盘）
   - 合法移动提示

3. **用户交互**
   - 鼠标点击处理
   - 移动选择与确认
   - 游戏状态显示

### 当前状态 (2025年11月28日)
- ✅ Week 1-6: Phase 1核心开发全部完成
  - ✅ Week 1-2: Bitboard基础系统
  - ✅ Week 3: Minimax AI引擎
  - ✅ Week 4: 高级评估函数
  - ✅ Week 5: 置换表优化
  - ✅ Week 6: 高级搜索技术
- 🔄 Week 7-8: Phase 2 UI与网络 - 准备开始
- 📝 所有文档已更新以反映真实进度

---

## 💡 经验教训

### Week 0-1见解
1. **构建系统：**
   - CMake功能强大但有平台特定问题
   - 备用构建方法 (batch脚本) 很有价值

2. **C++标准：**
   - C++20特性显著改进bitboard代码
   - 需确保编译器支持 (GCC 13.2+良好)

3. **项目组织：**
   - 清晰的目录结构有助于开发
   - 关注点分离 (core/ai/ui) 很重要

4. **文档：**
   - 边做边记录，不要事后补
   - 学习笔记帮助巩固理解

### Week 2见解 ✅
1. **性能优化：**
   - 提前完成基础功能使Week 2有更多优化时间
   - 微基准测试对发现性能瓶颈至关重要
   - 缓存友好的数据结构带来显著性能提升
   - 预分配内存(std::array)避免动态分配开销

2. **测试策略：**
   - 单元测试覆盖率直接影响代码质量
   - 45个测试用例确保了移动生成/执行的正确性
   - FForum标准测试位置是验证实现的金标准

3. **Zobrist哈希：**
   - 增量更新比重新计算快5-10倍
   - 正确处理Pass和Undo是关键难点
   - 哈希碰撞率需要持续监控

4. **时间管理：**
   - 18小时完成Week 2任务,严格按计划执行
   - 三天集中开发(周二-周四)效率高
   - 提前完成Week 1任务为Week 2留出缓冲时间

5. **代码质量：**
   - 零警告编译提高代码可靠性
   - inline关键函数带来5-10%性能提升
   - 代码重构应在功能完成后立即进行

---

## 📌 重要日期

| 日期 | 事件 | 优先级 | 状态 |
|------|------|--------|------|
| **2025-10-29** | CA1提案截止 | 🔴 高 | ⏳ 待完成 |
| **2025-10-22** | Week 3验收 (Minimax完成) | 🟡 中 | ✅ 已完成 |
| **2025-10-29** | Week 4验收 (高级评估) | 🟡 中 | ⏳ 待开始 |
| **2025-11-12** | Week 6验收 (高级搜索) | 🟡 中 | ⏳ 待开始 |
| **2025-11-26** | Week 8验收 (网络对战) | 🟡 中 | ⏳ 待开始 |
| **2025-12-17** | Week 11验收 (MCTS完成) | 🟡 中 | ⏳ 待开始 |
| **2025-12-31** | 最终提交 | 🔴 高 | ⏳ 待开始 |

---

## 📊 成功指标

### 技术指标
- [x] 项目无错误编译 ✅ (Week 1)
- [x] 遵循C++20最佳实践 ✅ (Week 1-3)
- [x] 单元测试覆盖率 > 80% ✅ (Week 1-3: 8个测试100%通过)
- [x] AI可进行合法游戏 ✅ (Week 3: Minimax引擎完成)
- [x] Alpha-Beta剪枝实现 ✅ (Week 3: 3.07x加速)
- [x] 置换表优化 ✅ (Week 5)
- [ ] MCTS引擎实现 ⏳ (Week 9)
- [ ] 基准框架运行 ⏳ (Week 10+)

### 学习成果
- [x] 理解bitboard表示 ✅ (Week 1)
- [x] C++20特性熟练度 ✅ (Week 1-3)
- [x] Minimax/Negamax搜索算法 ✅ (Week 3)
- [x] Alpha-Beta剪枝理论 ✅ (Week 3)
- [x] 评估函数设计 ✅ (Week 3)
- [ ] 高级搜索技术 ⏳ (Week 4-6)
- [ ] MCTS算法 ⏳ (Week 9)
- [ ] 性能优化 ⏳ (Week 10-11)
- [ ] 研究方法 ⏳ (Week 9-12)

---

## 🔗 相关资源

### 项目文档
- [README.md](../README.md) - 项目概览
- [TECHNICAL_ISSUES_SOLUTIONS.md](TECHNICAL_ISSUES_SOLUTIONS.md) - 技术问题手册
- [github/GETTING_STARTED.md](github/GETTING_STARTED.md) - 入门指南
- [github/BUILD_AND_RUN.md](github/BUILD_AND_RUN.md) - 构建说明

### 学习笔记
- [docs/week1_bitboard_notes.md](../docs/week1_bitboard_notes.md) - Week 1笔记 (495行)
- [docs/week2_testing_optimization_notes.md](../docs/week2_testing_optimization_notes.md) - Week 2笔记 (438行)
- [docs/week3_minimax_ai_notes.md](../docs/week3_minimax_ai_notes.md) - Week 3笔记 (674行)
- [docs/week4_advanced_evaluation_notes.md](../docs/week4_advanced_evaluation_notes.md) - Week 4笔记 (436行)
- [docs/week5_transposition_notes.md](../docs/week5_transposition_notes.md) - Week 5笔记
- [docs/week6_advanced_search_notes.md](../docs/week6_advanced_search_notes.md) - Week 6笔记 ✅

### 参考资料
- Egaroucid - Bitboard实现参考
- AlphaZero_Gomoku - MCTS参考
- Chess Programming Wiki - Bitboards理论

---

**当前焦点：** Week 7 SFML UI开发 🔄  
**下一里程碑：** 图形界面 + 用户交互  
**整体状态：** ✅ Phase 1完成（Week 1-6完成，52%整体进度）

---

*本文档每周更新以跟踪项目进度*

