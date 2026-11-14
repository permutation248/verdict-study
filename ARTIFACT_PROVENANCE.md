# 来源与检查记录

本目录是为个人学习整理的论文 artifact 副本，不是作者官方维护仓库。

## 论文与下载来源

- 论文：**VERDICT: Multi-Agent Parallel Modular Verification**。
- 作者：Tengjie Lin、Zhongyi Wang、Haokun Li、Jianwei Yin、Mingshuai Chen。
- [arXiv 论文](https://arxiv.org/abs/2610.29147)；[论文 HTML](https://arxiv.org/html/2610.29147v1)。
- 论文参考文献 [12]：[verdict-artifact-7158](https://anonymous.4open.science/r/verdict-artifact-7158)。
- 原始 ZIP 下载地址：`https://anonymous.4open.science/api/repo/verdict-artifact-7158/zip`。
- 原始文件名：`verdict-artifact-7158.zip`。
- 本地检查日期：2026-11-14（Asia/Shanghai）。
- macOS 记录的下载来源与上述 ZIP 地址完全一致。

## 完整性与内容

| 项目 | 结果 |
| --- | --- |
| ZIP 大小 | 17,382,885 字节 |
| ZIP SHA-256（本地计算） | `0bb289b362496c905dd14337e17602996bb5c225554b8cd0a1c83290e674747b` |
| ZIP CRC | 全部成员通过 |
| 原始文件数 | 2,312 |
| 原始展开大小 | 144,231,738 字节 |
| Python 文件 | 140 个，其中核心 `src/verdict/` 下 87 个 |
| Python 语法解析 | 140 个均通过；未执行模块代码 |
| 主要额外语言 | C++ 解析绑定、OCaml Frama-C 插件、C 基准程序 |
| 大于 100 MiB 的文件 | 无；最大文件约 4.97 MB |
| 常见密钥格式与敏感文件名检查 | 未发现匹配项；不等于完整安全审计 |

未取得发布方的独立哈希清单，不能将本地 SHA-256 当作官方签名或官方哈希比对。

## 与论文机制的对应

代码中存在函数级 agent、临时 callee 契约假设、契约版本和依赖记录、SpecPublished 向上通知、proof demand 向下请求、证明状态推导、并行调度及 Frama-C/WP 调用。关键模块的阅读路线见 [中文指南](READING_GUIDE.zh-CN.md)。

本检查不是对数学正确性、安全性或全部实验结果的独立复现。完整运行需要外部验证工具和模型服务。作者明确说明基线工具本体与完整中间日志不随包提供。

## 学习副本的改动

- 保留全部原始实现、测试、基准、锁文件、实验材料及许可文件。
- 仅在原 README 顶部增加学习副本说明与导航。
- 新增 `READING_GUIDE.zh-CN.md` 和本记录。
- 原始 SHA-256 对应下载的 ZIP，不对应补充说明后的目录。
- 从 ZIP 导入，不包含上游 Git 提交历史；没有改写作者身份或声称为原创实现。

## 许可与归属

根目录 [LICENSE](LICENSE) 为 MIT，版权归 `The Verdict Authors`。本副本保留原文。基准程序、Frama-C 插件及其他第三方材料应同时遵循各自文件中的版权和许可说明；不要将所有第三方内容统一解释为新授予的 MIT 许可。
