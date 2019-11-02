
#ifndef __COLLECTIONS_H_INCLUDED__
#define __COLLECTIONS_H_INCLUDED__

// size_t
#include <cstring>

// Interface for implementing Iterators on collections.
template <typename T>
class Iterable;

// AbstractIterator
// Implements Iterable
//
// Used to create different types of Iterators.
template <typename T>
class AbstractIterator;

// Iterator
// Implements AbstractIterator
//
// Used to iterate over a collection. Values are retrieved lazily.
template <typename T>
class Iterator;

// Iterator adapter for mapping functions on iterators.
// Implements AbstractIterator
template <typename F, typename T>
class MapIterator;

// ArrayList
// Implements Iterable
template <typename T>
class ArrayList;

// Simple node used in linked lists.
template <typename T>
struct Node;

// LinkedList
// Implements Iterable
template <typename T>
class LinkedList;

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
     * Get next data from current data.
     */
    virtual T* next(T* data) = 0;

    /*
     * Get previous data from current data.
     */
    virtual T* prev(T* data) = 0;

    /*
     * Determine if the data pointed to is valid.
     */
    virtual bool valid(T* data) = 0;

    /*
     * Get value associated with data
     */
    virtual T value(T* it) = 0;
};

template <typename T>
class AbstractIterator : public Iterable<T> {
public:
    /*
     * Construct a AbstractIterator from a iterable and a pointer to its data.
     */
    AbstractIterator(Iterable<T>* iterable, T* data);

    /*
     * Move this iterator to the next value.
     * Default behaviour is to delegate task to layer below iterator.
     */
    virtual void next() { data = iterable->next(data); }

    /*
     * Move this iterator to the previous value.
     * Default behaviour is to delegate task to layer below iterator.
     */
    virtual void prev() { data = iterable->next(data); }

    /*
     * Determine if the iterator has a value.
     * Default behaviour is to delegate task to layer below iterator.
     */
    virtual bool valid() { return iterable->valid(data); }

    /*
     * Get the value contained in the iterator.
     * Default behaviour is to delegate task to layer below iterator.
     * TODO: consider * and -> operators
     */
    virtual T value() { return iterable->value(data); }

    /*
     * Return a iterator that maps map_function to every value in the iterator.
     */
    template <typename N>
    MapIterator<T, N> map(N (*map_function)(T)) {
        return MapIterator<T, N>(iterable, data, map_function);
    }

    /*
     * The Iterable on the layer below this one.
     * Since Iterators can be stacked they need a way to interface with
     * its lower levels. This field links to the layer below
     * in a linked list fashion.
     */
    Iterable<T>* iterable;

    /*
     * The current position of the Iterable.
     * A pointer is used because some collections do not store
     * data in sequential order, such as linked lists.
     */
    T* data;
};

template <typename T>
class Iterator : AbstractIterator<T> {
public:
    // Default constructor
    using AbstractIterator<T>::AbstractIterator;

    // Default data fields
    using AbstractIterator<T>::iterable;
    using AbstractIterator<T>::data;

    // Default operators
    using AbstractIterator<T>::map;
    using AbstractIterator<T>::next;
    using AbstractIterator<T>::prev;
    using AbstractIterator<T>::valid;
    using AbstractIterator<T>::value;

    Iterator<T> iter() {
        return iterable->iter();
    }

    Iterator<T> iter_end() {
        return iterable->iter_end();
    }

    T* next(T* data) {
        return iterable->next(data);
    }

    T* prev(T* data) {
        return iterable->prev(data);
    }

    bool valid(T* data) {
        return iterable->valid(data);
    }

    T value(T* data) {
        return iterable->value(data);
    }

};

template <typename F, typename T>
class MapIterator : public AbstractIterator<T> {
public:
    MapIterator(Iterable<F>* iterable, F* data, T (*map_function)(F)) :
        AbstractIterator<T>(nullptr, nullptr),
        iterable(iterable),
        data(data),
        map_function(map_function) {}

    void next() override {
        data = iterable->next(data);
    }

    void prev() override {
        data = iterable->prev(data);
    }

    bool valid() override {
        return iterable->valid(data);
    }

    T value() override {
        return map_function(iterable->value(data));
    }

    template <typename N>
    MapIterator<T, N> map(N (*map_function)(T)) {
        return MapIterator<T, N>(this, (T*)data, map_function);
    }

    Iterator<T> iter() {
        return Iterator<T>(this, (T*)data);
    }

    Iterator<T> iter_end() {
        return iter();
    }

    T* next(T* data) {
        return (T*)iterable->next((F*) data);
    }

    T* prev(T* data) {
        return (T*)iterable->prev((F*) data);
    }

    bool valid(T* data) {
        return iterable->valid((F*) data);
    }

    T value(T* data) {
        return map_function(iterable->value((F*)data));
    }

    /*
     * Iterable of old type
     */
    Iterable<F>* iterable;

    /*
     * Data of old type
     */
    F* data;

    /*
     * Function used to map elements of old Iterator
     */
    T (*map_function)(F);
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

    T* next(T* data);

    T* prev(T* data);

    bool valid(T* data);

    T value(T* data) {
        return *data;
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

    T* next(T* data);

    T* prev(T* data);

    bool valid(T* data);

    T value(T* data) {
        return *data;
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
AbstractIterator<T>::AbstractIterator(Iterable<T>* iterable, T* data) :
    iterable(iterable),
    data(data) {}

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
T* ArrayList<T>::next(T* data) {
    return data + 1;
}

template <typename T>
T* ArrayList<T>::prev(T* data) {
    return data - 1;
}

template <typename T>
bool ArrayList<T>::valid(T* data) {
    return this->data <= data && data < this->data + size;
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
T* LinkedList<T>::next(T* data) {
    return (T*)((Node<T>*)data)->next;
}

template <typename T>
T* LinkedList<T>::prev(T* data) {
    return (T*)((Node<T>*)data)->prev;
}

template <typename T>
bool LinkedList<T>::valid(T* data) {
    return data != nullptr;
}

#endif
