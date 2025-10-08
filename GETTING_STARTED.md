# 🚀 快速启动指南

> **项目**: Reversi AI Algorithm Benchmarking  
> **创建日期**: 2025年10月8日  
> **当前状态**: Phase 1 - Week 1 Day 1

---

## ✅ 已完成的准备工作

### 1. 项目结构 ✅
```
黑白棋毕业项目/
├── src/
│   ├── core/          # 核心游戏逻辑（待开发）
│   ├── ai/            # AI引擎（待开发）
│   ├── ui/            # 用户界面（待开发）
│   ├── network/       # 网络对战（待开发）
│   └── main.cpp       # ✅ 主程序入口（已创建）
├── tests/             # 单元测试（待开发）
├── docs/
│   ├── notes/         # 学习笔记
│   │   └── Bitboard_Day1.md  # ✅ Day1学习笔记模板
│   └── diagrams/      # UML图表（待创建）
├── assets/            # 资源文件（待添加）
├── build/             # 构建目录（Git忽略）
├── .gitignore         # ✅ Git忽略配置
├── CMakeLists.txt     # ✅ CMake构建配置
├── README.md          # ✅ 项目说明
└── TODO.md            # ✅ 12周开发计划
```

### 2. Git 版本控制 ✅
- [x] Git仓库初始化
- [x] 用户配置完成
- [x] 首次提交完成
- [x] 所有核心文件已纳入版本控制

### 3. 文档完备性 ✅
- [x] README.md - 项目概述和技术栈
- [x] TODO.md - 详细的12周开发计划
- [x] Bitboard_Day1.md - 学习笔记模板
- [x] GETTING_STARTED.md - 本文档

---

## 📅 今晚/明天的学习任务

### Week 1 Day 1 学习计划（2小时）

**学习时间**: 今晚19:00-22:30

**学习材料**:
```
D:\大三390毕业选课黑白棋项目\他人成熟黑白棋项目参考\Egaroucid\src\engine\bit.hpp
```

**任务清单**:
1. ⏰ 19:05-19:30 - 阅读 bit.hpp Line 1-50
2. ⏰ 19:35-20:00 - 阅读 bit.hpp Line 51-100
3. ⏰ 20:05-20:30 - 阅读 bit.hpp Line 101-150
4. ⏰ 20:35-21:00 - 阅读 bit.hpp Line 151-200
5. ☕ 21:00-21:15 - 长休息
6. ⏰ 21:15-21:40 - 完善学习笔记
7. ⏰ 21:45-22:10 - Git提交学习笔记
8. ⏰ 22:10-22:30 - 总结和计划明天

**学习笔记位置**:
```
docs/notes/Bitboard_Day1.md
```

---

## 🛠️ 开发环境设置

### 必需工具（已安装）
- [x] GCC 13.2.0
- [x] CMake 4.1.2
- [x] Git
- [x] VSCode

### 待安装工具（Week 1开始时）
```powershell
# SFML 图形库
scoop install sfml

# Google Test 测试框架
scoop install gtest

# 验证安装
pkg-config --modversion sfml-all
```

---

## 💻 编译和运行（当前版本）

### 方法1：直接编译（快速测试）
```powershell
# 进入项目目录
cd "D:\大三390毕业选课黑白棋项目\黑白棋毕业项目"

# 直接编译main.cpp
g++ -std=c++17 -O2 -o reversi_ai.exe src/main.cpp

# 运行
.\reversi_ai.exe
```

### 方法2：CMake构建（推荐）
```powershell
# 1. 创建构建目录
New-Item -ItemType Directory -Force -Path build

# 2. 进入构建目录
cd build

# 3. 配置CMake
cmake .. -G "MinGW Makefiles"

# 4. 编译
cmake --build . --config Release

# 5. 运行
.\bin\reversi_ai.exe
```

### 预期输出
```
========================================
  Reversi AI v1.0.0
========================================
项目: COMP390 Honours Year Project
学生: Tianqixing (201821852)
主题: 黑白棋AI算法比较与优化研究
========================================

编译信息:
----------------------------------------
C++ 标准: C++17
编译器: GCC 13.2.0
构建类型: Release
平台: Windows 64-bit
----------------------------------------

当前开发状态:
----------------------------------------
✅ Phase 0: 环境配置完成
✅ 项目结构创建完成
🔄 Phase 1: Week 1 - Bitboard学习中...
⏳ Phase 2: 核心引擎开发（待开始）
⏳ Phase 3: UI与网络（待开始）
⏳ Phase 4: 测试与文档（待开始）
----------------------------------------

=== Bitboard 演示 ===
玩家棋子数量: 2
对手棋子数量: 2

初始棋盘状态:
  A B C D E F G H
8 . . . . . . . .
7 . . . . . . . .
6 . . . . . . . .
5 . . . ○ ● . . .
4 . . . ● ○ . . .
3 . . . . . . . .
2 . . . . . . . .
1 . . . . . . . .

📚 下一步:
1. 完成 Week 1 Bitboard 学习
2. 实现 Board 类（Week 3）
3. 实现 Minimax 引擎（Week 4）
4. 实现 MCTS 引擎（Week 5）

🚀 项目启动成功！开始你的学习之旅吧！
```

---

## 📝 Git 使用指南

### 查看状态
```powershell
git status
```

### 查看提交历史
```powershell
git log --oneline
```

### 提交学习笔记（每日任务）
```powershell
# 1. 查看修改
git status
git diff docs/notes/Bitboard_Day1.md

# 2. 添加文件
git add docs/notes/Bitboard_Day1.md

# 3. 提交
git commit -m "docs(learning): add Bitboard Day 1 learning notes

- Read Egaroucid bit.hpp line 1-200
- Understand basic bitboard concepts
- Document 64-bit board representation

Learning time: 2 hours"

# 4. 查看提交
git log -1
```

### 提交信息规范
```
类型(范围): 简短描述

- 详细说明1
- 详细说明2

附加信息
```

**类型标签**:
- `feat`: 新功能
- `fix`: Bug修复
- `docs`: 文档更新
- `refactor`: 重构
- `perf`: 性能优化
- `test`: 测试相关
- `chore`: 构建/工具配置

---

## 🎯 Week 1 学习目标

### 核心目标
- [ ] 理解Bitboard的64位表示法
- [ ] 理解翻转算法的8个方向
- [ ] 能手写简单的位运算操作
- [ ] 完成2000字+学习笔记
- [ ] 至少6次Git提交

### 学习材料
1. **Egaroucid源码** (主要参考)
   - `bit.hpp` - Bitboard核心操作
   - `flip.hpp` - 翻转算法实现

2. **在线资源** (辅助学习)
   - [Chess Programming Wiki - Bitboards](https://www.chessprogramming.org/Bitboards)
   - [位运算基础教程](https://www.geeksforgeeks.org/bitwise-operators-in-c-cpp/)

### 时间分配
| 天数 | 任务 | 时间 |
|------|------|------|
| Day 1 | bit.hpp 前200行 | 2小时 |
| Day 2 | bit.hpp 200-500行 | 3小时 |
| Day 3 | flip.hpp 前400行 | 3小时 |
| Day 4 | flip.hpp 400-800行 | 3小时 |
| Day 5 | 编写测试代码 | 4小时 |
| Day 6 | 完善笔记总结 | 3小时 |
| **总计** | - | **18小时** |

---

## 📋 检查清单

### 今晚完成前检查 ✅
- [x] 项目目录结构创建完成
- [x] README.md 创建完成
- [x] TODO.md 创建完成
- [x] CMakeLists.txt 创建完成
- [x] .gitignore 创建完成
- [x] main.cpp 创建完成
- [x] Git仓库初始化
- [x] 首次提交完成
- [x] 学习笔记模板创建

### 明天开始前检查
- [ ] 电脑已充电
- [ ] 关闭无关程序
- [ ] 关闭手机通知
- [ ] 准备水杯
- [ ] VSCode已打开
- [ ] 学习笔记文件已打开
- [ ] Egaroucid的bit.hpp已定位
- [ ] 番茄钟已准备（可选）
- [ ] 深呼吸，准备开始！

---

## 🔗 连接到GitHub（可选）

### 创建GitHub仓库
1. 访问 https://github.com
2. 点击右上角 "+" → "New repository"
3. 填写信息：
   - Repository name: `reversi-ai-project`
   - Description: "Reversi AI Algorithm Benchmarking and Optimisation Research"
   - Visibility: Public 或 Private
   - 不要勾选初始化选项（我们已有本地仓库）
4. 点击 "Create repository"

### 连接本地仓库到GitHub
```powershell
# 添加远程仓库（替换YOUR_USERNAME）
git remote add origin https://github.com/YOUR_USERNAME/reversi-ai-project.git

# 推送到GitHub
git branch -M main
git push -u origin main

# 验证
git remote -v
```

---

## ❓ 常见问题

### Q1: CMake配置失败？
**A**: 可能原因：
1. SFML未安装 → 运行 `scoop install sfml`
2. 编译器路径问题 → 检查 `gcc --version`
3. 暂时跳过CMake，使用直接编译：
   ```powershell
   g++ -std=c++17 -O2 -o reversi_ai.exe src/main.cpp
   ```

### Q2: 找不到Egaroucid源码？
**A**: 源码位置：
```
D:\大三390毕业选课黑白棋项目\他人成熟黑白棋项目参考\Egaroucid\src\engine\bit.hpp
```

### Q3: Git提交遇到问题？
**A**: 检查配置：
```powershell
git config --list
git config user.name "Your Name"
git config user.email "your@email.com"
```

### Q4: 学习进度跟不上？
**A**: 建议：
1. 降低目标：每天2小时改为1.5小时
2. 跳过不懂的部分，先继续
3. 周末补足进度
4. 标记❓，后面再回来理解

---

## 💡 学习技巧

### 番茄工作法
- 25分钟专注学习
- 5分钟休息
- 4个番茄后休息15分钟
- 严格遵守时间

### 笔记方法
```markdown
### 代码片段 [Line XX-XX]
\`\`\`cpp
// 复制关键代码
\`\`\`

**理解**: ...
**疑问**: ❓ ...
```

### 时间管理
- 每天固定时间学习
- 避开精力低谷期
- 手机静音
- 准备充足水分

---

## 🎉 下一步行动

### 立即行动（今晚）
1. ✅ 阅读本文档
2. ✅ 验证环境（运行main.cpp）
3. ⏰ 设定明天闹钟（7:00 AM）
4. ⏰ 准备学习环境

### 明天行动（Day 1）
1. 🎯 19:00 准时开始
2. 📚 完成Bitboard学习（2小时）
3. 📝 更新学习笔记
4. 💾 Git提交

### 本周行动（Week 1）
1. 📖 完成bit.hpp和flip.hpp学习
2. 💻 编写测试代码
3. 📊 完成Week 1总结
4. 🎯 准备Week 2（MCTS学习）

---

**项目启动时间**: 2025年10月8日  
**当前状态**: Week 1 Day 1  
**下一个里程碑**: 2025年10月14日（Week 1完成）

---

**记住**: 
> 每一个伟大的项目都始于第一步。  
> 你已经完成了最重要的准备工作！  
> 现在，开始你的学习之旅吧！ 🚀

---

*文档创建: 2025年10月8日*  
*最后更新: 2025年10月8日*

