#pragma once

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
