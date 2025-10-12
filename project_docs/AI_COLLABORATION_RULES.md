# 🤖 AI协作规范

> **重要：新AI会话必读！**  
> 本文档定义了与此项目交互时必须遵循的所有规则和最佳实践。

**项目：** Reversi AI - COMP390 Honours Project  
**最后更新：** 2025年10月12日  
**版本：** 2.0 - 新增用户协作偏好

---

## 🚨 核心原则（必须遵守）

### 0️⃣ 用户协作偏好（最高优先级）⭐

> **这些是用户的个人工作习惯，必须严格遵守！**

#### 💬 **沟通与展示方式**

1. **优先在聊天界面展示，而非创建文档**
   - ✅ 总结、报告、统计 → **直接在聊天中展示**
   - ✅ 使用格式化 Markdown（表格、emoji、分区块）
   - ✅ 让信息清晰、美观、易读
   - ❌ **禁止**创建 `SUMMARY.md`、`REPORT.md`、`STATUS.md` 等临时报告文件
   
   **示例：**
   ```markdown
   ✅ 正确：在聊天中展示
   ## 📊 Week 1 完成总结
   - ✅ 任务1: 完成
   - ✅ 任务2: 完成
   ...
   
   ❌ 错误：创建文件
   # 创建 WEEK1_SUMMARY.md
   ```

2. **展示风格要求**
   - ✅ 使用 emoji 增强可读性（✅❌📊🎯⚠️等）
   - ✅ 使用表格整理数据
   - ✅ 使用分隔线和标题层次
   - ✅ 突出重点信息（加粗、引用块）
   - ✅ 提供清晰的统计数字

#### 🎯 **工作方式偏好**

1. **主动执行，而非仅建议**
   - ✅ 直接执行任务（"我已经..."）
   - ✅ 遇到问题立即修复
   - ✅ 完整完成任务，不半途而废
   - ❌ 避免过多的"我建议..."、"你可以..."

2. **高效批量操作**
   - ✅ 并行读取/修改多个文件
   - ✅ 一次性完成相关的所有更新
   - ✅ 避免多次来回询问

3. **自动清理**
   - ✅ 任务完成后自动删除临时文件
   - ✅ 清理测试代码和调试输出
   - ✅ 保持项目整洁

#### 🔧 **Git 操作习惯**

1. **提交信息规范**
   - ✅ 使用详细的多行提交信息
   - ✅ 包含修改统计（文件数、行数）
   - ✅ 列出所有主要变更
   - ✅ 使用常规提交格式（feat:, fix:, docs:, refactor:等）
   
   **示例：**
   ```bash
   docs: 全面更新项目文档至Week 1完成状态
   
   - 更新所有文档的日期至2025-10-12
   - PROJECT_PLAN.md: Week 1标记为完成，更新进度20%
   - week1_bitboard_notes.md: 标记完成状态
   - 统计: 11个文件修改，109行新增，202行删除
   ```

2. **Git 工作流**
   - ✅ 提交前展示所有修改的文件（`git diff --name-only`）
   - ✅ 提交后展示提交历史（`git log --oneline -5`）
   - ✅ **不要自动推送** - 让用户决定何时 `git push`
   - ✅ 提供推送建议，但不执行

#### 📝 **文档更新习惯**

1. **同步更新原则**
   - ✅ 更新一个文档时，同步更新所有相关文档
   - ✅ 统一所有文档的日期和版本号
   - ✅ 保持进度、时间、状态的一致性
   - ✅ 更新后展示完整的变更总结

2. **文档一致性检查**
   - ✅ 确保 PROJECT_PLAN.md、README.md、github/README.md 进度一致
   - ✅ 确保所有日期统一
   - ✅ 确保时间统计准确
   - ✅ 展示"更新前 vs 更新后"对比

#### 🐛 **问题解决方式**

1. **遇到错误立即修复**
   - ✅ 发现 Windows/Linux 命令差异 → 立即适配
   - ✅ 发现路径问题 → 立即修正
   - ✅ 发现逻辑错误 → 立即重试正确方法
   - ❌ 不要让错误阻塞工作流程

2. **问题记录**
   - ✅ 重要技术问题记录到 TECHNICAL_ISSUES_SOLUTIONS.md
   - ✅ 包含：现象、原因、解决方案、经验教训
   - ✅ 小问题直接修复，不必记录

#### 📊 **任务完成展示**

完成任务后，必须提供清晰的总结：

```markdown
## ✅ 任务完成总结

### 📊 修改统计
- 📝 X个文件修改
- ➕ Y行新增
- ➖ Z行删除

### 🎯 主要变更
1. 文件A: 变更内容
2. 文件B: 变更内容

### 🔍 验证结果
- ✅ 测试通过
- ✅ 文档同步
- ✅ 无遗留问题

### 🚀 下一步建议
- 建议1
- 建议2
```

---

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

✅ **正确做法（按优先级）：**
```
1. 【最优】直接在聊天中展示格式化的总结（用户偏好！）
   - 使用 Markdown 格式
   - 包含 emoji、表格、分区块
   - 清晰美观，易于阅读
   
2. 【次选】整合到 PROJECT_PLAN.md 的相应部分
   - 更新进度追踪
   - 追加到相应周的记录
   
3. 【最后】如用户明确要求文件，才创建在 project_docs/ 中
```

**示例：**
```markdown
用户："总结一下 Week 1 的完成情况"

✅ 正确：在聊天中展示
## 📊 Week 1 完成总结
- ✅ Bitboard实现: 100%
- ✅ 移动生成: 100%
- ✅ 性能测试: 11M ops/sec
...

❌ 错误：创建 WEEK1_SUMMARY.md
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

### 场景6: Windows 环境命令适配 ⭐

**背景：** 用户使用 Windows PowerShell

❌ **错误做法：**
```bash
# 使用 Linux 命令（会失败）
cat file.txt
head -n 10 file.txt
ls -la
grep "pattern" file.txt
```

✅ **正确做法：**
```powershell
# 使用 PowerShell 兼容命令
Get-Content file.txt
Get-Content file.txt -Head 10
Get-ChildItem
Select-String "pattern" file.txt

# 或使用 Git Bash 命令（如果可用）
# 或使用跨平台的工具命令
```

**重要提示：**
- ✅ 遇到命令错误立即切换到 Windows 兼容命令
- ✅ 使用 `copy` 而非 `cp`
- ✅ 使用 `dir` 或 `Get-ChildItem` 而非 `ls`
- ✅ 路径使用反斜杠 `\` 或正斜杠 `/`（都支持）
- ✅ 不要使用 `head`、`tail` 等 Unix 专用命令

---

## 🎯 最佳实践

### ✨ 文档管理最佳实践

1. **"聊天优先"原则** ⭐ **用户偏好**
   - 总结、报告 → 优先在聊天中展示
   - 使用格式化 Markdown，清晰美观
   - 避免创建临时文档

2. **"问问再做"原则**
   - 创建新文档前，先问用户是否可以整合
   - 不确定放哪里时，询问用户

3. **"一次一地"原则**  
   - 同一信息只保存在一个地方
   - 避免信息分散在多个文档

4. **"即时清理"原则**
   - 临时文件用完立即删除
   - 过时文档及时清理或更新

5. **"结构稳定"原则**
   - 不轻易改变文档结构
   - 改变前先与用户确认

6. **"主动执行"原则** ⭐ **用户偏好**
   - 直接执行而非仅建议
   - 遇到问题立即修复
   - 完整完成任务

7. **"批量高效"原则** ⭐ **用户偏好**
   - 并行操作多个文件
   - 一次性完成相关更新
   - 减少来回询问

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
- [ ] **已阅读"用户协作偏好"部分** ⭐ 最重要
- [ ] 了解文档结构（查看 project_docs/README.md）
- [ ] 知道当前任务（查看 PROJECT_PLAN.md）

**在工作过程中：**
- [ ] **优先在聊天中展示结果**，而非创建文档 ⭐
- [ ] **主动执行任务**，而非仅建议 ⭐
- [ ] **批量并行操作**，提高效率 ⭐
- [ ] 需要创建文档时先询问用户
- [ ] 优先整合到现有文档
- [ ] 文档放在 project_docs/ 中
- [ ] 遵循文档管理规则

**在结束工作前：**
- [ ] **在聊天中展示完整的任务总结** ⭐
- [ ] 更新了相关进度（同步所有相关文档）
- [ ] 清理了临时文件
- [ ] 没有在根目录创建新文档
- [ ] 保持了项目整洁
- [ ] 提供了 Git 提交信息和下一步建议

---

## 🏆 总结：核心规则速查

### 0️⃣ 用户偏好（最高优先级）⭐
```
总结报告 → 聊天中展示（格式化 Markdown）
工作方式 → 主动执行，批量操作，立即修复
Git提交 → 详细信息，展示历史，不自动推送
文档更新 → 同步所有相关文档，保持一致性
任务完成 → 聊天中展示完整总结（统计+变更+验证+建议）
```

### 1️⃣ 位置规则
```
所有文档 → project_docs/
GitHub文档 → project_docs/github/
根目录 → 只有 README.md（从github/复制）
```

### 2️⃣ 创建规则
```
总结报告 → 聊天中展示（不创建文件）⭐
新内容 → 优先整合到现有文档
必须新建 → 先询问用户
临时文件 → 用完立即删除
```

### 3️⃣ 维护规则
```
任务完成 → 更新 PROJECT_PLAN.md（同步所有相关文档）
遇到问题 → 记录 TECHNICAL_ISSUES_SOLUTIONS.md
结构变化 → 更新 README.md 导航
```

### 4️⃣ 工作风格
```
沟通方式 → 主动执行，不仅建议 ⭐
操作效率 → 并行批量，减少来回 ⭐
问题处理 → 立即修复，不阻塞流程 ⭐
展示风格 → emoji + 表格 + 分区块 ⭐
```

---

## 🎯 新AI会话快速上手

**第一步：** 阅读 **"0️⃣ 用户协作偏好"** 部分 ⭐ **最重要！**

**第二步：** 了解项目结构
- 查看 `project_docs/README.md`
- 查看 `project_docs/PROJECT_PLAN.md`

**第三步：** 记住核心原则
1. 总结报告 → **聊天中展示**，不创建文件
2. 工作方式 → **主动执行**，批量操作
3. 文档更新 → **同步所有**相关文档
4. Git 操作 → 详细提交信息，**不自动推送**

**第四步：** 开始工作！

---

**版本：** 2.0 - 新增用户协作偏好  
**创建日期：** 2025年10月8日  
**最后更新：** 2025年10月12日  
**维护者：** 田祺星 (Tianqixing)  
**状态：** ✅ 活跃使用

---

*"清晰的规则带来高效的协作"* 🤖✨

**请新AI会话将此文档作为工作指南，严格遵守以上所有规则！**

