//4.3 Планировщик процессов
// В операционной системе Technux есть планировщик процессов.
// Каждый процесс характеризуется:
// - приоритетом P
// - временем, которое он уже отработал t
// - временем, которое необходимо для завершения работы процесса T
// Планировщик процессов выбирает процесс с минимальным значением P * (t + 1), выполняет его время P
// и кладет обратно в очередь процессов.
// Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
// Требуется посчитать кол-во переключений процессора.
//Формат входных данных: Сначала вводится кол-во процессов. После этого процессы в формате P T
//Формат выходных данных: Кол-во переключений процессора.

#include <iostream>
#include <sstream>
#include <cassert>

struct Process {
    Process() = default;
    Process(int P, int T) 
    : P(P), T(T) {}
    int P{};
    int t{};
    int T{};
};

template <typename T>
class Array {
public:
    Array() = default;
    Array(T *ptr, int length)
    : length(length) {
        assert(length >= 0);
        array = new T[length];
        for (int i{}; i < length; i++, capacity++) {
            array[i] = ptr[i];
        }
    }
    ~Array() {
        delete[] array;
    }
    Array(const Array &other) 
    : capacity(other.capacity), length(other.length) {
        array = new T[capacity];
        for (int i{}; i < length; i++) array[i] = other.array[i];
    }
    void operator = (const Array &other) {
        Array *new_array = new Array(other.array, other.length);
        this->array = new_array->array;
        this->capacity = new_array->capacity;
        this->length = new_array->length;
    }
    T &operator [] (int index) {
        return array[index];
    }
    void push(const T value) {
        if (length == capacity) expand();
        array[length++] = value;
    }
    T pop() {
        assert(length);
        if (length < 0.25 * capacity) reduce();
        return array[length--];
    }
    int get_length() {
        return length;
    }
    T *get_array() {
        return array;
    }
private:
    void reallocate() {
        T *new_array = new T[capacity];
        for (int i{}; i < length; i++) new_array[i] = array[i];
        delete [] array;
        array = new_array;
    }
    void expand() {
        if (capacity) {
            capacity *= 2;
        } else {
            capacity = 1;
        }
        reallocate();
    }
    void reduce() {
        capacity /= 2;
        reallocate();
    }
    T *array = nullptr;
    int capacity{};
    int length{};
};

template <typename T, typename Comparator = std::less<T>>
class Heap {
public:
    Heap() = default;
    Heap(const Array<T> &array_parameter) {
        array = array_parameter;
        heapify();
    }
    T &top() {
        return array[0];
    }
    T extract() {
        T value = array[0];
        array[0] = array[array.get_length() - 1];
        array.pop();
        sift_down(0);
        return value;
    }
    void add(const T &value) {
        array.push(value);
    }
    int get_size() {
        return array.get_length();
    }
    Array<T> &get_array() {
        return array;
    }
    void heapify() {
        for (int i{array.get_length() / 2}; i >= 0; i--) {
            sift_down(i);
        }
    }
    bool is_right() {
        return is_right_private(0);
    }
    bool is_empty() {
        return !array.get_length();
    }
private:
    void sift_down(int index) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int next_index{};
        if (right < array.get_length()) {
            next_index = cmp(array[left], array[right]) ? left : right;
        } else if (left < array.get_length()) {
            next_index = left;
        } else return;
        if (cmp(array[next_index], array[index])) {
            std::swap(array[next_index], array[index]);
        } else return;
        sift_down(next_index);
    }
    void sift_up(int index) {
        if (!index) return;
        int parent_index = (index - 1) / 2;
        if (cmp(array[index], array[parent_index])) {
            std::swap(array[index], array[parent_index]);
        }
        sift_up(parent_index);
    }
    bool is_right_private(int index) {
        int left = 2 * index + 1;
        int right = left + 1;
        if (left < array.get_length()) {
            if (cmp(array[left], array[index])) return false;
            bool bool_right{true};
            if (right < array.get_length()) {
                if (cmp(array[right], array[index])) return false;
                bool_right = is_right_private(right);
            }
            bool bool_left = is_right_private(left);
            return bool_left && bool_right;
        } else {
            return true;
        }
    }
    Array<T> array;
    Comparator cmp;
};

class Process_comparator {
public:
    Process_comparator() = default;
    bool operator() (const Process &p1, const Process &p2) {
        return p1.P * (p1.t + 1) < p2.P * (p2.t + 1);
    }
};

void run(std::istream &input, std::ostream &output);
bool Process_less(const Process &p1, const Process &p2);
template <typename T>
bool default_less(const T &val1, const T &val2);
template <typename T, typename Comparator>
std::ostream& operator << (std::ostream& out , const Heap<T, Comparator> &heap);
void test_Process();
void test_Array();
void test_Process_less();
void test_Heap();
void test_run();
void test();

int main() {

    // test();
    run(std::cin, std::cout);

    return 0;
}

void run(std::istream &input, std::ostream &output) {
    int number{};
    input >> number;
    Array<Process> array;
    for (int i{}; i < number; i++) {
        int P{}, T{};
        input >> P >> T;
        array.push(Process(P, T));
    }
    Heap<Process, Process_comparator> heap(array);
    int count{};
    while (!heap.is_empty()) {
        Process process = heap.extract();
        process.t += process.P;
        if (process.t < process.T) heap.add(process);
        count++;
    }
    output << count << std::endl;
}

bool Process_less(const Process &p1, const Process &p2) {
    return p1.P * (p1.t + 1) < p2.P * (p2.t + 1);
}

template <typename T>
bool default_less(const T &val1, const T &val2) {
    return val1 < val2;
}

template <typename T, typename Comparator>
std::ostream& operator << (std::ostream& out, Heap<T, Comparator> &heap) {
    int length = heap.get_size();
    for (int i{0}; i < heap.get_size(); i++) {
        out << heap.get_array()[i] << " ";
    }
    return out;
}

void test_Process() {
    {
        Process process(10, 20);
        assert(process.P == 10);
        assert(process.T == 20);
        assert(process.t == 0);
    }
    std::cout << "Process test OK" << std::endl;
}

void test_Array() {
    {
        int ptr[] = {1, 2, 3, 4, 5};
        Array<int> array(ptr, 5);
        assert(array[0] == 1);
        assert(array[1] == 2);
        assert(array[2] == 3);
        assert(array[3] == 4);
        assert(array[4] == 5);
    }
    {
        char ptr[] = {'a', 'r', 'r', 'a', 'y'};
        Array<char> array(ptr, 5);
        assert(array[0] == 'a');
        assert(array[1] == 'r');
        assert(array[2] == 'r');
        assert(array[3] == 'a');
        assert(array[4] == 'y');
    }
    {
        int array[] = {1, 2, 3, 4, 5};
        Array<int> arr1(array, 5), arr2;
        arr2 = arr1;
        assert(arr1.get_length() == arr2.get_length());
    }
    std::cout << "Array test OK" << std::endl;
}

void test_Process_less() {
    {
        Process p1(10, 10), p2(20, 10);
        assert(Process_less(p1, p2));
        p1.P = 30;
        assert(!Process_less(p1, p2));
    }
    {
        Process p1(1, 10), p2(2, 10);
        p1.t = 2;
        assert(Process_less(p2, p1));
    }
    std::cout << "Process_less test OK" << std::endl;
}

void test_default_less() {
    {
        assert(default_less<int>(1, 2));
        assert(default_less<char>('a', 'b'));
        assert(default_less<float>(5.7, 10.9));
    }
    std::cout << "default_less test OK" << std::endl;
}

void test_Heap() {
    {
        int array[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
        Array<int> din_array(array, 9);
        Heap<int> heap(din_array);
        assert(heap.is_right());
        assert(heap.top() == 1);
        assert(heap.extract() == 1);
        assert(heap.extract() == 2);
        assert(heap.is_right());
    }
    std::cout << "Heap test OK" << std::endl;
}

void test_run() {
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "1 10" << std::endl;
        input << "1 5" << std::endl;
        input << "2 5" << std::endl;
        run(input, output);
        assert(output.str() == "18\n");
    }
    std::cout << "run test OK" << std::endl;
}

void test() {
    test_Process();
    test_Array();
    test_Process_less();
    test_default_less();
    test_Heap();
    test_run();
}