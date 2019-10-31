
#ifndef __COLLECTIONS_H_INCLUDED__
#define __COLLECTIONS_H_INCLUDED__

// size_t
#include <cstring>

// Interface for implementing iterators on collecitons
template <typename T>
class Iterable;

// Iterator used to iterate over a collection
template <typename T>
class Iterator;

// Iterator adapter for mapping functions on iterators
template <typename F, typename T>
class MapIterator;

// Generic arraylist
template <typename T>
class ArrayList;

// Simple node used in linked lists
template <typename T>
struct Node;

// Amortized linked list (might remove amortized part)
template <typename T>
class LinkedList;

template <typename T>
class Iterator : Iterable<T> {
public:
    Iterator(Iterable<T>* iterable, T* data);

    // Convenience functions

    void next();

    void prev();

    bool valid();

    T value();

    template <typename N>
    MapIterator<T, N> map(N (*map_function)(T)) {
        return MapIterator<T, N>(iterable, data, map_function);
    }

    Iterable<T>* iterable;
    T* data;

    Iterator<T> iter() {
        return iterable->iter();
    }

    Iterator<T> iter_end() {
        return iterable->iter_end();
    }

    Iterator<T> next(const Iterator<T>* it) {
        return iterable->next(it);
    }

    Iterator<T> prev(const Iterator<T>* it) {
        return iterable->prev(it);
    }

    bool valid(const Iterator<T>* it) {
        return iterable->valid(it);
    }

    T value(Iterator<T>* it) {
        return iterable->value(it);
    }
};

template <typename F, typename T>
class MapIterator : public Iterable<T> {
public:
    MapIterator(Iterable<F>* iterable, F* data, T (*map_function)(F)) :
    iterable(iterable),
    data(data),
    map_function(map_function) {}

    // Convenience functions

    void next() {
        auto temp = Iterator<F>(iterable, data);
        data = iterable->next(&temp).data;
    }

    void prev() {
        auto temp = Iterator<F>(iterable, data);
        iterable->prev(&temp);
    }

    bool valid() {
        auto temp = Iterator<F>(iterable, data);
        return iterable->valid(&temp);
    }

    T value() {
        auto temp_it = Iterator<F>(iterable, data);
        T temp_value = map_function(iterable->value(&temp_it));
        return temp_value;
    }

    template <typename N>
    MapIterator<T, N> map(N (*map_function)(T)) {
        return MapIterator<T, N>((Iterable<T>*)iterable, (T*)data, map_function);
    }

    Iterable<F>* iterable;
    F* data;
    T (*map_function)(F);

    Iterator<T> iter() {
        return Iterator<T>(this, (T*)data);
    }

    Iterator<T> iter_end() {
        // TODO: fix?
        return iter();
    }

    Iterator<T> next(const Iterator<T>* it) {
        auto itt = iterable->next((Iterator<F>*) it);
        return Iterator<T>(this, (T*)itt.data);
    }

    Iterator<T> prev(const Iterator<T>* it) {
        return iter().prev(it);
    }

    bool valid(const Iterator<T>* it) {
        return iter().valid(it);
    }

    T value(Iterator<T>* it) {
        T temp = map_function(iterable->value((Iterator<F>*)it)); 
        std::cout << temp << std::endl;
        return temp;
    }
};

template <typename T>
class Iterable {
public:
    /*
     * Return a Iterator to to the object at the beginning.
     */
    virtual Iterator<T> iter() = 0;

    /*
     * Return a Iterator to to the object at the end.
     */
    virtual Iterator<T> iter_end() = 0;

    /*
     * Get next iterator from current.
     */
    virtual Iterator<T> next(const Iterator<T>* it) = 0;

    /*
     * Get previous iterator from current.
     */
    virtual Iterator<T> prev(const Iterator<T>* it) = 0;

    /*
     * Determine if the iterator has a value;
     */
    virtual bool valid(const Iterator<T>* it) = 0;

    virtual T value(Iterator<T>* it) = 0;
};

template <typename T>
class ArrayList : Iterable<T> {
public:
    ArrayList();

    void append(T value);

    void reserve(size_t new_capacity);

    size_t len();

    size_t capacity();

    T operator[](size_t index);

    Iterator<T> iter();

    Iterator<T> iter_end();

    Iterator<T> next(const Iterator<T>* iterator);

    Iterator<T> prev(const Iterator<T>* iterator);

    bool valid(const Iterator<T>* it);

    T value(Iterator<T>* it) {
        return *it->data;
    }

private:
    T* data;
    size_t size;
    size_t max_size;
};

template <typename T>
struct Node {
    T value;
    Node<T>* next;
    Node<T>* prev;
};

template <typename T>
class LinkedList : Iterable<T> {
public:
    LinkedList();

    void append(T value);

    void remove(Iterator<T>& iterator);

    void reserve(size_t new_capacity);

    size_t len();

    size_t capacity();

    Iterator<T> iter();

    Iterator<T> iter_end();

    Iterator<T> next(const Iterator<T>* iterator);

    Iterator<T> prev(const Iterator<T>* iterator);

    bool valid(const Iterator<T>* iterator);

    T value(Iterator<T>* it) {
        return *it->data;
    }

    Node<T>* head;
    Node<T>* tail;
private:
    Node<T>* amortized;
    Node<T>* buffer;
    size_t size;
    size_t max_size;
};

//
// Implementation
//

template <typename T>
Iterator<T>::Iterator(Iterable<T>* iterable, T* data) :
    iterable(iterable),
    data(data) {}

template <typename T>
void Iterator<T>::next() {
    Iterator<T> temp = iterable->next(this);
    iterable = temp.iterable;
    data = temp.data;
};

template <typename T>
void Iterator<T>::prev() {
    Iterator<T> temp = iterable->prev(this);
    iterable = temp.iterable;
    data = temp.data;
}

template <typename T>
bool Iterator<T>::valid() {
    return iterable->valid(this);
}

template <typename T>
T Iterator<T>::value() {
    return iterable->value(this);
}

template <typename T>
ArrayList<T>::ArrayList() :
    size(0),
    max_size(0),
    data(nullptr) {
        reserve(1);
}

template <typename T>
void ArrayList<T>::append(T value) {
    if (size == max_size) reserve(max_size * 2);
    data[size++] = value;
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
size_t ArrayList<T>::len() {
    return size;
}

template <typename T>
size_t ArrayList<T>::capacity() {
    return max_size;
}

template <typename T>
T ArrayList<T>::operator[](size_t index) {
    return data[index];
}

template <typename T>
Iterator<T> ArrayList<T>::iter() {
    return Iterator<T>(this, data);
}

template <typename T>
Iterator<T> ArrayList<T>::iter_end() {
    return Iterator<T>(this, data + size);
}

template <typename T>
Iterator<T> ArrayList<T>::next(const Iterator<T>* iterator) {
    return Iterator<T>(this, iterator->data + 1);
}

template <typename T>
Iterator<T> ArrayList<T>::prev(const Iterator<T>* iterator) {
    return Iterator<T>(this, iterator->data - 1);
}

template <typename T>
bool ArrayList<T>::valid(const Iterator<T>* it) {
    return data <= it->data && it->data < data + size;
}

template <typename T>
LinkedList<T>::LinkedList() :
    head(nullptr),
    tail(nullptr),
    amortized(nullptr),
    buffer(nullptr),
    size(0),
    max_size(0) {
    reserve(1);
}

template <typename T>
void LinkedList<T>::append(T value) {
    if (size == max_size) reserve(max_size * 2);
    Node<T>* new_node = amortized;
    amortized = amortized->next;
    *new_node = { value, nullptr, nullptr };
    if (size == 0) {
        head = new_node;
        tail = new_node;
    } else {
        new_node->prev = tail;
        tail->next = new_node;
        tail = new_node;
    }
    size++;
}

template <typename T>
void LinkedList<T>::remove(Iterator<T>& iterator) {
    Node<T>* node = (Node<T>*)iterator.data;
    iterator.next();
    if (node->next == nullptr) {
        tail = node->prev;
    } else {
        node->next->prev = node->prev;
    }
    if (node->prev == nullptr) {
        head = node->next;
    } else {
        node->prev->next = node->next;
    }
    node->next = amortized;
    amortized = node;
    size--;
}

template <typename T>
void LinkedList<T>::reserve(size_t new_capacity) {
    if (new_capacity <= max_size) return;
    max_size = new_capacity;
    Node<T>* new_buffer = new Node<T>[max_size];

    // Copy current list to new buffer
    Iterator<T> it = iter();
    size_t i = 0;
    while (it.valid()) {
        new_buffer[i].value = it.value();
        if (i > 0) {
            new_buffer[i].prev = new_buffer + i - 1;
        }
        if (i < size - 1) {
            new_buffer[i].next = new_buffer + i + 1;
        }
        it.next();
        i++;
    }

    // Create amortized list
    for (size_t i = size; i < max_size; i++) {
        new_buffer[i].next = amortized;
        amortized = new_buffer + i;
    }

    delete[] buffer;
    buffer = new_buffer;
    if (size) {
        head = buffer;
        tail = buffer + size - 1;
    }
}

template <typename T>
size_t LinkedList<T>::len() {
    return size;
}

template <typename T>
size_t LinkedList<T>::capacity() {
    return max_size;
}

template <typename T>
Iterator<T> LinkedList<T>::iter() {
    return Iterator<T>(this, (T*)head);
}

template <typename T>
Iterator<T> LinkedList<T>::iter_end() {
    return Iterator<T>(this, (T*)tail);
}

template <typename T>
Iterator<T> LinkedList<T>::next(const Iterator<T>* iterator) {
    return Iterator<T>(this, (T*)((Node<T>*)iterator->data)->next);
}

template <typename T>
Iterator<T> LinkedList<T>::prev(const Iterator<T>* iterator) {
    return Iterator<T>(this, (T*)((Node<T>*)iterator->data)->prev);
}

template <typename T>
bool LinkedList<T>::valid(const Iterator<T>* iterator) {
    return iterator->data != nullptr;
}

#endif
