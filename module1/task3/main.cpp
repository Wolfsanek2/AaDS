// 3_3. Реализовать очередь с помощью двух стеков.

#include <iostream>
#include <cassert>
#include <sstream>

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

class Queue {
public:
    Queue() = default;
    void enqueue(const int value) {
        stack_1.push(value);
    }
    int dequeue() {
        if (stack_2.is_empty()) {
            pour();
        }
        if (stack_2.is_empty()) return -1;
        return stack_2.pop();
    }
private:
    void pour() {
        while (!stack_1.is_empty()) {
            stack_2.push(stack_1.pop());
        }
    }
    Stack stack_1, stack_2;
};

void test();
void test_Stack();
void test_run();
void run(std::istream &input, std::ostream &output);

int main() {

    // test();
    run(std::cin, std::cout);

    return 0;
}

void run(std::istream &input, std::ostream &output) {
    int n{};
    input >> n;
    Queue queue;
    bool out = true;
    int i{};
    while (i++ < n && out) {
        int a{}, b{}, value{};
        input >> a >> b;
        switch (a) {
            case 2:
                value = queue.dequeue();
                if (value != b) out = false;
                break;
            case 3:
                queue.enqueue(b);
                break;
        }
    }
    if (out) {
        output << "YES" << std::endl;
    } else {
        output << "NO" << std::endl;
    }
}

void test() {
    test_Stack();
    test_run();
}

void test_Stack() {
    Stack stack;
    stack.push(1);
    assert(stack.get_array()[0] == 1);
    assert(stack.get_size() == 1);
    assert(stack.get_capacity() == 1);

    stack.push(2);
    assert(stack.get_array()[1] == 2);
    assert(stack.get_size() == 2);
    assert(stack.get_capacity() == 2);

    stack.push(3);
    assert(stack.get_array()[2] == 3);
    assert(stack.get_size() == 3);
    assert(stack.get_capacity() == 4);

    assert(stack.pop() == 3);
    assert(stack.get_size() == 2);
    assert(stack.get_capacity() == 4);

    assert(stack.pop() == 2);
    assert(stack.get_size() == 1);
    assert(stack.get_capacity() == 4);

    assert(stack.pop() == 1);
    assert(stack.get_size() == 0);
    assert(stack.get_capacity() == 2);

    std::cout << "Stack test OK" << std::endl;
}

void test_run() {
    {
        std::stringstream input, output;
        input << "3\n";
        input << "3 44\n";
        input << "3 50\n";
        input << "2 44\n";
        run(input, output);
        assert(output.str() == "YES\n");
    }
    {
        std::stringstream input, output;
        input << "2\n";
        input << "2 -1\n";
        input << "3 10\n";
        run(input, output);
        assert(output.str() == "YES\n");
    }
    {
        std::stringstream input, output;
        input << "2\n";
        input << "3 44\n";
        input << "2 66\n";
        run(input, output);
        assert(output.str() == "NO\n");
    }
    std::cout << "run test OK" << std::endl;
}