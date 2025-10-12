# 🔄 Git工作流程指南

**项目：** Reversi AI  
**仓库：** https://github.com/Apoxtq/reversi-ai-project  
**分支策略：** 单主分支（main）

---

## 📋 目录

1. [快速命令](#快速命令)
2. [日常工作流](#日常工作流)
3. [提交规范](#提交规范)
4. [分支管理](#分支管理)
5. [常见问题](#常见问题)

---

## 🚀 快速命令

### 检查状态
```bash
git status
```

### 查看修改
```bash
git diff                    # 查看未暂存的修改
git diff --staged          # 查看已暂存的修改
```

### 添加文件
```bash
git add .                  # 添加所有修改
git add <file>             # 添加特定文件
git add -A                 # 添加所有（包括删除）
```

### 提交更改
```bash
git commit -m "feat: add feature description"
```

### 推送到GitHub
```bash
git push origin main
# 或使用脚本
./push_to_github.bat
```

### 从GitHub拉取
```bash
git pull origin main
```

---

## 🔄 日常工作流

### 1️⃣ 开始工作

```bash
# 1. 确保在最新版本
git pull origin main

# 2. 查看当前状态
git status

# 3. 开始编码...
```

### 2️⃣ 保存进度（本地提交）

```bash
# 1. 查看修改
git status

# 2. 添加文件
git add .

# 3. 提交（使用规范的消息）
git commit -m "feat: implement legal move generation"

# 4. 查看历史
git log --oneline -5
```

### 3️⃣ 同步到GitHub

```bash
# 方式1: 手动推送
git push origin main

# 方式2: 使用脚本（推荐）
./push_to_github.bat
```

### 4️⃣ 每日结束工作

```bash
# 1. 提交所有更改
git add -A
git commit -m "chore: daily progress - Week X Day Y"

# 2. 推送到GitHub（备份）
git push origin main

# 3. 查看状态（应该干净）
git status
```

---

## 📝 提交规范

### 提交消息格式

```
<type>(<scope>): <subject>

<body>

<footer>
```

### Type类型

| Type | 说明 | 示例 |
|------|------|------|
| **feat** | 新功能 | `feat: add bitboard legal move generation` |
| **fix** | Bug修复 | `fix: correct edge mask calculation` |
| **docs** | 文档更新 | `docs: update Week 1 learning notes` |
| **refactor** | 代码重构 | `refactor: simplify Board::display()` |
| **perf** | 性能优化 | `perf: optimize flip operation with SIMD` |
| **test** | 测试相关 | `test: add unit tests for Move class` |
| **chore** | 构建/工具 | `chore: update CMakeLists.txt` |
| **style** | 代码格式 | `style: format with clang-format` |

### Scope范围（可选）

- `core` - 核心模块（Board, Move）
- `ai` - AI引擎
- `ui` - 用户界面
- `network` - 网络模块
- `research` - 研究/基准测试
- `docs` - 文档

### 示例

#### ✅ 好的提交消息

```bash
# 简短清晰
git commit -m "feat: implement bitboard flip operations"

# 带scope
git commit -m "feat(core): add Zobrist hashing"

# 多行详细说明
git commit -m "fix(ai): correct minimax alpha-beta pruning

- Fix off-by-one error in depth calculation
- Add boundary checks for search window
- Improve move ordering for better pruning

Fixes #12"
```

#### ❌ 不好的提交消息

```bash
git commit -m "update"
git commit -m "fix bug"
git commit -m "改了一些东西"
git commit -m "asdfasdf"
```

---

## 🌿 分支管理

### 当前策略：单主分支

```
main (默认分支)
  └── 所有开发直接在main进行
```

**理由：**
- 单人项目，无需复杂分支
- 简化工作流
- 避免合并冲突

### 未来可选：功能分支

如果需要实验性功能：

```bash
# 创建功能分支
git checkout -b feature/mcts-implementation

# 开发并提交
git add .
git commit -m "feat: initial MCTS structure"

# 完成后合并回main
git checkout main
git merge feature/mcts-implementation

# 删除功能分支
git branch -d feature/mcts-implementation
```

---

## 🏷️ 版本标签

### 重要里程碑打标签

```bash
# Week 1完成 - Bitboard基础
git tag -a v0.1.0 -m "Week 1: Bitboard implementation complete"

# Week 3完成 - Minimax AI
git tag -a v0.2.0 -m "Week 3: Basic Minimax AI complete"

# Week 6完成 - 核心引擎
git tag -a v0.3.0 -m "Week 6: Core AI engine complete"

# Week 8完成 - 完整游戏
git tag -a v1.0.0 -m "Week 8: Full game with UI and network"

# Week 12完成 - 项目交付
git tag -a v2.0.0 -m "Week 12: Final project delivery"

# 推送标签到GitHub
git push origin --tags
```

### 查看标签

```bash
git tag                    # 列出所有标签
git show v0.1.0           # 查看标签详情
```

---

## 📊 项目历史追踪

### 查看提交历史

```bash
# 简洁视图
git log --oneline --graph --all -10

# 详细统计
git log --stat -5

# 特定文件历史
git log --oneline -- src/core/Board.cpp

# 特定作者
git log --author="Tianqixing"

# 特定时间
git log --since="2025-10-01" --until="2025-10-08"
```

### 查看代码变更

```bash
# 查看具体提交
git show <commit-hash>

# 比较两个提交
git diff <commit1> <commit2>

# 查看文件历史
git log -p -- src/core/Board.cpp
```

---

## 🔍 常见场景

### 场景1: 撤销未提交的修改

```bash
# 撤销工作区修改（危险！）
git restore <file>
git restore .              # 撤销所有

# 撤销暂存（保留修改）
git restore --staged <file>
```

### 场景2: 修改最后一次提交

```bash
# 修改提交消息
git commit --amend -m "new message"

# 添加遗漏的文件
git add forgotten_file.cpp
git commit --amend --no-edit
```

### 场景3: 查看某个版本

```bash
# 临时切换到历史版本
git checkout <commit-hash>

# 返回最新
git checkout main
```

### 场景4: 冲突解决

```bash
# 拉取时遇到冲突
git pull origin main
# ... 编辑冲突文件 ...
git add <resolved-files>
git commit -m "merge: resolve conflicts"
git push origin main
```

---

## 🛠️ 实用工具脚本

### push_to_github.bat
```batch
# 推送到GitHub（自动检查状态）
./push_to_github.bat
```

### 快速提交脚本（可创建）
```batch
@echo off
set /p message="Commit message: "
git add -A
git commit -m "%message%"
git push origin main
```

---

## 📌 最佳实践

### ✅ 推荐做法

1. **频繁提交**
   - 每完成一个小功能就提交
   - 每天至少1次提交

2. **清晰的消息**
   - 使用规范的提交格式
   - 说明"为什么"而不只是"做了什么"

3. **定期推送**
   - 每天工作结束推送到GitHub
   - 重要节点立即推送

4. **保持干净**
   - 提交前检查`git status`
   - 不提交编译产物（.exe, .o）
   - 不提交IDE配置

5. **备份重要版本**
   - 里程碑打标签
   - 重大修改前创建分支

### ❌ 避免的做法

1. **不要：**
   - 提交未测试的代码
   - 使用无意义的提交消息
   - 直接`push --force`到main
   - 提交敏感信息（密钥、密码）
   - 提交大文件（>10MB）

2. **危险命令慎用：**
   ```bash
   git reset --hard        # 会丢失所有修改
   git push --force        # 会覆盖远程历史
   git clean -fd           # 会删除未跟踪文件
   ```

---

## 🚨 紧急情况处理

### 误删文件恢复

```bash
# 恢复已删除但未提交的文件
git restore <file>

# 恢复已提交的删除（从历史）
git checkout <commit-hash> -- <file>
```

### 回退到之前版本

```bash
# 创建新提交回退（推荐）
git revert <commit-hash>

# 重置到某个版本（危险）
git reset --hard <commit-hash>
git push --force origin main  # 谨慎！
```

### 清理错误提交

```bash
# 回退最后一次提交（保留修改）
git reset --soft HEAD~1

# 重新提交
git add .
git commit -m "correct message"
```

---

## 📈 项目里程碑

### 已完成 ✅

- [x] **v0.0.1** - 项目初始化（2025-10-08）
- [x] **v0.0.2** - 文档整理完成（2025-10-08）

### 规划中 📅

- [ ] **v0.1.0** - Week 1: Bitboard实现
- [ ] **v0.2.0** - Week 3: Minimax AI
- [ ] **v0.3.0** - Week 6: 核心引擎完成
- [ ] **v1.0.0** - Week 8: UI与网络
- [ ] **v2.0.0** - Week 12: 最终交付

---

## 🔗 相关资源

- **GitHub仓库**: https://github.com/Apoxtq/reversi-ai-project
- **Git官方文档**: https://git-scm.com/doc
- **提交规范参考**: https://www.conventionalcommits.org/

---

## 📞 Git常用命令速查

```bash
# === 基础操作 ===
git status                 # 查看状态
git add .                  # 添加所有修改
git commit -m "message"    # 提交
git push origin main       # 推送
git pull origin main       # 拉取

# === 历史查看 ===
git log --oneline -10      # 简洁历史
git log --graph --all      # 图形历史
git show <commit>          # 查看提交

# === 分支操作 ===
git branch                 # 查看分支
git checkout -b <name>     # 创建并切换分支
git merge <branch>         # 合并分支

# === 撤销操作 ===
git restore <file>         # 撤销工作区修改
git restore --staged <file> # 取消暂存
git commit --amend         # 修改最后提交

# === 远程操作 ===
git remote -v              # 查看远程仓库
git fetch origin           # 获取远程更新
git push --tags            # 推送标签
```

---

**最后更新：** 2025年10月12日  
**版本：** 1.0  
**当前进度：** Week 1完成 ✅  
**维护者：** Tianqixing

