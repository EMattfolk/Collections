#pragma once

template <typename T>
class ArrayList {
public:
    ArrayList();
    ArrayList(const ArrayList<T>& other);
    ArrayList<T>& operator=(const ArrayList<T>& other);

    virtual ~ArrayList();

    void append(T value);

    void insert(size_t index, T value);

    T pop();

    void remove(size_t index);

    void reserve(size_t new_capacity);

    void clear();

    void sort();

    size_t len() const;

    size_t capacity() const;

    bool is_empty() const;

    T& operator[](size_t index) const;

    bool operator==(const ArrayList<T>& other) const;

    bool operator!=(const ArrayList<T>& other) const;

private:

    void reserve_default();

    void quick_sort(size_t start, size_t end);

    T* data;
    size_t size;
    size_t max_size;
};

#include "arraylist.cpp"
