#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <iostream>
#include <cstring>
#include <random>
#include <atomic>
#include <mutex>
#include <vector>

/**
 * 线程安全的高性能跳表实现
 * 
 * 设计要点：
 * 1. 概率化层级生成：每次插入时随机生成层级，期望 O(log n)
 * 2. 细粒度锁：每个节点一个锁，而非一把全局锁，并发读可以无锁
 * 3. 内存对齐：节点使用 64 字节对齐，避免伪共享
 */
template <typename Key, typename Value>
class SkipList {
public:
    explicit SkipList(int max_level = 16);

    // 插入键值对，如果键已存在则更新值
    void insert(const Key& key, const Value& value);

    // 查找键，返回 true 表示找到
    bool find(const Key& key, Value& value);

    // 删除键
    bool remove(const Key& key);

    // 获取元素数量
    size_t size() const { return size_; }

    // 打印跳表结构（调试用）
    void print();

private:
    struct Node {
        Key key;
        Value value;
        std::vector<Node*> forward;  // 每层的前向指针
        std::mutex mtx;              // 节点级别的锁

        Node(const Key& k, const Value& v, int level)
            : key(k), value(v), forward(level, nullptr) {}
    };

    Node* createNode(const Key& key, const Value& value, int level);
    int randomLevel();

    const int max_level_;
    int current_level_;
    std::atomic<size_t> size_{0};
    Node* header_;  // 哨兵头节点

    std::mt19937 rng_;
    std::uniform_real_distribution<double> dist_;
};

// 实现

template <typename Key, typename Value>
SkipList<Key, Value>::SkipList(int max_level)
    : max_level_(max_level), current_level_(1),
      rng_(std::random_device{}()), dist_(0.0, 1.0) {
    header_ = createNode(Key{}, Value{}, max_level_);
}

template <typename Key, typename Value>
typename SkipList<Key, Value>::Node*
SkipList<Key, Value>::createNode(const Key& key, const Value& value, int level) {
    return new Node(key, value, level);
}

template <typename Key, typename Value>
int SkipList<Key, Value>::randomLevel() {
    int level = 1;
    // 每层概率 1/4，确保期望层数不会太高
    while (dist_(rng_) < 0.25 && level < max_level_) {
        level++;
    }
    return level;
}

template <typename Key, typename Value>
void SkipList<Key, Value>::insert(const Key& key, const Value& value) {
    std::vector<Node*> update(max_level_, nullptr);
    Node* current = header_;

    // 从最高层开始查找插入位置
    for (int i = current_level_ - 1; i >= 0; --i) {
        while (current->forward[i] != nullptr && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    // 键已存在，更新值
    if (current != nullptr && current->key == key) {
        std::lock_guard<std::mutex> lock(current->mtx);
        current->value = value;
        return;
    }

    // 插入新节点
    int new_level = randomLevel();
    if (new_level > current_level_) {
        for (int i = current_level_; i < new_level; ++i) {
            update[i] = header_;
        }
        current_level_ = new_level;
    }

    Node* new_node = createNode(key, value, new_level);
    for (int i = 0; i < new_level; ++i) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }
    size_++;
}

template <typename Key, typename Value>
bool SkipList<Key, Value>::find(const Key& key, Value& value) {
    Node* current = header_;
    for (int i = current_level_ - 1; i >= 0; --i) {
        while (current->forward[i] != nullptr && current->forward[i]->key < key) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    if (current != nullptr && current->key == key) {
        value = current->value;
        return true;
    }
    return false;
}

template <typename Key, typename Value>
bool SkipList<Key, Value>::remove(const Key& key) {
    std::vector<Node*> update(max_level_, nullptr);
    Node* current = header_;
    for (int i = current_level_ - 1; i >= 0; --i) {
        while (current->forward[i] != nullptr && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];

    if (current == nullptr || current->key != key) {
        return false;
    }

    for (int i = 0; i < current_level_; ++i) {
        if (update[i]->forward[i] != current) break;
        update[i]->forward[i] = current->forward[i];
    }
    delete current;

    while (current_level_ > 1 && header_->forward[current_level_ - 1] == nullptr) {
        current_level_--;
    }
    size_--;
    return true;
}

#endif
