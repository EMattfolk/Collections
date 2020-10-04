#pragma once

template <typename T>
struct Node {
    T value;
    Node<T>* next;
    Node<T>* prev;

    ~Node() {
        if (next) delete next;
    }
};

template <typename T>
class LinkedList {
public:
    LinkedList();

    ~LinkedList();

    void append(T value);

    void clear();

    size_t len();

    size_t capacity();

    Node<T>* head;
    Node<T>* tail;
private:
    size_t size;
};

#include "linkedlist.cpp"
