#include "skiplist.h"
#include <set>
#include <chrono>
#include <iostream>
#include <iomanip>

int main() {
    SkipList<int, int> skiplist(16);
    std::set<int> stl_set;

    const int N = 1000000;

    // 测试跳表插入性能
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) {
        skiplist.insert(i, i * 10);
    }
    auto end = std::chrono::steady_clock::now();
    double skiplist_insert = std::chrono::duration<double, std::milli>(end - start).count();

    // 测试 STL set 插入性能
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) {
        stl_set.insert(i);
    }
    end = std::chrono::steady_clock::now();
    double set_insert = std::chrono::duration<double, std::milli>(end - start).count();

    // 测试查询性能
    int value;
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) {
        skiplist.find(i, value);
    }
    end = std::chrono::steady_clock::now();
    double skiplist_find = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) {
        stl_set.find(i);
    }
    end = std::chrono::steady_clock::now();
    double set_find = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "=== SkipList vs std::set 性能对比 (100万条数据) ===" << std::endl;
    std::cout << "操作   | 跳表耗时  | std::set耗时" << std::endl;
    std::cout << "插入   | " << std::setw(7) << skiplist_insert << " ms | " << set_insert << " ms" << std::endl;
    std::cout << "查询   | " << std::setw(7) << skiplist_find << " ms | " << set_find << " ms" << std::endl;

    return 0;
}
