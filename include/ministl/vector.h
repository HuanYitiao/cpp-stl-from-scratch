#pragma once

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>

namespace ministl {

template <typename T, typename Allocator = std::allocator<T>>
class vector {
public:
    // ─── Type aliases ────────────────────────────────────────
    using value_type      = T;
    using allocator_type  = Allocator;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference       = T&;
    using const_reference = const T&;
    using pointer         = T*;
    using const_pointer   = const T*;

    // ─── Iterator (raw pointer) ──────────────────────────────
    using iterator               = pointer;
    using const_iterator         = const_pointer;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // ─── Constructors ────────────────────────────────────────

    vector() noexcept(noexcept(Allocator()))
        : data_(nullptr), size_(0), capacity_(0), alloc_() {}

    explicit vector(size_type count, const T& value = T(),
                    const Allocator& alloc = Allocator())
        : alloc_(alloc) {
        allocate_and_fill(count, value);
    }

    template <typename InputIt,
              typename = std::enable_if_t<!std::is_integral_v<InputIt>>>
    vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())
        : alloc_(alloc) {
        allocate_and_copy(first, last);
    }

    vector(std::initializer_list<T> init, const Allocator& alloc = Allocator())
        : alloc_(alloc) {
        allocate_and_copy(init.begin(), init.end());
    }

    // Copy constructor
    vector(const vector& other)
        : alloc_(std::allocator_traits<Allocator>::select_on_container_copy_construction(
              other.alloc_)) {
        allocate_and_copy(other.begin(), other.end());
    }

    // Move constructor
    vector(vector&& other) noexcept
        : data_(other.data_),
          size_(other.size_),
          capacity_(other.capacity_),
          alloc_(std::move(other.alloc_)) {
        other.data_     = nullptr;
        other.size_     = 0;
        other.capacity_ = 0;
    }

    // ─── Destructor ──────────────────────────────────────────

    ~vector() {
        clear();
        deallocate();
    }

    // ─── Assignment ──────────────────────────────────────────

    vector& operator=(const vector& other) {
        if (this != &other) {
            vector tmp(other);
            swap(tmp);
        }
        return *this;
    }

    vector& operator=(vector&& other) noexcept {
        if (this != &other) {
            clear();
            deallocate();
            data_     = other.data_;
            size_     = other.size_;
            capacity_ = other.capacity_;
            alloc_    = std::move(other.alloc_);
            other.data_     = nullptr;
            other.size_     = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    vector& operator=(std::initializer_list<T> ilist) {
        vector tmp(ilist, alloc_);
        swap(tmp);
        return *this;
    }

    // ─── Element access ──────────────────────────────────────

    reference operator[](size_type pos) { return data_[pos]; }
    const_reference operator[](size_type pos) const { return data_[pos]; }

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

    // ─── Iterators ───────────────────────────────────────────

    iterator begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }
    const_iterator cbegin() const noexcept { return data_; }

    iterator end() noexcept { return data_ + size_; }
    const_iterator end() const noexcept { return data_ + size_; }
    const_iterator cend() const noexcept { return data_ + size_; }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

    // ─── Capacity ────────────────────────────────────────────

    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }
    size_type capacity() const noexcept { return capacity_; }

    void reserve(size_type new_cap) {
        if (new_cap <= capacity_) return;
        reallocate(new_cap);
    }

    void shrink_to_fit() {
        if (capacity_ > size_) {
            reallocate(size_);
        }
    }

    // ─── Modifiers ───────────────────────────────────────────

    void clear() noexcept {
        destroy_range(data_, data_ + size_);
        size_ = 0;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) grow();
        std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, value);
        ++size_;
    }

    void push_back(T&& value) {
        if (size_ == capacity_) grow();
        std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, std::move(value));
        ++size_;
    }

    template <typename... Args>
    reference emplace_back(Args&&... args) {
        if (size_ == capacity_) grow();
        std::allocator_traits<Allocator>::construct(
            alloc_, data_ + size_, std::forward<Args>(args)...);
        ++size_;
        return back();
    }

    void pop_back() {
        --size_;
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + size_);
    }

    iterator insert(const_iterator pos, const T& value) {
        auto offset = pos - begin();
        if (size_ == capacity_) grow();
        iterator it = begin() + offset;
        // Shift elements right
        if (it != end()) {
            shift_right(it, end(), 1);
        }
        std::allocator_traits<Allocator>::construct(alloc_, it, value);
        ++size_;
        return it;
    }

    iterator erase(const_iterator pos) {
        iterator it = begin() + (pos - begin());
        std::allocator_traits<Allocator>::destroy(alloc_, it);
        // Shift elements left
        if (it + 1 != end()) {
            shift_left(it + 1, end(), 1);
        }
        --size_;
        return it;
    }

    iterator erase(const_iterator first, const_iterator last) {
        iterator f = begin() + (first - begin());
        iterator l = begin() + (last - begin());
        auto n = std::distance(f, l);
        destroy_range(f, l);
        shift_left(l, end(), n);
        size_ -= static_cast<size_type>(n);
        return f;
    }

    void resize(size_type count, const T& value = T()) {
        if (count < size_) {
            destroy_range(data_ + count, data_ + size_);
            size_ = count;
        } else if (count > size_) {
            reserve(count);
            for (size_type i = size_; i < count; ++i) {
                std::allocator_traits<Allocator>::construct(alloc_, data_ + i, value);
            }
            size_ = count;
        }
    }

    void swap(vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(alloc_, other.alloc_);
    }

    // ─── Comparison operators ────────────────────────────────

    friend bool operator==(const vector& lhs, const vector& rhs) {
        if (lhs.size_ != rhs.size_) return false;
        return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    friend bool operator!=(const vector& lhs, const vector& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const vector& lhs, const vector& rhs) {
        return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                            rhs.begin(), rhs.end());
    }

    friend bool operator<=(const vector& lhs, const vector& rhs) {
        return !(rhs < lhs);
    }

    friend bool operator>(const vector& lhs, const vector& rhs) {
        return rhs < lhs;
    }

    friend bool operator>=(const vector& lhs, const vector& rhs) {
        return !(lhs < rhs);
    }

private:
    pointer   data_;
    size_type size_;
    size_type capacity_;
    Allocator alloc_;

    // ─── Internal helpers ────────────────────────────────────

    void allocate_and_fill(size_type count, const T& value) {
        data_     = alloc_.allocate(count);
        capacity_ = count;
        size_     = count;
        for (size_type i = 0; i < count; ++i) {
            std::allocator_traits<Allocator>::construct(alloc_, data_ + i, value);
        }
    }

    template <typename InputIt>
    void allocate_and_copy(InputIt first, InputIt last) {
        auto count = static_cast<size_type>(std::distance(first, last));
        data_      = alloc_.allocate(count);
        capacity_  = count;
        size_      = count;
        size_type i = 0;
        for (auto it = first; it != last; ++it, ++i) {
            std::allocator_traits<Allocator>::construct(alloc_, data_ + i, *it);
        }
    }

    void deallocate() {
        if (data_) {
            alloc_.deallocate(data_, capacity_);
            data_ = nullptr;
        }
    }

    void destroy_range(pointer first, pointer last) {
        for (auto p = first; p != last; ++p) {
            std::allocator_traits<Allocator>::destroy(alloc_, p);
        }
    }

    void grow() {
        size_type new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
        reallocate(new_cap);
    }

    void reallocate(size_type new_cap) {
        pointer new_data = alloc_.allocate(new_cap);
        // Move-construct existing elements into new storage
        for (size_type i = 0; i < size_; ++i) {
            std::allocator_traits<Allocator>::construct(
                alloc_, new_data + i, std::move(data_[i]));
        }
        destroy_range(data_, data_ + size_);
        deallocate();
        data_     = new_data;
        capacity_ = new_cap;
    }

    void shift_right(iterator pos, iterator old_end, size_type count) {
        for (auto it = old_end - 1; it >= pos; --it) {
            std::allocator_traits<Allocator>::construct(
                alloc_, it + count, std::move(*it));
            std::allocator_traits<Allocator>::destroy(alloc_, it);
        }
    }

    void shift_left(iterator pos, iterator old_end, difference_type count) {
        for (auto it = pos; it != old_end; ++it) {
            *(it - count) = std::move(*it);
        }
    }
};

}  // namespace ministl
