# LFSkipList - 高性能并发跳表引擎

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)](https://en.cppreference.com/w/)
[![Linux](https://img.shields.io/badge/Linux-x86__64-green)]()

**线程安全的高性能跳表实现**，采用概率层级算法与节点级细粒度锁，支持 O(log n) 的插入、查找和删除。

---

## 🔥 核心亮点

| 特性 | 实现 | 面试必问 |
|------|------|----------|
| **并发安全** | 节点级细粒度锁，读操作完全无锁 | 为什么比红黑树更适合并发？ |
| **概率层级** | 1/4 概率因子，期望层数可控 | 概率因子怎么选的？ |
| **性能验证** | 与 `std::set` 百万级压测对比 | 为什么插入慢但查询快？ |
| **工程实践** | 64 字节内存对齐，避免伪共享 | 伪共享是什么？怎么避免？ |

---

## 📊 性能对比（100万条数据）

| 操作 | 跳表 | std::set (红黑树) | 
|------|------|------------------|
| 插入 | 425 ms | 316 ms |
| 查询 | **149 ms** | 176 ms |

> 查询性能比 `std::set` 快 15%，插入略慢因动态内存分配，可通过内存池优化。

---

## 🚀 30秒跑起来

```bash
git clone https://github.com/whn121/LFSkipList.git && cd LFSkipList
mkdir build && cd build && cmake .. && make -j$(nproc)
./skiplist          # 性能压测
./test_skiplist     # 单元测试
```

---

## 🧩 架构设计

```
SkipList (模板类)
  ├── Node (每节点含 forward[] 跳表指针 + mutex 锁)
  ├── insert()   → 随机层级 + 逐层定位 + 插入
  ├── find()     → 逐层下降查找
  ├── remove()   → 逐层定位 + 删除
  └── randomLevel() → 概率生成层级 (P=0.25)
```

---

## 📁 项目结构

```
LFSkipList/
├── include/
│   └── skiplist.h       # 跳表核心实现（模板类）
├── src/
│   └── main.cpp         # 性能压测入口
├── test/
│   └── test_skiplist.cpp # 单元测试
├── CMakeLists.txt
└── README.md
```

---

## 🎯 面试可聊的点

- **跳表 vs 红黑树**：为什么 Redis ZSet 用跳表？范围查询、并发控制、实现复杂度三维对比
- **并发设计**：节点级锁 vs 全局锁，读写并发性能分析
- **内存优化**：伪共享问题与 64 字节对齐，内存池优化方向
- **概率因子**：1/4 的选择依据，期望层数 = 1/(1-p) = 1.33 层

---

## 📝 许可证

MIT License

**GitHub**: [whn121/LFSkipList](https://github.com/whn121/LFSkipList)
```
随便用
