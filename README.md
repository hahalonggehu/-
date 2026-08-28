# 金庸群侠传 水月碧影 v1.12 — Switch 移植（GitHub Actions 构建）

将《金庸群侠传：水月碧影 v1.12》（基于 jysdl 引擎）交叉编译为 Switch
Homebrew（NRO）。本仓库 **只含引擎源码**，不含任何游戏数据。

## 构建

推送到本仓库后，GitHub Actions 自动在 devkitPro 官方 docker 镜像
（`devkitpro/devkita64`）里编译 `engine/jysdl.nro`：

- 手动触发：Actions 页面 → **Build switch NRO** → Run workflow
- 自动触发：任意 push（`push` 事件）

构建产物在每次运行页底部的 **Artifacts** 中下载（`jysdl-nro`）。

同时 CI 会自动把 `jysdl.nro` 提交到**仓库根目录**：
回到仓库首页 → 点 `jysdl.nro` → 右侧 **Download** 按钮即可直接下载。
（多个分支同名文件时以最新提交为准；请用文件刷新确认时间。）

## 在 Switch 上运行

1. 下载 NRO，放到 `sdmc:/switch/jysdl/`（与游戏数据目录同级）。
2. 游戏数据（约 180MB）放于 `sdmc:/switch/jysdl/` 下，含
   `config.lua`、`data/`、`font/`、`pic/`、`script/` 等目录。
3. 大气层按住 R 进入相册（或主菜单使用 HB 加载器）运行。

完整安装说明见移植区 `安装说明.md`。

## 目录结构

    engine/                  引擎源码（jysdl + Lua 5.2 + minizip，__SWITCH__ 分支）
      src/                   移植源码（sdlfun.cpp 音频/手柄、jymain.cpp 路径等）
      minizip_config/        mz_config.h 头文件配置、get_file_date.c
      vendor/                minizip-ng、Lua 5.2 源码
      Makefile               编译入口（需 devkitA64 环境）
    .github/workflows/       GitHub Actions 构建脚本