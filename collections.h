#ifndef __COLLECTIONS_H_INCLUDED__
#define __COLLECTIONS_H_INCLUDED__

// TODO: Forward / Backward iterator?
// TODO: Tests
// TODO: More datastructures
// TODO: Default values for Iterable
// TODO: References

// size_t
#include <cstddef>

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

// Iterator adapter for filtering out elements in iterators.
// Implements AbstractIterator
template <typename T>
class FilterIterator;

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
     * Undefined behaviour if the Iterator is not in a valid position.
     */
    using Iterable<T>::next;
    void next() { data = next(data); }

    /*
     * Move this iterator to the previous value.
     * Undefined behaviour if the Iterator is not in a valid position.
     */
    using Iterable<T>::prev;
    void prev() { data = prev(data); }

    /*
     * Determine if the iterator has a value.
     */
    using Iterable<T>::valid;
    bool valid() { return valid(data); }

    /*
     * Get the value contained in the iterator.
     * Undefined behaviour if the Iterator is not in a valid position.
     * TODO: consider * and -> operators
     */
    using Iterable<T>::value;
    T value() { return value(data); }

    /*
     * Return a iterator that maps map_function to every value in the iterator.
     */
    template <typename N>
    MapIterator<T, N> map(N (*map_function)(T)) {
        return MapIterator<T, N>(iterable, data, map_function);
    }

    /*
     * Return a iterator that ignores elements where filter_function is false.
     */
    FilterIterator<T> filter(bool (*filter_function)(T)) {
        return FilterIterator<T>(iterable, data, filter_function);
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
class Iterator : public AbstractIterator<T> {
public:
    // Default constructor
    using AbstractIterator<T>::AbstractIterator;

    // Default data fields
    using AbstractIterator<T>::iterable;
    using AbstractIterator<T>::data;

    // Default operators
    using AbstractIterator<T>::next;
    using AbstractIterator<T>::prev;
    using AbstractIterator<T>::valid;
    using AbstractIterator<T>::value;
    using AbstractIterator<T>::map;
    using AbstractIterator<T>::filter;

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
        AbstractIterator<T>(this, (T*)data),
        iterable(iterable),
        data(data),
        map_function(map_function) {}

    // Default operators
    using AbstractIterator<T>::next;
    using AbstractIterator<T>::prev;
    using AbstractIterator<T>::valid;
    using AbstractIterator<T>::value;
    using AbstractIterator<T>::map;
    using AbstractIterator<T>::filter;

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
class FilterIterator : public AbstractIterator<T> {
public:
    FilterIterator(Iterable<T>* iterable, T* data, bool (*filter_function)(T)) :
        AbstractIterator<T>(iterable, data),
        filter_function(filter_function) {
            // Make sure iterator is in a valid spot
            if (iterable->valid(data) && !filter_function(value(data))) {
                next();
            }
    }

    // Default data fields
    using AbstractIterator<T>::iterable;
    using AbstractIterator<T>::data;

    // Default operators
    using AbstractIterator<T>::next;
    using AbstractIterator<T>::prev;
    using AbstractIterator<T>::valid;
    using AbstractIterator<T>::value;
    using AbstractIterator<T>::map;
    using AbstractIterator<T>::filter;

    Iterator<T> iter() {
        return Iterator<T>(this, data);
    }

    Iterator<T> iter_end() {
        return iter();
    }

    T* next(T* data) {
        do {
            data = iterable->next(data);
        } while (iterable->valid(data) && !filter_function(value(data)));

        return data;
    }

    T* prev(T* data) {
        do {
            data = iterable->prev(data);
        } while (iterable->valid(data) && !filter_function(value(data)));

        return data;
    }

    bool valid(T* data) {
        return iterable->valid(data);
    }

    T value(T* data) {
        return iterable->value(data);
    }

    /*
     * Function used to filter elements of old Iterator
     */
    bool (*filter_function)(T);
};

template <typename T>
class ArrayList : Iterable<T> {
public:
    ArrayList();

    ~ArrayList();

    void append(T value);

    void reserve(size_t new_capacity);

    void clear();

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

    ~Node() {
        if (next) delete next;
    }
};

template <typename T>
class LinkedList : Iterable<T> {
public:
    LinkedList();

    ~LinkedList();

    void append(T value);

    void remove(Iterator<T> iterator);

    void clear();

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
    size_t size;
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
ArrayList<T>::~ArrayList() {
    if (data) delete[] data;
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
void ArrayList<T>::clear() {
    size = 0;
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
    size(0) {}

template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

template <typename T>
void LinkedList<T>::append(T value) {
    Node<T>* new_node = new Node<T>();
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
void LinkedList<T>::remove(Iterator<T> iterator) {
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
    delete node;
    size--;
}

template <typename T>
void LinkedList<T>::clear() {
    if (head) delete head;
    size = 0;
    head = nullptr;
    tail = nullptr;
}

template <typename T>
size_t LinkedList<T>::len() {
    return size;
}

template <typename T>
size_t LinkedList<T>::capacity() {
    return size;
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
