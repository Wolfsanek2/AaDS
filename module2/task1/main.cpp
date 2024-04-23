// Реализуйте структуру данных типа “множество строк” на основе динамической
// хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных
// латинских букв.
// 1_2. Для разрешения коллизий используйте двойное хеширование.

#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>

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
    : table(initial_size, HashTableData<T>()), size(0), hasher_1(Hasher(113)), hasher_2(Hasher(313)) {}
    ~HashTable() = default;

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
        std::vector<HashTableData<T>> old_table = table;
        size_t new_size = old_table.size() * 2;
        std::vector<HashTableData<T>> new_table(new_size, HashTableData<T>());
        table = new_table;
        size = 0;
        for (int i = 0; i < old_table.size(); i++) {
            if (old_table[i].state == 1) {
                add(old_table[i].key);
            }
        }
    }
    size_t h(const T &key, int i) {
        return (hasher_1(key) + (hasher_2(key) * 2 + 1) * i);
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
    StringHasher(size_t prime)
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
void test_HashTable();
void test_run();
void test();

int main() {

    // test();
    run(std::cin, std::cout);

    return 0;
}

void run(std::istream &input, std::ostream &output) {
    HashTable<std::string, StringHasher> table;
    char op;
    std::string key;
    while (input >> op >> key) {
        switch (op) {
            case '+':
                output << (table.add(key) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                output << (table.remove(key) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                output << (table.has(key) ? "OK" : "FAIL") << std::endl;
                break;
        }
    }
}

void test_HashTable() {
    {
        HashTable<std::string, StringHasher> table;
        assert(table.has("1") == 0);
        assert(table.has("1") == 0);
        assert(table.remove("1") == 0);
        assert(table.remove("1") == 0);
        assert(table.add("1") == 1);
        assert(table.add("1") == 0);
        assert(table.has("1") == 1);
        assert(table.has("1") == 1);
        assert(table.remove("1") == 1);
        assert(table.remove("1") == 0);
        assert(table.has("1") == 0);
        assert(table.has("1") == 0);
        assert(table.add("1") == 1);
    }
    {
        HashTable<std::string, StringHasher> table;
        assert(table.add("0") == 1);
        assert(table.add("1") == 1);
        assert(table.add("2") == 1);
        assert(table.add("3") == 1);
        assert(table.add("4") == 1);
        assert(table.add("5") == 1);
        assert(table.add("6") == 1);
        assert(table.add("7") == 1);
        assert(table.add("8") == 1);
        assert(table.add("9") == 1);
        assert(table.add("10") == 1);
        assert(table.add("11") == 1);
        assert(table.add("12") == 1);
        assert(table.add("13") == 1);
        assert(table.add("14") == 1);
        assert(table.add("15") == 1);
        assert(table.add("16") == 1);
        assert(table.add("17") == 1);
        assert(table.add("18") == 1);
        assert(table.add("19") == 1);
        assert(table.add("20") == 1);
    }
    std::cout << "HashTable test OK" << std::endl;
}

void test_run() {
    {
        std::stringstream input, output;
        input << "+ hello" << std::endl;
        input << "+ bye" << std::endl;
        input << "? bye" << std::endl;
        input << "+ bye" << std::endl;
        input << "- bye" << std::endl;
        input << "? bye" << std::endl;
        input << "? hello" << std::endl;
        run(input, output);
        assert(output.str() == "OK\nOK\nOK\nFAIL\nOK\nFAIL\nOK\n");
    }
    std::cout << "run test OK" << std::endl;
}

void test() {
    test_HashTable();
    test_run();
}