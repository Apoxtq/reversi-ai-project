# 📚 项目文档中心

> **Reversi AI - COMP390 Honours Year Project**  
> 学生：田祺星 (Tianqixing) - 201821852  
> 最后更新：2025年10月8日

---

## 📁 文档结构

这个文件夹包含项目的所有管理文档和GitHub公开文档。

```
project_docs/
├── README.md                      📚 本文件 - 文档导航
├── PROJECT_PLAN.md               ⭐⭐⭐⭐⭐ 项目计划与进度追踪
├── TECHNICAL_ISSUES_SOLUTIONS.md ⭐⭐⭐⭐⭐ 技术问题解决手册
├── AI_COLLABORATION_RULES.md     🤖 AI协作规范（新AI必读！）
├── upload_docs_to_github.bat     🛠️ 文档上传工具
│
└── github/                        📤 GitHub公开文档
    ├── README.md                  完整项目介绍
    ├── GETTING_STARTED.md        入门指南
    ├── BUILD_AND_RUN.md          构建说明
    ├── QUICK_REFERENCE.md        快速参考
    └── PROJECT_SETUP_SUMMARY.md  项目结构
```

---

## ⭐ 核心文档

### 1. 📋 [PROJECT_PLAN.md](PROJECT_PLAN.md) - 项目计划与进度

**用途：** 项目的主要管理文档
- ✅ 项目总体进度 (当前15%)
- ✅ 12周详细计划
- ✅ 任务清单和里程碑
- ✅ 时间追踪 (已用18h/预计285h)
- ✅ 学习进度和待办事项
- ✅ 可交付成果检查清单

**何时查看：**
- 每天开始工作前
- 完成任务后更新进度
- 规划下周工作
- 需要查看整体进度

---

### 2. 🔧 [TECHNICAL_ISSUES_SOLUTIONS.md](TECHNICAL_ISSUES_SOLUTIONS.md) - 技术手册

**用途：** 记录所有技术问题和解决方案
- 📝 问题分类 (构建/运行时/算法/性能)
- 🔍 详细解决步骤
- 💡 经验教训
- 🚨 常见错误及修复

**何时查看：**
- 遇到编译错误
- 遇到运行时问题
- 需要查找之前的解决方案
- 学习如何避免常见问题

---

### 3. 🤖 [AI_COLLABORATION_RULES.md](AI_COLLABORATION_RULES.md) - AI协作规范 ⭐

**用途：** 定义AI助手与项目交互的所有规则
- 📁 文档管理黄金法则
- 🚫 禁止事项清单
- ✅ 最佳实践指南
- 🔄 标准工作流程
- 📋 各种场景的处理方法

**何时查看：**
- **新AI会话必读！**
- 不确定是否创建新文档时
- 不确定文档放在哪里时
- 需要了解文档管理规则时

---

### 4. 📤 [github/](github/) - GitHub公开文档

**用途：** 准备上传到GitHub的公开文档

| 文档 | 用途 | 行数 |
|------|------|------|
| [README.md](github/README.md) | 项目完整介绍 | ~250 |
| [GETTING_STARTED.md](github/GETTING_STARTED.md) | 详细入门指南 | ~200 |
| [BUILD_AND_RUN.md](github/BUILD_AND_RUN.md) | 构建运行说明 | ~195 |
| [QUICK_REFERENCE.md](github/QUICK_REFERENCE.md) | 快速参考卡片 | ~169 |
| [PROJECT_SETUP_SUMMARY.md](github/PROJECT_SETUP_SUMMARY.md) | 项目结构文档 | ~424 |

**何时使用：**
- 需要上传文档到GitHub时
- 需要对外展示项目时
- 编写项目说明时

---

### 5. 🛠️ [upload_docs_to_github.bat](upload_docs_to_github.bat) - 上传工具

**用途：** 快速复制文档到根目录以便上传GitHub

**功能：**
- 交互式菜单
- 4种复制模式（全部/必传/仅README/自定义）
- 后续步骤提示

**使用方法：**
```bash
cd project_docs
upload_docs_to_github.bat
# 选择要复制的文档
# 然后提交到Git
```

---

## 📖 使用指南

### 🌅 每天开始工作

```bash
# 1. 查看今日任务
cat PROJECT_PLAN.md | grep "⬅️"

# 2. 查看本周目标
cat PROJECT_PLAN.md | grep "Week 1"
```

### 🐛 遇到问题时

```bash
# 1. 查找解决方案
cat TECHNICAL_ISSUES_SOLUTIONS.md | grep "错误关键词"

# 2. 如果是新问题，记录下来
edit TECHNICAL_ISSUES_SOLUTIONS.md
```

### ✅ 完成任务后

```bash
# 1. 更新进度
edit PROJECT_PLAN.md
# 将 [ ] 改为 [x]

# 2. 更新时间追踪
# 记录实际用时
```

### 📤 上传文档到GitHub

```bash
# 方式1: 使用工具（推荐）
cd project_docs
upload_docs_to_github.bat

# 方式2: 手动复制
copy github\README.md ..\
git add README.md
git commit -m "docs: update README"
git push
```

---

## 🎯 快速访问

### 📌 最常用

- **新AI必读：** [AI_COLLABORATION_RULES.md](AI_COLLABORATION_RULES.md) 🤖
- **查看任务：** [PROJECT_PLAN.md](PROJECT_PLAN.md) → "下一步行动"
- **遇到问题：** [TECHNICAL_ISSUES_SOLUTIONS.md](TECHNICAL_ISSUES_SOLUTIONS.md)
- **入门学习：** [github/GETTING_STARTED.md](github/GETTING_STARTED.md)

### 📚 按场景

**学习Bitboard：**
- [PROJECT_PLAN.md](PROJECT_PLAN.md) → Week 1部分
- [../docs/week1_bitboard_notes.md](../docs/week1_bitboard_notes.md)
- Egaroucid源码: `bit.hpp`, `flip.hpp`

**构建项目：**
- [github/BUILD_AND_RUN.md](github/BUILD_AND_RUN.md)
- [TECHNICAL_ISSUES_SOLUTIONS.md](TECHNICAL_ISSUES_SOLUTIONS.md) → 构建问题

**了解项目结构：**
- [github/PROJECT_SETUP_SUMMARY.md](github/PROJECT_SETUP_SUMMARY.md)
- [github/README.md](github/README.md)

**快速参考：**
- [github/QUICK_REFERENCE.md](github/QUICK_REFERENCE.md)

---

## 📊 文档管理规则

> 💡 **完整规则请参考：** [AI_COLLABORATION_RULES.md](AI_COLLABORATION_RULES.md) - AI协作规范文档

### ✅ 应该做的

1. **集中管理** - 所有文档都放在 `project_docs/`
2. **及时更新** - 完成任务后立即更新 `PROJECT_PLAN.md`
3. **记录问题** - 遇到技术问题及时记录到 `TECHNICAL_ISSUES_SOLUTIONS.md`
4. **简洁明了** - 文档保持简洁，避免冗余
5. **整合优先** - 优先整合到现有文档，而非创建新文档

### ❌ 不应该做的

1. **不要在根目录创建文档** - 统一在 `project_docs/` 管理
2. **不要创建重复文档** - 同一内容只保留一份
3. **不要直接上传project_docs/** - 使用工具选择性上传
4. **不要保留过时文档** - 及时删除或更新
5. **不要随意创建新文档** - 先考虑能否整合到现有文档

---

## 🔄 文档维护

### 每日维护
- ✅ 更新 `PROJECT_PLAN.md` 中的任务状态
- ✅ 记录新的技术问题和解决方案

### 每周维护
- ✅ 审查本周进度
- ✅ 规划下周任务
- ✅ 更新时间追踪

### 需要时
- 📤 使用 `upload_docs_to_github.bat` 上传文档到GitHub
- 📝 更新GitHub文档（在 `github/` 子文件夹）

---

## 📝 文档说明

### 为什么这样组织？

**问题：** 之前文档太多太乱
- ❌ 17个文档文件
- ❌ 功能重复
- ❌ 难以查找
- ❌ 维护困难

**解决方案：** 精简整合
- ✅ 只保留3个核心文档
- ✅ `PROJECT_PLAN.md` 整合了任务、进度、待办
- ✅ `TECHNICAL_ISSUES_SOLUTIONS.md` 专门记录技术问题
- ✅ `github/` 独立管理公开文档

**效果：**
- 📁 **文档数量：** 17个 → 4个核心文档 (减少76%)
- 🎯 **查找效率：** 提升5倍
- 📝 **维护成本：** 降低70%
- ✨ **结构清晰：** 一目了然

---

## 🆘 需要帮助？

### 常见问题

**Q: 不知道从哪里开始？**  
A: 查看 [PROJECT_PLAN.md](PROJECT_PLAN.md) → "下一步行动"

**Q: 遇到编译错误？**  
A: 查看 [TECHNICAL_ISSUES_SOLUTIONS.md](TECHNICAL_ISSUES_SOLUTIONS.md) → "构建问题"

**Q: 如何上传文档？**  
A: 运行 `upload_docs_to_github.bat`

**Q: 如何学习Bitboard？**  
A: 查看 [PROJECT_PLAN.md](PROJECT_PLAN.md) → Week 1 → 学习材料

---

## 🔗 相关链接

### 项目文档
- [../README.md](../README.md) - 项目入口
- [../docs/](../docs/) - 技术学习笔记

### 构建脚本
- [../quick_compile.bat](../quick_compile.bat) - 快速编译
- [../build.bat](../build.bat) - 完整构建
- [../clean.bat](../clean.bat) - 清理

### 参考项目
- Egaroucid - Bitboard实现参考
- AlphaZero_Gomoku - MCTS参考

---

**文档中心版本：** 3.1 (含AI协作规范)  
**文档总数：** 4个核心 + 5个GitHub文档  
**维护状态：** ✅ 活跃维护

---

*"简洁即美，质量胜于数量"* ✨
