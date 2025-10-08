# 🤖 AI协作规范

> **重要：新AI会话必读！**  
> 本文档定义了与此项目交互时必须遵循的所有规则和最佳实践。

---

## 🚨 核心原则（必须遵守）

### 1️⃣ 文档管理的黄金法则

#### ✅ **必须做的**

1. **所有文档统一放在 `project_docs/` 文件夹**
   - ❌ **永远不要**在根目录创建新的 `.md` 文档
   - ✅ 所有项目管理文档都应在 `project_docs/` 中
   - ✅ GitHub公开文档放在 `project_docs/github/` 中

2. **整合而非创建新文档**
   - ❌ **永远不要**随意创建新的文档文件
   - ✅ 优先向现有文档中**追加内容**
   - ✅ 如需新文档，先询问用户是否可以整合到现有文档

3. **保持项目整洁**
   - ❌ **永远不要**创建临时文档而不清理
   - ❌ **永远不要**创建冗余的总结报告
   - ✅ 定期清理不需要的文件

#### ❌ **禁止做的**

```
🚫 在根目录创建 SUMMARY.md
🚫 在根目录创建 REPORT.md  
🚫 在根目录创建 STATUS.md
🚫 创建重复功能的文档
🚫 创建临时性的总结文档
```

---

### 2️⃣ 文档结构规范

#### 📂 当前文档结构（必须遵守）

```
project_docs/               ← 所有文档的家
├── README.md              📚 文档导航（更新此文档）
├── PROJECT_PLAN.md        📋 项目计划和进度追踪
├── TECHNICAL_ISSUES_SOLUTIONS.md  🔧 技术问题手册
├── AI_COLLABORATION_RULES.md      🤖 本文档
├── upload_docs_to_github.bat      🛠️ 文档上传工具
│
└── github/                📤 GitHub公开文档
    ├── README.md          项目主页
    ├── GETTING_STARTED.md 入门指南
    ├── BUILD_AND_RUN.md   构建说明
    ├── QUICK_REFERENCE.md 快速参考
    └── PROJECT_SETUP_SUMMARY.md 项目结构
```

#### 📝 各文档的用途和更新规则

| 文档 | 用途 | 何时更新 | 更新方式 |
|------|------|----------|----------|
| **PROJECT_PLAN.md** | 项目计划、任务清单、进度追踪 | 任务完成、新计划 | 追加/更新进度 |
| **TECHNICAL_ISSUES_SOLUTIONS.md** | 技术问题记录和解决方案 | 遇到新问题 | 追加新问题 |
| **README.md** | 文档导航和使用指南 | 文档结构变化 | 更新导航 |
| **github/** | 对外公开文档 | 需要更新GitHub时 | 直接编辑 |

---

### 3️⃣ 工作流程规范

#### 🔄 标准工作流程

```mermaid
开始任务
  ↓
查看 PROJECT_PLAN.md 确认当前任务
  ↓
执行任务
  ↓
遇到问题？
  ├── 是 → 记录到 TECHNICAL_ISSUES_SOLUTIONS.md
  └── 否 → 继续
  ↓
完成任务 → 更新 PROJECT_PLAN.md 进度
  ↓
需要文档？
  ├── 是 → 整合到现有文档（询问用户）
  └── 否 → 结束
```

#### ✅ 任务完成后的检查清单

- [ ] 更新了 `PROJECT_PLAN.md` 中的任务状态？
- [ ] 记录了技术问题到 `TECHNICAL_ISSUES_SOLUTIONS.md`？
- [ ] 清理了所有临时文件？
- [ ] 没有在根目录创建新文档？

---

## 📋 具体场景的处理规则

### 场景1: 用户要求总结或报告

❌ **错误做法：**
```
创建 SUMMARY.md
创建 COMPLETION_REPORT.md
创建 STATUS_REPORT.md
```

✅ **正确做法：**
```
1. 询问："是否可以将总结追加到 PROJECT_PLAN.md 中？"
2. 或者在聊天中直接回复总结，不创建文件
3. 如用户坚持要文件，创建在 project_docs/ 中
```

---

### 场景2: 遇到技术问题

❌ **错误做法：**
```
创建 ISSUE_FIX.md
创建 DEBUG_LOG.md
```

✅ **正确做法：**
```
1. 打开 TECHNICAL_ISSUES_SOLUTIONS.md
2. 在相应分类下追加问题记录
3. 格式：
   #### 问题X: [问题标题]
   **现象：** ...
   **原因：** ...
   **解决方案：** ...
   **经验教训：** ...
```

---

### 场景3: 项目进度更新

❌ **错误做法：**
```
创建 PROGRESS_REPORT.md
创建 WEEK1_SUMMARY.md
```

✅ **正确做法：**
```
1. 打开 PROJECT_PLAN.md
2. 更新 "项目总体进度" 部分
3. 更新相应周的任务状态 [ ] → [x]
4. 更新 "时间追踪" 部分
```

---

### 场景4: 需要创建新文档

❌ **错误做法：**
```
直接在根目录创建
直接在 project_docs/ 创建
```

✅ **正确做法：**
```
1. 先问："这个内容是否可以整合到以下现有文档？
   - PROJECT_PLAN.md (计划/进度相关)
   - TECHNICAL_ISSUES_SOLUTIONS.md (技术问题)
   - README.md (导航/说明)
   - github/ 文件夹 (公开文档)"

2. 如果必须创建新文档：
   - 放在 project_docs/ 中
   - 更新 project_docs/README.md 的导航
   - 说明创建理由
```

---

### 场景5: GitHub文档更新

❌ **错误做法：**
```
直接修改根目录的 README.md
创建新的公开文档在根目录
```

✅ **正确做法：**
```
1. 编辑 project_docs/github/README.md
2. 使用 upload_docs_to_github.bat 工具复制到根目录
3. 或手动复制：
   copy project_docs\github\README.md README.md
```

---

## 🎯 最佳实践

### ✨ 文档管理最佳实践

1. **"问问再做"原则**
   - 创建新文档前，先问用户是否可以整合
   - 不确定放哪里时，询问用户

2. **"一次一地"原则**  
   - 同一信息只保存在一个地方
   - 避免信息分散在多个文档

3. **"即时清理"原则**
   - 临时文件用完立即删除
   - 过时文档及时清理或更新

4. **"结构稳定"原则**
   - 不轻易改变文档结构
   - 改变前先与用户确认

---

### 📊 文档精简原则

**背景：** 之前项目有17个文档，90%是冗余的

**目标：** 保持最少文档数量，最高信息密度

**执行：**
- ✅ 核心文档只保留3个（PLAN + TECH + README）
- ✅ GitHub文档独立管理在 `github/` 子文件夹
- ✅ 新内容优先整合，实在不行才新建
- ✅ 定期审查，删除过时文档

---

## 🔧 工具和命令

### 📁 文档位置检查

```bash
# 检查根目录文档（应该只有README.md）
ls *.md

# 检查 project_docs（核心文档在这里）
ls project_docs/*.md
```

### 📤 GitHub文档上传

```bash
# 方法1：使用工具（推荐）
cd project_docs
upload_docs_to_github.bat

# 方法2：手动复制
copy project_docs\github\README.md README.md
```

### 🧹 清理命令

```bash
# 清理构建文件
clean.bat

# 查找临时文档（不应该有）
ls *TEMP*.md *TMP*.md *_OLD.md
```

---

## 📖 给新AI会话的提示

### 🆕 初次接触此项目？

**第一步：** 阅读本文档（你正在做！）

**第二步：** 了解文档结构
```bash
# 查看文档导航
cat project_docs/README.md

# 查看当前任务
cat project_docs/PROJECT_PLAN.md | grep "下一步行动"
```

**第三步：** 遵循规则
- ✅ 文档放 `project_docs/`
- ✅ 优先整合而非新建
- ✅ 保持结构稳定
- ✅ 及时清理临时文件

---

### 🤔 不确定时的处理方式

**如果不确定：**
1. ❓ 不确定是否创建文档 → **先问用户**
2. ❓ 不确定放哪个文档 → **先问用户**
3. ❓ 不确定是否删除 → **先问用户**
4. ❓ 不确定文档结构 → **查看 project_docs/README.md**

**默认原则：**
- 🛡️ **保守优先** - 不确定就不做
- 💬 **沟通优先** - 先问用户再行动
- 📋 **现有优先** - 优先使用现有文档

---

## ⚠️ 常见错误及避免方法

### ❌ 错误1: 在根目录创建文档

```bash
# 错误示例
echo "Summary" > SUMMARY.md  # ❌ 在根目录

# 正确做法
echo "Summary" > project_docs/SUMMARY.md  # ✅ 在 project_docs
# 或者整合到现有文档
```

### ❌ 错误2: 创建冗余文档

```bash
# 错误示例
- TODO.md (任务清单)          # ❌ 已有 PROJECT_PLAN.md
- ISSUES.md (问题记录)        # ❌ 已有 TECHNICAL_ISSUES_SOLUTIONS.md
- SETUP.md (设置说明)         # ❌ 已有 github/GETTING_STARTED.md

# 正确做法：使用现有文档
```

### ❌ 错误3: 不清理临时文件

```bash
# 错误示例
temp_notes.md      # ❌ 临时笔记忘记删除
debug_log.txt      # ❌ 调试日志未清理

# 正确做法：用完立即删除
rm temp_notes.md debug_log.txt
```

---

## 📈 成功案例

### ✅ 文档精简成功案例

**之前：**
```
根目录/
├── TODO.md
├── CHECKLIST.md
├── GETTING_STARTED.md
├── BUILD_AND_RUN.md
├── PROJECT_STATUS.md
├── SETUP_COMPLETE.md
├── FINAL_SUMMARY.md
├── QUICK_REFERENCE.md
├── COMPLETION_REPORT.md
├── FILE_INDEX.md
├── WELCOME.md
├── DOCUMENTATION_REORGANIZATION.md
└── ... 更多冗余文档

总计：17个文档，功能重复，难以维护
```

**现在：**
```
project_docs/
├── README.md                        📚 导航
├── PROJECT_PLAN.md                  📋 计划+进度+任务
├── TECHNICAL_ISSUES_SOLUTIONS.md    🔧 技术问题
├── AI_COLLABORATION_RULES.md        🤖 协作规范
└── github/                          📤 公开文档
    ├── README.md
    ├── GETTING_STARTED.md
    ├── BUILD_AND_RUN.md
    ├── QUICK_REFERENCE.md
    └── PROJECT_SETUP_SUMMARY.md

总计：4个核心文档 + 5个GitHub文档
效果：信息密度提升5倍，维护成本降低70%
```

---

## 🎓 学习资源

### 📚 如何快速了解项目

1. **项目概述：** `project_docs/github/README.md`
2. **入门指南：** `project_docs/github/GETTING_STARTED.md`
3. **项目结构：** `project_docs/github/PROJECT_SETUP_SUMMARY.md`
4. **当前任务：** `project_docs/PROJECT_PLAN.md` → 下一步行动
5. **技术问题：** `project_docs/TECHNICAL_ISSUES_SOLUTIONS.md`

### 🔗 重要链接

- **文档中心：** `project_docs/README.md`
- **项目根目录：** `../README.md`
- **技术笔记：** `../docs/`
- **源代码：** `../src/`

---

## 📞 需要帮助？

### 常见问题速查

| 问题 | 答案 |
|------|------|
| 要创建新文档？ | 先问用户是否可以整合到现有文档 |
| 不知道放哪里？ | 优先放 `project_docs/`，GitHub文档放 `project_docs/github/` |
| 要总结报告？ | 在聊天中回复或整合到 `PROJECT_PLAN.md` |
| 遇到技术问题？ | 记录到 `TECHNICAL_ISSUES_SOLUTIONS.md` |
| 要更新进度？ | 更新 `PROJECT_PLAN.md` |
| 要更新GitHub？ | 编辑 `project_docs/github/` 中的文件 |

---

## ✅ AI会话检查清单

**在开始工作前：**
- [ ] 已阅读本文档（AI_COLLABORATION_RULES.md）
- [ ] 了解文档结构（查看 project_docs/README.md）
- [ ] 知道当前任务（查看 PROJECT_PLAN.md）

**在工作过程中：**
- [ ] 需要创建文档时先询问用户
- [ ] 优先整合到现有文档
- [ ] 文档放在 project_docs/ 中
- [ ] 遵循文档管理规则

**在结束工作前：**
- [ ] 更新了相关进度
- [ ] 清理了临时文件
- [ ] 没有在根目录创建新文档
- [ ] 保持了项目整洁

---

## 🏆 总结：三大核心规则

### 1️⃣ 位置规则
```
所有文档 → project_docs/
GitHub文档 → project_docs/github/
根目录 → 只有 README.md（从github/复制）
```

### 2️⃣ 创建规则
```
新内容 → 优先整合到现有文档
必须新建 → 先询问用户
临时文件 → 用完立即删除
```

### 3️⃣ 维护规则
```
任务完成 → 更新 PROJECT_PLAN.md
遇到问题 → 记录 TECHNICAL_ISSUES_SOLUTIONS.md
结构变化 → 更新 README.md 导航
```

---

**版本：** 1.0  
**创建日期：** 2025年10月8日  
**维护者：** 田祺星 (Tianqixing)  
**状态：** ✅ 活跃使用

---

*"清晰的规则带来高效的协作"* 🤖✨

**请新AI会话将此文档作为工作指南，严格遵守以上所有规则！**

