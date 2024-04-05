#include <iostream>

int binary_search(int *array, int start, int end, int key) {
    if (start == end) {
        if (array[start] == key) {
            return start;
        } else if (array[start] < key) {
            return start + 1;
        } else {
            return start;
        }
    }
    int middle = (start + end) / 2;
    if (array[middle] == key) {
        return middle;
    } else if (array[middle] < key) {
        return binary_search(array, middle + 1, end, key);
    } else {
        return binary_search(array, start, std::max(middle - 1, start), key);
    }
}

int main() {
    int length{};
    std::cin >> length;
    int *array = new int[length];
    for (int i{}; i < length; i++) {
        std::cin >> array[i];
    }
    int key{};
    std::cin >> key;
    int pos = binary_search(array, 0, length - 1, key);
    std::cout << pos << std::endl;
    delete[] array;
    return 0;
}