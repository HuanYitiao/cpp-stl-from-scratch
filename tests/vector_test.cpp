#include <ministl/vector.h>

#include <gtest/gtest.h>

#include <string>
#include <utility>

// ═══════════════════════════════════════════════════════════════
// Construction
// ═══════════════════════════════════════════════════════════════

TEST(VectorTest, DefaultConstruct) {
    ministl::vector<int> v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0u);
    EXPECT_EQ(v.capacity(), 0u);
}

TEST(VectorTest, FillConstruct) {
    ministl::vector<int> v(5, 42);
    EXPECT_EQ(v.size(), 5u);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 42);
    }
}

TEST(VectorTest, InitializerListConstruct) {
    ministl::vector<int> v{1, 2, 3, 4, 5};
    EXPECT_EQ(v.size(), 5u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorTest, RangeConstruct) {
    int arr[] = {10, 20, 30};
    ministl::vector<int> v(std::begin(arr), std::end(arr));
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[2], 30);
}

TEST(VectorTest, CopyConstruct) {
    ministl::vector<int> v1{1, 2, 3};
    ministl::vector<int> v2(v1);
    EXPECT_EQ(v1, v2);
    // Verify deep copy
    v2[0] = 99;
    EXPECT_NE(v1[0], v2[0]);
}

TEST(VectorTest, MoveConstruct) {
    ministl::vector<int> v1{1, 2, 3};
    auto* old_data = v1.data();
    ministl::vector<int> v2(std::move(v1));
    EXPECT_EQ(v2.data(), old_data);
    EXPECT_TRUE(v1.empty());
}

// ═══════════════════════════════════════════════════════════════
// Assignment
// ═══════════════════════════════════════════════════════════════

TEST(VectorTest, CopyAssign) {
    ministl::vector<int> v1{1, 2, 3};
    ministl::vector<int> v2;
    v2 = v1;
    EXPECT_EQ(v1, v2);
}

TEST(VectorTest, MoveAssign) {
    ministl::vector<int> v1{1, 2, 3};
    ministl::vector<int> v2;
    v2 = std::move(v1);
    EXPECT_EQ(v2.size(), 3u);
    EXPECT_TRUE(v1.empty());
}

TEST(VectorTest, InitializerListAssign) {
    ministl::vector<int> v;
    v = {10, 20, 30};
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[1], 20);
}

TEST(VectorTest, SelfAssign) {
    ministl::vector<int> v{1, 2, 3};
    v = v;
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
}

// ═══════════════════════════════════════════════════════════════
// Element access
// ═══════════════════════════════════════════════════════════════

TEST(VectorTest, At) {
    ministl::vector<int> v{10, 20, 30};
    EXPECT_EQ(v.at(0), 10);
    EXPECT_EQ(v.at(2), 30);
}

TEST(VectorTest, AtOutOfRange) {
    ministl::vector<int> v{1, 2, 3};
    EXPECT_THROW(v.at(3), std::out_of_range);
    EXPECT_THROW(v.at(100), std::out_of_range);
}

TEST(VectorTest, FrontBack) {
    ministl::vector<int> v{10, 20, 30};
    EXPECT_EQ(v.front(), 10);
    EXPECT_EQ(v.back(), 30);
}

TEST(VectorTest, Data) {
    ministl::vector<int> v{1, 2, 3};
    int* p = v.data();
    EXPECT_EQ(p[0], 1);
    EXPECT_EQ(p[2], 3);
}

// ═══════════════════════════════════════════════════════════════
// Iterators
// ═══════════════════════════════════════════════════════════════

TEST(VectorTest, BeginEnd) {
    ministl::vector<int> v{1, 2, 3};
    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST(VectorTest, RangeFor) {
    ministl::vector<int> v{1, 2, 3};
    int sum = 0;
    for (int x : v) {
        sum += x;
    }
    EXPECT_EQ(sum, 6);
}

TEST(VectorTest, ReverseIterator) {
    ministl::vector<int> v{1, 2, 3};
    auto it = v.rbegin();
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(*it, 2);
}

TEST(VectorTest, ConstIterator) {
    const ministl::vector<int> v{1, 2, 3};
    int sum = 0;
    for (auto it = v.cbegin(); it != v.cend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

// ═══════════════════════════════════════════════════════════════
// Capacity
// ═══════════════════════════════════════════════════════════════

TEST(VectorTest, Reserve) {
    ministl::vector<int> v;
    v.reserve(100);
    EXPECT_GE(v.capacity(), 100u);
    EXPECT_EQ(v.size(), 0u);
}

TEST(VectorTest, ReservePreservesData) {
    ministl::vector<int> v{1, 2, 3};
    v.reserve(100);
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorTest, ShrinkToFit) {
    ministl::vector<int> v;
    v.reserve(100);
    v.push_back(1);
    v.push_back(2);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), v.size());
}

// ═══════════════════════════════════════════════════════════════
// Modifiers
// ═══════════════════════════════════════════════════════════════

TEST(VectorTest, PushBack) {
    ministl::vector<int> v;
    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }
    EXPECT_EQ(v.size(), 100u);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[99], 99);
}

TEST(VectorTest, PushBackMove) {
    ministl::vector<std::string> v;
    std::string s = "hello";
    v.push_back(std::move(s));
    EXPECT_EQ(v[0], "hello");
    EXPECT_TRUE(s.empty());  // moved-from
}

TEST(VectorTest, EmplaceBack) {
    ministl::vector<std::pair<int, std::string>> v;
    v.emplace_back(1, "one");
    v.emplace_back(2, "two");
    EXPECT_EQ(v[0].first, 1);
    EXPECT_EQ(v[1].second, "two");
}

TEST(VectorTest, PopBack) {
    ministl::vector<int> v{1, 2, 3};
    v.pop_back();
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v.back(), 2);
}

TEST(VectorTest, InsertMiddle) {
    ministl::vector<int> v{1, 2, 4, 5};
    auto it = v.insert(v.begin() + 2, 3);
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(v.size(), 5u);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], i + 1);
    }
}

TEST(VectorTest, InsertBegin) {
    ministl::vector<int> v{2, 3};
    v.insert(v.begin(), 1);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v.size(), 3u);
}

TEST(VectorTest, EraseSingle) {
    ministl::vector<int> v{1, 2, 3, 4, 5};
    auto it = v.erase(v.begin() + 2);  // erase 3
    EXPECT_EQ(*it, 4);
    EXPECT_EQ(v.size(), 4u);
    EXPECT_EQ(v[2], 4);
}

TEST(VectorTest, EraseRange) {
    ministl::vector<int> v{1, 2, 3, 4, 5};
    v.erase(v.begin() + 1, v.begin() + 3);  // erase 2,3
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 4);
    EXPECT_EQ(v[2], 5);
}

TEST(VectorTest, Clear) {
    ministl::vector<int> v{1, 2, 3};
    v.clear();
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0u);
    EXPECT_GE(v.capacity(), 3u);  // capacity unchanged
}

TEST(VectorTest, Resize) {
    ministl::vector<int> v{1, 2, 3};
    v.resize(5, 0);
    EXPECT_EQ(v.size(), 5u);
    EXPECT_EQ(v[3], 0);
    EXPECT_EQ(v[4], 0);

    v.resize(2);
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST(VectorTest, Swap) {
    ministl::vector<int> v1{1, 2, 3};
    ministl::vector<int> v2{4, 5};
    v1.swap(v2);
    EXPECT_EQ(v1.size(), 2u);
    EXPECT_EQ(v2.size(), 3u);
    EXPECT_EQ(v1[0], 4);
    EXPECT_EQ(v2[0], 1);
}

// ═══════════════════════════════════════════════════════════════
// Comparison
// ═══════════════════════════════════════════════════════════════

TEST(VectorTest, Equality) {
    ministl::vector<int> v1{1, 2, 3};
    ministl::vector<int> v2{1, 2, 3};
    ministl::vector<int> v3{1, 2, 4};
    EXPECT_EQ(v1, v2);
    EXPECT_NE(v1, v3);
}

TEST(VectorTest, LessThan) {
    ministl::vector<int> v1{1, 2, 3};
    ministl::vector<int> v2{1, 2, 4};
    ministl::vector<int> v3{1, 2, 3, 0};
    EXPECT_TRUE(v1 < v2);
    EXPECT_TRUE(v1 < v3);
    EXPECT_FALSE(v2 < v1);
}

// ═══════════════════════════════════════════════════════════════
// Non-trivial types
// ═══════════════════════════════════════════════════════════════

TEST(VectorTest, StringVector) {
    ministl::vector<std::string> v;
    v.push_back("hello");
    v.push_back("world");
    v.emplace_back("!");
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], "hello");
    EXPECT_EQ(v[2], "!");
}

// ═══════════════════════════════════════════════════════════════
// Growth behavior
// ═══════════════════════════════════════════════════════════════

TEST(VectorTest, CapacityGrowth) {
    ministl::vector<int> v;
    // Push many elements and verify capacity grows geometrically
    for (int i = 0; i < 1000; ++i) {
        v.push_back(i);
    }
    EXPECT_EQ(v.size(), 1000u);
    // Capacity should be power-of-2 growth: 1024
    EXPECT_GE(v.capacity(), 1000u);
    EXPECT_LE(v.capacity(), 2048u);  // reasonable upper bound
}

// ═══════════════════════════════════════════════════════════════
// Edge cases
// ═══════════════════════════════════════════════════════════════

TEST(VectorTest, EmptyVectorOperations) {
    ministl::vector<int> v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.begin(), v.end());
    EXPECT_EQ(v.rbegin(), v.rend());
    EXPECT_NO_THROW(v.clear());
}

TEST(VectorTest, SingleElement) {
    ministl::vector<int> v{42};
    EXPECT_EQ(v.front(), 42);
    EXPECT_EQ(v.back(), 42);
    EXPECT_EQ(v.front(), v.back());
    v.pop_back();
    EXPECT_TRUE(v.empty());
}
