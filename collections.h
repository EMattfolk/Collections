#ifndef __COLLECTIONS_H_INCLUDED__
#define __COLLECTIONS_H_INCLUDED__

// TODO: Tests
// TODO: More datastructures
// TODO: Default values for Iterable
// TODO: References
// TODO: Better collection interface
// TODO: Fix inheritance bug

#include <cstddef>    // size_t
#include <functional> // Hashing

// Amount of space allocated if not otherwise specified
const size_t COLLECTIONS_DEFAULT_CAPACITY = 32;

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

// HashMap
template <typename K, typename V>
class HashMap;

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
     * The current front position of the Iterable.
     * A pointer is used because some collections do not store
     * data in sequential order, such as linked lists.
     */
    T* data;

    /*
     * The current back position of the Iterable.
     * A pointer is used because some collections do not store
     * data in sequential order, such as linked lists.
     */
    T* data_back;
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
// TODO: constructors
// TODO: const on things
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

    size_t len() const;

    size_t capacity() const;

    bool is_empty() const;

    T& operator[](size_t index) const;

    bool operator==(const ArrayList<T>& other) const;

    bool operator!=(const ArrayList<T>& other) const;

    Iterator<T> iter();

    Iterator<T> iter_end();

    T* next(T* data);

    T* prev(T* data);

    bool valid(T* data);

    T value(T* data) {
        return *data;
    }

private:

    void reserve_default();

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

template <typename K, typename V>
class HashMap {
public:
    HashMap() :
        data(new MapPair[8]),
        hash_data(new HashPair[8]),
        size(0),
        capacity(8) {

    }

    void add(K key, V value) {
        size_t dist = 0;
        size_t hash = get_hash(key);
        size_t pos = desired_pos(hash);

        while (true) {
            if (hash_data[pos].hash == 0) {
                hash_data[pos].hash = hash;
                hash_data[pos].tomb = false;
                data[pos] = { key, value };
                break;
            }

            size_t existing_dist = probe_dist(hash_data[pos].hash, pos);
            if (existing_dist < dist) {
                if (hash_data[pos].tomb) {
                    hash_data[pos].hash = hash;
                    hash_data[pos].tomb = false;
                    data[pos] = { key, value };
                    break;
                }
                std::swap(hash, hash_data[pos].hash);
                std::swap(key, data[pos].key);
                std::swap(value, data[pos].value);
                dist = existing_dist;
            }

            dist++;
            pos = (pos + 1) % capacity;
        }

        size++;
    }

    bool erase(const K& key) {
        int index = index_of(key);
        if (index == -1) return false;

        data[index].~MapPair();
        hash_data[index].tomb = true;
        size--;
        return true;
    }

    V& operator[](const K& key) {
        return data[index_of(key)].value;
    }

    bool contains(K key) {
        return index_of(key) != -1;
    }

private:

    int index_of(K key) {
        size_t hash = get_hash(key);
        size_t pos = desired_pos(hash);
        size_t dist = 0;

        while (true) {
            if (hash_data[pos].hash == 0) {
                return -1;
            } else if (dist > probe_dist(hash_data[pos].hash, pos)) {
                return -1;
            } else if (//!hash_data[pos].tomb &&
                    hash_data[pos].hash == hash &&
                    data[pos].key == key) {
                return pos;
            }
            dist++;
            pos = (pos + 1) % capacity;
        }
    }

    size_t probe_dist(size_t hash, size_t pos) {
        size_t desired = desired_pos(hash);
        if (desired > pos)
            return capacity + pos - desired;
        return pos - desired;
    }

    size_t desired_pos(size_t hash) {
        return hash % capacity;
    }

    size_t get_hash(K key) {
        // Make sure hash cannot be 0
        size_t hash = hasher(key);
        if (!hash) return 1;
        return hash;
    }

    struct MapPair {
        K key;
        V value;
    };

    struct HashPair {
        size_t hash;
        bool tomb;
    };

    MapPair* data;
    HashPair* hash_data;
    std::hash<K> hasher;
    size_t size;
    size_t capacity;
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
    data(nullptr),
    size(0),
    max_size(0) {}

template <typename T>
ArrayList<T>::ArrayList(const ArrayList<T>& other) : ArrayList<T>() {
    reserve(other.max_size);
    size = other.size;
    for (size_t i = 0; i < size; i++) {
        data[i] = other[i];
    }
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
Iterator<T> ArrayList<T>::iter() {
    return Iterator<T>(this, data);
}

template <typename T>
Iterator<T> ArrayList<T>::iter_end() {
    return Iterator<T>(this, data + size - 1);
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
void ArrayList<T>::reserve_default() {
    if (max_size == 0)
        reserve(COLLECTIONS_DEFAULT_CAPACITY);
    else
        reserve(max_size * 2);
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
