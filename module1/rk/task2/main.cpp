#include <iostream>
#include <cassert>

class Stack {
public:
    Stack() = default;
    Stack(const Stack &other) 
    : capacity(other.capacity), size(other.size){
        array = new int[capacity];
        for (int i{}; i < size; i++) {
            array[i] = other.array[i];
        }
    }
    ~Stack() {
        delete[] array;
    }
    Stack & operator = (const Stack &other) {
        Stack *new_stack = new Stack(other);
        return *new_stack;
    }
    void push(int value) {
        if (size == capacity) {
            expand();
        }
        array[size++] = value;
    }
    int pop() {
        assert(size);
        if (size == 0.25 * capacity) reduce();
        return array[--size];
    }
    bool is_empty() {
        return !size;
    }
    int *get_array() {
        return array;
    }
    int get_size() {
        return size;
    }
    int get_capacity() {
        return capacity;
    }
private:
    void expand() {
        if (capacity) {
            capacity *= 2;
        } else {
            capacity = 1;
        }
        int *new_array = new int[capacity];
        for (int i{}; i < size; i++) {
            new_array[i] = array[i];
        }
        delete[] array;
        array = new_array;
    }
    void reduce() {
        if (!capacity) return;
        capacity /= 2;
        int *new_array = new int[capacity];
        for (int i{}; i < size; i++) {
            new_array[i] = array[i];
        }
        delete[] array;
        array = new_array;
    }
    int *array = nullptr;
    int size{};
    int capacity{};
};

int main() {
    std::string str1, str2;
    std::cin >> str1 >> str2;

    return 0;
}