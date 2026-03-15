#pragma once

#include <algorithm>
#include <cstddef>
#include <stdexcept>

#include <type_traits>

namespace ministl {

template <typename T>
class vector {
public:
    // ─── 类型别名（STL 标准接口需要这些）─────────────────
    using value_type      = T;
    using size_type       = std::size_t;
    using reference       = T&;
    using const_reference = const T&;
    using pointer         = T*;
    using const_pointer   = const T*;
    using iterator        = pointer;
    using const_iterator  = const_pointer;

    // ─── 构造函数 ─────────────────────────────────────────

    // 默认构造
    vector() : data_(nullptr), size_(0), capacity_(0) {}

    // fill 构造: vector<int> v(5, 42)
    explicit vector(size_type count, const T& value = T()) {
        reserve(count);
        for (size_type i = 0; i < count; i++)
        {
            push_back(value);
        }
    }

    vector(std::initializer_list<T> init) {
        reserve(init.size());
        for (const T& val: init)
        {
            push_back(val);
        }
    }

template <typename InputIt,
          typename = std::enable_if_t<!std::is_integral_v<InputIt>>>
    vector(InputIt first, InputIt last) {
        for (auto it = first; it != last; ++it)
        {
            push_back(*it);
        }  
    }

    // 拷贝构造
    vector(const vector& other) {
        reserve(other.size());
        for (size_type i = 0; i < other.size(); i++)
        {
            push_back(other[i]);
        }
    }

    // ─── 析构函数 ─────────────────────────────────────────

    ~vector() {
        delete[] data_;
    }

    // ─── 赋值 ─────────────────────────────────────────────

    // 拷贝赋值
    vector& operator=(const vector& other) {
        if (this!=&other)
        {
            clear();
            reserve(other.size());
            for (size_type i = 0; i < other.size(); i++)
            {
                push_back(other[i]);
            }
        }
        return *this;
    }

    // ─── 元素访问 ─────────────────────────────────────────

    // 不检查越界（STL 标准行为）
    reference operator[](size_type pos) { return data_[pos]; }
    const_reference operator[](size_type pos) const { return data_[pos]; }

    // 检查越界
    reference at(size_type pos) {
        if (pos >= size_) throw std::out_of_range("ministl::vector::at");
        return data_[pos];
    }
    const_reference at(size_type pos) const {
        if (pos >= size_) throw std::out_of_range("ministl::vector::at");
        return data_[pos];
    }

    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }

    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }

    pointer data() noexcept { return data_; }
    const_pointer data() const noexcept { return data_; }

    // ─── 迭代器 ───────────────────────────────────────────

    iterator begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }

    iterator end() noexcept { return data_ + size_; }
    const_iterator end() const noexcept { return data_ + size_; }

    const_iterator cbegin() const noexcept { return data_; }
    const_iterator cend() const noexcept { return data_ + size_; }

    // ─── 容量 ─────────────────────────────────────────────

    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }
    size_type capacity() const noexcept { return capacity_; }

    void reserve(size_type new_cap) {
        if(new_cap <= capacity_)
        {
            return;
        }

        T* nptr;
        nptr = new T[new_cap];
        for (size_type i = 0; i < size_; i++)
        {
            nptr[i] = data_[i];
        }
        delete[] data_;
        data_ = nptr;
        capacity_ = new_cap;
    }

    // ─── 修改器 ───────────────────────────────────────────

    void clear() noexcept {
        size_ = 0;
    }

    void push_back(const T& value) {
        if(size_ == capacity_)
        {
            grow();
        }
        data_[size_] = value;
        size_++;
    }

    void pop_back() {
        size_--;
    }

    iterator insert(const_iterator pos, const T& value) {
        // TODO
        return nullptr;
    }

    iterator erase(const_iterator pos) {
        // TODO
        return nullptr;
    }

    iterator erase(const_iterator first, const_iterator last) {
        // TODO
        return nullptr;
    }

    void swap(vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    // ─── 比较运算符 ───────────────────────────────────────

    friend bool operator==(const vector& lhs, const vector& rhs) {
        if (lhs.size_ != rhs.size_) return false;
        return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    friend bool operator!=(const vector& lhs, const vector& rhs) {
        return !(lhs == rhs);
    }

private:
    pointer   data_     = nullptr;
    size_type size_     = 0;
    size_type capacity_ = 0;

    // ─── 内部辅助函数 ─────────────────────────────────────

    void grow() {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
};

}  // namespace ministl