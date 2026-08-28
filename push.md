# 推送到 GitHub（用 MSYS2 自带的 git）

仓库内已就绪：`engine/`（引擎源码）+ `.github/workflows/build.yml`（CI）+ `README.md`。

## 1. 创建远程仓库

GitHub 网页新建 **私有** 仓库，例如 `jysdl-switch`（不要点"Add README"等初始化选项）。

## 2. 打开 MSYS2 UCRT64 终端，执行

把 `<用户名>` 换成你的 GitHub 用户名：

```bash
cd "/d/金庸群侠传水月碧影v1.12/switch_port/gh-repo"
git init -b main
git add .
git commit -m "jysdl switch engine + CI"
git remote add origin https://github.com/<用户名>/jysdl-switch.git
git branch -M main
git push -u origin main
```

HTTPS 推送身份验证：

- 用户名：你的 GitHub 用户名
- 密码：**Token（PAT）**，不是登录密码
  - 生成：GitHub → Settings（个人设置）→ Developer settings → Personal access tokens → Tokens (classic)
  - 勾选 `repo` 权限，复制后立即保存（只显示一次）

## 3. 触发构建

- 推送即自动触发一次（`push` 事件）
- 之后可随时：仓库 Actions 页 → **Build switch NRO** → Run workflow

## 4. 下载产物

每次运行页底部 **Artifacts** → `jysdl-nro`，解压得 `jysdl.nro`。

放到 Switch 内存卡 `sdmc:/switch/jysdl/jysdl.nro`（与游戏数据同级），按
`安装说明.md` 运行。

## 常见问题

| 现象 | 处理 |
|---|---|
| `git push` 要求输入密码 | 用上面生成的 PAT 当密码粘贴 |
| 403 / `Repository not found` | 仓库是私有的，确认 push 用正确的用户名和 PAT，仓库名拼写无误 |
| 想改仓库名 | 网页重命名后重设 origin 即可 |
| 科学上网波动 | 可先 `git config --global http.sslVerify false`（临时），或换代理 `git config --global http.proxy http://127.0.0.1:7890`，用后取消 |