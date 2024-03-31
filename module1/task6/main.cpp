// Дано множество целых чисел из [0..10^9] размера n.
// Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры
// множества:
// 1) 10% перцентиль
// 2) медиана
// 3) 90% перцентиль
// 6_2. Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию Partition реализуйте
// методом прохода двумя итераторами от конца массива к началу.

#include <iostream>
#include <sstream>
#include <cassert>

template <typename T, typename Comparator = std::less<T>>
int partition(T *arr, int left, int right, Comparator cmp = Comparator());
template <typename T, typename Comparator = std::less<T>>
int kth_statistic(T *arr, int length, double k, Comparator cmp = Comparator());
void run(std::istream &input, std::ostream &output);
void test_partition();
void test_run();
void test();

int main() {

    // test();
    run(std::cin, std::cout);

    return 0;
}

template <typename T, typename Comparator>
int partition(T *arr, int left, int right, Comparator cmp) {
    if (left == right) return left;
    T value_1 {arr[left]}, value_2 {arr[(left + right) / 2]}, value_3 {arr[right]};
    T pivot{};
    int pivot_index{};
    if (value_1 < value_2 && value_1 < value_3) {
        if (value_2 < value_3) {
            pivot = value_2;
            pivot_index = (left + right) / 2;
        } else {
            pivot = value_3;
            pivot_index = right;
        }
    } else if (value_2 < value_1 && value_2 < value_3) {
        if (value_1 < value_3) {
            pivot = value_1;
            pivot_index = left;
        } else {
            pivot = value_3;
            pivot_index = right;
        }
    } else {
        if (value_1 < value_2) {
            pivot = value_1;
            pivot_index = left;
        } else {
            pivot = value_2;
            pivot_index = (left + right) / 2;
        }
    }
    std::swap(arr[pivot_index], arr[left]);
    int i = right, j = right;
    while (i > left) {
        while (cmp(arr[i], pivot)) i--;
        if (i > left) {
            std::swap(arr[i], arr[j]);
            i--;
            j--;
        }
    }
    std::swap(arr[left], arr[j]);
    return j;
}

template <typename T, typename Comparator>
int kth_statistic(T *arr, int length, double k, Comparator cmp) {
    int k_pos = k * length;
    int left{0}, right{length - 1};
    int pos = partition(arr, left, right, cmp);
    int value{arr[pos]};
    while (k_pos != pos) {
        if (pos < k_pos) {
            left = pos + 1;
            pos = partition(arr, left, right, cmp);
        } else {
            right = pos - 1;
            pos = partition(arr, left, right, cmp);
        }
        value = arr[pos];
    }
    return value;
}

void run(std::istream &input, std::ostream &output) {
    int length{};
    input >> length;
    int *arr = new int[length];
    for (int i{}; i < length; i++) {
        input >> arr[i];
    }
    int value_1{}, value_2{}, value_3{};
    value_1 = kth_statistic(arr, length, 0.1);
    value_2 = kth_statistic(arr, length, 0.5);
    value_3 = kth_statistic(arr, length, 0.9);
    output << value_1 << std::endl;
    output << value_2 << std::endl;
    output << value_3 << std::endl;

    delete[] arr;
}

void test_partition() {
    {
        int array[] = {1, 2, 3, 4, 5};
        int length = 5;
        int index = partition(array, 0, length - 1);
        assert(array[0] == 1);
        assert(array[1] == 2);
        assert(array[2] == 3);
        assert(array[3] == 4);
        assert(array[4] == 5);
        assert(index == 2);
    }
    {
        int array[] = {5, 1, 3, 4, 2};
        int length = 5;
        int index = partition(array, 0, length - 1);
        assert(array[0] == 2);
        assert(array[1] == 1);
        assert(array[2] == 3);
        assert(array[3] == 5);
        assert(array[4] == 4);
        assert(index == 2);
    }
    std::cout << "partition test OK" << std::endl;
}

void test_run() {
    {
        std::stringstream input, output;
        input << "10" << std::endl;
        input << "1 2 3 4 5 6 7 8 9 10" << std::endl;
        run(input, output);
        assert(output.str() == "2\n6\n10\n");
    }
    {
        std::stringstream input, output;
        input << "10" << std::endl;
        input << "10 1 9 2 8 3 7 4 6 5" << std::endl;
        run(input, output);
        assert(output.str() == "2\n6\n10\n");        
    }
    {
        std::stringstream input, output;
        input << "1" << std::endl;
        input << "5" << std::endl;
        run(input, output);
        assert(output.str() == "5\n5\n5\n");
    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "1 1 1 1 1" << std::endl;
        run(input, output);
        assert(output.str() == "1\n1\n1\n");
    }
    {
        std::stringstream input, output;
        input << "2" << std::endl;
        input << "2 1" << std::endl;
        run(input, output);
        assert(output.str() == "1\n2\n2\n");
    }
    {
        std::stringstream input, output;
        input << "2" << std::endl;
        input << "27 36" << std::endl;
        run(input, output);
        assert(output.str() == "27\n36\n36\n");
    }
    {
        std::stringstream input, output;
        input << "6" << std::endl;
        input << "4 2 7 3 6 1" << std::endl;
        run(input, output);
        assert(output.str() == "1\n4\n7\n");
    }
    {
        int number = 100;
        for (int i{}; i < number; i++) {
            std::stringstream input, output;
            int length = (rand() % 9) + 1;
            int *array = new int[length];
            input << length << std::endl;
            std::cout << "number: " << i << std::endl;
            std::cout << "length: " << length << std::endl;
            std::cout << "input: ";
            for (int i{}; i < length; i++) {
                array[i] = rand() % 100;
                input << array[i] << " ";
                std::cout << array[i] << " ";
            }
            std::cout << std::endl;
            run(input, output);
            std::cout << "output: " << std::endl << output.str() << std::endl;
        }
    }
    std::cout << "run test OK" << std::endl;
}

void test() {
    test_partition();
    test_run();
}