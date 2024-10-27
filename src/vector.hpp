#pragma once

#include <algorithm>
#include <cstddef>
#include <utility>
#include <compare>

#include <cassert>

using difference_type = ptrdiff_t;

template <typename T>
class Vector {
public:
  // Constructors & destructor
  Vector() = default;

  Vector(size_t count, const T& value = T()) : arr_(allocate(count)), sz_(count), cap_(count) {
    size_t i = 0;
    try {
      for (; i < sz_; ++i)
        new (arr_ + i) T(value);
    } catch (...) {
      for (size_t j = 0; j < i; ++j)
        arr_[j].~T();
      operator delete(arr_);
      throw;
    }
  }

  Vector(const Vector<T>& other) : arr_(allocate(other.sz_)), sz_(other.sz_), cap_(other.sz_) {
    size_t i = 0;
    try {
      for (; i < sz_; ++i)
        new (arr_ + i) T(other.arr_[i]);
    } catch (...) {
      for (size_t j = 0; j < i; ++j)
        arr_[j].~T();
      operator delete(arr_);
      throw;
    }
  }

  Vector(Vector<T>&& other) : arr_(other.arr_), sz_(other.sz_), cap_(other.cap_) {
    other.arr_ = nullptr;
    other.sz_ = 0;
    other.cap_ = 0;
  }

  ~Vector() {
    if (sz_) {
      for (size_t i = 0; i < sz_; ++i)
        arr_[i].~T();
    }
    if (arr_)
      operator delete(arr_);
  }


  void swap(Vector<T>& other) {
    std::swap(arr_, other.arr_);
    std::swap(sz_, other.sz_);
    std::swap(cap_, other.cap_);
  }

  Vector<T>& operator=(Vector<T> other) {
    swap(other);
    return *this;
  }

  size_t size() const {
    return sz_;
  }

  size_t capacity() const {
    return cap_;
  }

  bool empty() const {
    return !sz_;
  }

  const T& operator[](size_t idx) const {
    return arr_[idx];
  }

  T& operator[](size_t idx) {
    return arr_[idx];
  }

  const T& back() const {
    assert(sz_);
    return arr_[sz_ - 1];
  }

  T& back() {
    assert(sz_);
    return arr_[sz_ - 1];
  }

  void pop_back() {
    if (sz_) {
      --sz_;
      arr_[sz_].~T();
    }
  }

  void push_back(const T& value) {
    if (sz_ == cap_)
      reserve(std::max(cap_ * 2, 1ul));
    new (arr_ + sz_) T(value);
    ++sz_;
  }

  template <typename... Args>
  void emplace_back(Args&&... args) {
    if (sz_ == cap_)
      reserve(std::max(cap_ * 2, 1ul));
    new (arr_ + sz_) T(std::forward<Args>(args)...);
    ++sz_;
  }

  void reserve(size_t new_size) {
    if (cap_ >= new_size) return;
    T* new_arr = allocate(new_size);
    assert(new_arr);
    for (size_t i = 0; i < sz_; ++i)
      new (new_arr + i) T(std::move(arr_[i]));
    operator delete(arr_);
    arr_ = new_arr;
    cap_ = new_size;
  }

public: // basic iterators
  // P. s. Зря я туда полез
  template<bool C>
  class base_iterator {
  private:
    std::conditional_t<C, const T*, T*> ptr;

    base_iterator(T* ptr) : ptr(ptr) {}

    friend Vector<T>;

  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using pointer_type = std::conditional_t<C, const T*, T*>;
    using reference_type = std::conditional_t<C, const T&, T&>;

    base_iterator() = default;
    base_iterator(const base_iterator<C>&) = default;
    base_iterator& operator=(const base_iterator<C>&) = default;

    reference_type operator*() const {
      return *ptr;
    }

    pointer_type operator->() const {
      return ptr;
    }

    operator base_iterator<true>() const { // cast from non-const to const
      return *this;
    }

    base_iterator<C>& operator++() {
      ++ptr;
      return *this;
    }

    base_iterator<C> operator++(int) {
      base_iterator<C> copy = *this;
      ++ptr;
      return copy;
    }

    base_iterator<C>& operator--() {
      --ptr;
      return *this;
    }

    base_iterator<C> operator--(int) {
      base_iterator<C> copy = *this;
      --ptr;
      return copy;
    }

    base_iterator<C>& operator+=(difference_type diff) {
      ptr += diff;
      return *this;
    }

    base_iterator<C> operator+(difference_type diff) const {
      base_iterator<C> copy = *this;
      copy += diff;
      return copy;
    }

    base_iterator<C>& operator-=(difference_type diff) {
      ptr -= diff;
      return *this;
    }

    base_iterator<C> operator-(difference_type diff) const {
      base_iterator<C> copy = *this;
      copy -= diff;
      return copy;
    }

    difference_type operator-(const base_iterator<C>& rhs) const {
      return ptr - rhs.ptr;
    }

    std::strong_ordering operator<=>(const base_iterator<C>& other) const {
      return (ptr - other.ptr) <=> 0;
    }

    bool operator==(const base_iterator<C>& other) const = default;
  };

  using iterator = base_iterator<false>;
  using const_iterator = base_iterator<true>;
  
  iterator begin() const {
    return { &arr_[0] };
  }
  iterator end() const {
    return { &arr_[sz_] };
  }

  const_iterator cbegin() const {
    return { &arr_[0] };
  }
  const_iterator cend() const {
    return { &arr_[sz_] };
  }

private:
  static T* allocate(size_t count) {
    return reinterpret_cast<T*>(operator new(sizeof(T) * count));
  }

private:
  T* arr_ = nullptr;
  size_t sz_ = 0;
  size_t cap_ = 0;
};
