#include "skiplist.h"
#include <iostream>
#include <cassert>

int main() {
    SkipList<int, std::string> skiplist(16);

    // ========== 测试1：插入和查找 ==========
    std::cout << "[Test 1] Insert and Find... ";
    skiplist.insert(1, "one");
    skiplist.insert(2, "two");
    skiplist.insert(3, "three");

    std::string value;
    assert(skiplist.find(1, value) && value == "one");
    assert(skiplist.find(2, value) && value == "two");
    assert(skiplist.find(3, value) && value == "three");
    assert(!skiplist.find(999, value));  // 不存在的 key
    std::cout << "PASSED" << std::endl;

    // ========== 测试2：更新已存在的 key ==========
    std::cout << "[Test 2] Update existing key... ";
    skiplist.insert(1, "ONE");           // 更新 key=1 的值
    assert(skiplist.find(1, value) && value == "ONE");
    std::cout << "PASSED" << std::endl;

    // ========== 测试3：删除 ==========
    std::cout << "[Test 3] Delete... ";
    assert(skiplist.remove(2));          // 删除 key=2
    assert(!skiplist.find(2, value));    // 确认已删除
    assert(skiplist.size() == 2);        // 还剩 2 个元素
    std::cout << "PASSED" << std::endl;

    // ========== 测试4：删除不存在的 key ==========
    std::cout << "[Test 4] Delete non-existent key... ";
    assert(!skiplist.remove(999));
    std::cout << "PASSED" << std::endl;

    // ========== 测试5：大数量插入和查找 ==========
    std::cout << "[Test 5] Large-scale insert and find... ";
    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        skiplist.insert(i, std::to_string(i));
    }
    for (int i = 0; i < N; ++i) {
        assert(skiplist.find(i, value) && value == std::to_string(i));
    }
    std::cout << "PASSED (" << N << " elements)" << std::endl;

    std::cout << "\n🎉 All tests passed!" << std::endl;
    return 0;
}
