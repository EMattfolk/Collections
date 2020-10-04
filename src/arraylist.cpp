#pragma once

#include "arraylist.h"

template <typename T>
ArrayList<T>::ArrayList() :
    data(nullptr),
    size(0),
    max_size(0) {}

template <typename T>
ArrayList<T>::ArrayList(const ArrayList<T>& other) : ArrayList<T>() {
    *this = other;
}

template <typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& other) {
    reserve(other.max_size);
    size = other.size;
    for (size_t i = 0; i < size; i++) {
        data[i] = other[i];
    }

    return *this;
}

template <typename T>
ArrayList<T>::~ArrayList() {
    if (data) delete[] data;
}

template <typename T>
void ArrayList<T>::append(T value) {
    if (size == max_size) reserve_default();
    data[size++] = value;
}

template <typename T>
void ArrayList<T>::insert(size_t index, T value) {
    if (size == max_size) reserve_default();
    for (size_t i = size; i > index; i--) {
        data[i] = data[i - 1];
    }
    data[index] = value;
    size++;
}

template <typename T>
T ArrayList<T>::pop() {
    T temp = data[size - 1];
    remove(size - 1);
    return temp;
}

template <typename T>
void ArrayList<T>::remove(size_t index) {
    data[index].~T();
    for (size_t i = index; i < size - 1; i++) {
        data[i] = data[i + 1];
    }
    size--;
}

template <typename T>
void ArrayList<T>::reserve(size_t new_capacity) {
    if (new_capacity <= max_size) return;
    max_size = new_capacity;
    T* new_buffer = new T[max_size];

    for (size_t i = 0; i < size; i++) {
        new_buffer[i] = data[i];
    }

    delete[] data;
    data = new_buffer;
}

template <typename T>
void ArrayList<T>::clear() {
    size = 0;
}

template <typename T>
void ArrayList<T>::sort() {
    quick_sort(0, size);
}

template <typename T>
size_t ArrayList<T>::len() const {
    return size;
}

template <typename T>
size_t ArrayList<T>::capacity() const {
    return max_size;
}

template <typename T>
bool ArrayList<T>::is_empty() const {
    return size == 0;
}

template <typename T>
T& ArrayList<T>::operator[](size_t index) const {
    return data[index];
}

template <typename T>
bool ArrayList<T>::operator==(const ArrayList<T>& other) const {
    if (size != other.size) return false;
    for (size_t i = 0; i < size; i++) {
        if (data[i] != other[i]) return false;
    }
    return true;
}

template <typename T>
bool ArrayList<T>::operator!=(const ArrayList<T>& other) const {
    return !(*this == other);
}

template <typename T>
void ArrayList<T>::reserve_default() {
    if (max_size == 0)
        reserve(COLLECTIONS_DEFAULT_CAPACITY);
    else
        reserve(max_size * 2);
}

template <typename T>
void ArrayList<T>::quick_sort(size_t start, size_t end) {
    if (end - start < 2) return;

    std::swap(data[(start + end) / 2], data[end - 1]);
    T& pivot = data[end - 1];
    size_t low = start;
    for (size_t i = start; i < end - 1; i++) {
        if (data[i] < pivot) {
            std::swap(data[i], data[low]);
            low++;
        }
    }

    std::swap(data[low], data[end - 1]);

    quick_sort(start, low);
    quick_sort(low + 1, end);
}

