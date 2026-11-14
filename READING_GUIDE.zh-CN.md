# VERDICT 中文源码阅读路线

这份代码对应论文 [VERDICT: Multi-Agent Parallel Modular Verification](https://arxiv.org/abs/2610.29147)。论文参考文献 [12] 指向本副本的[原始 artifact 仓库](https://anonymous.4open.science/r/verdict-artifact-7158)。它包含可读的 Python、C++ 和 OCaml 实现，适合按论文机制逐层学习。

## 先抓住主线

每个 C 函数交给一个持久的 agent。Agent 可以暂时假设被调用函数的契约，先并行生成和验证自己的 ACSL 规格；随后通过契约发布、依赖检查和向下提出证明需求来协调各函数。使用了尚未兑现的假设，不等于已经完成全局验证。

## 建议阅读顺序

| 顺序 | 入口 | 阅读重点 |
| --- | --- | --- |
| 1 | [小例子](benchmarks/small_example.c)、[稍复杂的例子](benchmarks/medium_example.c) | 先认识输入 C 程序、函数及调用关系。 |
| 2 | [CLI](src/verdict/cli/app.py)、[solve 模式](src/verdict/cli/solve_mode.py) | 从命令行参数追踪预处理、agent 构建和完整运行。 |
| 3 | [核心类型](src/verdict/core/types.py)、[注册表](src/verdict/agent/registry.py) | 理解 FunctionSpec、版本、依赖快照、假设及 ProofState。 |
| 4 | [FunctionAgent](src/verdict/agent/runner.py)、[提示词](src/verdict/prompts/function_agent.py) | 看一个函数 agent 如何配置 Claude SDK、调用工具并保持会话。 |
| 5 | [契约验证工具](src/verdict/tools/verify/contract.py)、[被调用函数契约选择](src/verdict/tools/verify/_shared.py) | 追踪 callee_contracts 与 assume_callee，区分已发布契约和临时假设。 |
| 6 | [调度器](src/verdict/orchestrator/orchestrator.py)、[唤醒队列](src/verdict/orchestrator/wake_queue.py) | 理解并行调度、事件处理、重试预算和停止条件。 |
| 7 | [证明需求工具](src/verdict/tools/runtime/proof_demand.py)、[需求存储](src/verdict/agent/proof_demands.py) | 理解调用者如何请求被调用者提供所需性质。 |
| 8 | [证明状态推导](src/verdict/orchestrator/proof_state.py)、[依赖判断](src/verdict/orchestrator/dependency_judge.py) | 理解契约变更如何使旧证明失效，何时能达到 GLOBAL_OK。 |
| 9 | [WP 工具层](src/verdict/tools/_wp_runner.py)、[Frama-C 运行器](src/verdict/framac/runner.py) | 追踪 ACSL 注释如何交给 Frama-C/WP 和 SMT 求解器。 |
| 10 | [预处理流水线](src/verdict/preprocessing/pipeline.py)、[C++ 绑定](src/verdict/preprocessing/bindings/clang_parser.cpp)、[OCaml 插件](frama-c-plugins/global-deps/global_deps.ml) | 理解解析、调用关系、依赖提取及底层工具边界。 |

## 论文术语如何对应代码

- **Assumption-based synthesis**：`verify_function_contract` 的 `callee_contracts` 与 `assume_callee`，以及 `FunctionSpec.assumed_callees`。发布时，相关依赖会记为版本 0，阻止把未兑现的假设直接当成全局证明。
- **Supply**：代码中可从注册表发布 `SpecPublished` 事件开始追踪，再看调度器如何重检依赖、唤醒调用者；不必只搜索单词 Supply。
- **Demand**：从 `submit_proof_demand` 及 `proof_demands.py` 追踪向下提出的证明需求、需求状态及处理后的重新调度。
- **Version / fixpoint**：关注 `FunctionSpec.version`、`based_on_versions`、`proof_state.py` 与 `Orchestrator.run`。
- **局部与全局验证**：`LOCAL_OK` 与 `GLOBAL_OK` 不是同一状态；应结合依赖版本、假设是否兑现及环境公理理解最终结果。

## 用已有测试辅助理解

优先阅读 [假设依赖测试](tests/test_assume_callee_debt.py)、[发布门槛测试](tests/test_publish_gate.py)、[契约方向测试](tests/test_contract_direction_guards.py)、[依赖判断测试](tests/test_dependency_judge.py) 和 [证明失效测试](tests/test_vo_invalidation.py)。这些测试比直接通读所有实验日志更容易定位设计约束。

## 再看实验材料

- [实验说明](scripts/experiments/README.md)：RQ1、RQ2、RQ3、运行配置和统一评判方法。
- [统一评判脚本](scripts/experiments/lib/score_final.py)：理解论文如何比较不同工具的最终注释结果。
- [图表说明](scripts/figures/README.md)：从已附结果生成论文表格和图形。
- `scripts/experiments/results/`：保留最终注释、判分结果及经过整理的运行数据。它不是所有中间过程的完整归档。

## 运行边界

阅读代码不需要 API key。真实运行则需要 Python 3.10+、uv、Claude Agent SDK、Anthropic API key、Frama-C/WP、SMT 求解器和 LLVM/Clang 等，具体以作者 [README](README.md) 与构建脚本为准。不要把包内已有的 `.dylib` 当作任意机器都可用的构建结果。

本次完成了下载来源核对、ZIP CRC 校验、140 个 Python 文件的语法解析和部分关键机制核查；没有安装完整验证环境、运行论文实验或调用付费模型。因此本记录确认的是源码对应关系和可读性，并不声称已经复现论文所有实验结果。

Preguss 和 SpecSyn 基线工具本体没有随包提供；只有相关运行脚本和整理后的结果。
