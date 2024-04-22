#include <iostream>
#include <vector>

const size_t INITIAL_SIZE = 8;

template <typename T>
struct HashTableData {
    HashTableData() = default;
    HashTableData(const T &key)
    : key(key), state(1) {}
    T key;
    // 0 = NIL, 1 = data, 2 = DEL
    int state = 0;
};

template <typename T, typename Hasher>
class HashTable {
public:
    HashTable(size_t initial_size = INITIAL_SIZE) 
    : table(initial_size, HashTableData()), size(0) {}
    ~HashTable();

    bool add(const T &key) {
        if (size > 0.75 * table.size()) grow();
        HashTableData<T> new_data(key);
        bool met_del = false;
        int del_index = 0;
        for (int i = 0; i < table.size(); i++) {
            size_t index = get_index(key, i);
            if (table[index].state == 1) {
                if (table[index].key == key) return false;
                else continue;
            }
            if (table[index].state == 2) {
                if (!met_del) {
                    del_index = index;
                    met_del = true;
                }
                continue;
            }
            if (table[index].state == 0) {
                if (met_del) table[del_index] = new_data;
                else table[index] = new_data;
                size++;
                return true;
            }
        }
        return false;
    }
    bool has(const T &key) {
        for (int i = 0; i < table.size(); i++) {
            size_t index = get_index(key, i);
            if (table[index].state == 1) {
                if (table[index].key == key) return true;
                else continue;
            }
            if (table[index].state == 0) return false;
        }
        return false;
    }
    bool remove(const T &key) {
        for (int i = 0; i < table.size(); i++) {
            size_t index = get_index(key, i);
            if (table[index].state == 1) {
                if (table[index].key == key) {
                    table[index].state = 2;
                    return true;
                }
                continue;
            }
            else if (table[index].state == 0) {
                return false;
            }
        }
        return false;
    }
private:
    void grow() {
        size_t new_size = table.size() * 2;
        std::vector<HashTableData<T>> new_table(new_size);
        for (int i = 0; i < table.size(); i++) {
            if (table[i].state == 1) {
                size_t index = get_index(table[i].key);
                size_t new_index = h(table[i].key) % new_size;
                new_table[new_index] = table[index];
            }
        }
        table = new_table;
    }
    size_t h(const T &key, int i) {
        return (hasher_1(key) + hasher_2(key) * i);
    }
    size_t get_index(const T &key, int i) {
        return h(key, i) % table.size();
    }
    std::vector<HashTableData<T>> table;
    Hasher hasher_1, hasher_2;
    size_t size;
};

class StringHasher {
public:
    StringHasher(size_t prime = 113)
    : prime(prime) {}

    size_t operator () (const std::string &str) {
        size_t hash = 0;
        for (int i = 0; i < str.size(); i++) {
            hash = hash * prime + str[i];
        }
        return hash;
    }
private:
    size_t prime;
};

void run(std::istream &input, std::ostream &output);
void test();

int main() {

    run(std::cin, std::cout);
    test();

    return 0;
}

void run(std::istream &input, std::ostream &output) {

}

void test() {
    
}