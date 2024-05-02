// Напишите две функции для создания архива из одного файла и извлечения файла из
// архива.

#include "Huffman.h"
#include <vector>
#include <unordered_map>
#include <stack>
#include <cassert>

class Buffer {
public:
    Buffer() = default;
    ~Buffer() = default;

    void write_byte(byte value) {
        if (bit_size % 8) {
            size_t tmp_1 = bit_size % 8;
            size_t tmp = value >> (bit_size % 8);
            data[bit_size / 8] |= (value >> (bit_size % 8));
            data.push_back(value << (8 - (bit_size % 8)));
        }
        else {
            data.push_back(value);
        }
        bit_size += 8;
    }
    void write_bit(byte value) {
        if (bit_size % 8) {
            data[bit_size / 8] |= (value << (7 - (bit_size % 8)));
        }
        else {
            data.push_back(value << 7);
        }
        bit_size++;
    }
    byte read_byte() {
        byte value = data[pos / 8];
        value <<= (pos % 8);
        if (pos % 8) {
            value |= (data[(pos / 8) + 1] >> (8 - (pos % 8)));
        }
        pos += 8;
        return value;
    }
    byte read_bit() {
        byte mask = 1 << (7 - (pos % 8));
        byte value = (data[pos / 8] & mask) >> (7 - (pos % 8));
        pos++;
        return value;
    }
    bool can_read_byte() {
        return bit_size - pos >= 8;
    }
    bool can_read_bit() {
        return bit_size - pos > 0;
    }
    void start_read() {
        pos = 0;
    }
    size_t get_number_of_last_bits() {
        return bit_size % 8;
    }
    bool on_last_byte() {
        return (pos / 8) + 1 >= (bit_size / 8);
    }
private:
    std::vector<byte> data;
    size_t bit_size = 0;
    size_t pos = 0;
};

template <typename T, typename Comparator = std::less<T>>
class Heap {
public:
    Heap() = default;
    Heap(const std::vector<T> &array_parameter) {
        array = array_parameter;
        heapify();
    }
    T &top() {
        return array[0];
    }
    T extract() {
        T value = array[0];
        array[0] = array[array.size() - 1];
        array.pop_back();
        sift_down(0);
        return value;
    }
    void add(const T &value) {
        array.push_back(value);
        sift_up(array.size() - 1);
    }
    int get_size() {
        return array.size();
    }
    std::vector<T> &get_array() {
        return array;
    }
    void heapify() {
        for (int i = array.size() / 2; i >= 0; i--) {
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
        if (right < array.size()) {
            next_index = cmp(array[left], array[right]) ? left : right;
        } else if (left < array.size()) {
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
        if (left < array.size()) {
            if (cmp(array[left], array[index])) return false;
            bool bool_right{true};
            if (right < array.size()) {
                if (cmp(array[right], array[index])) return false;
                bool_right = is_right_private(right);
            }
            bool bool_left = is_right_private(left);
            return bool_left && bool_right;
        } else {
            return true;
        }
    }
    std::vector<T> array;
    Comparator cmp;
};

struct HuffmanNode {
    HuffmanNode(const byte value, const size_t frequency, bool leaf = true)
    : value(value), frequency(frequency), leaf(leaf) {}
    size_t frequency;
    byte value;
    bool leaf;
    HuffmanNode *left = nullptr;
    HuffmanNode *right = nullptr;
};

class HuffmanNodeComparator {
public:
    HuffmanNodeComparator() = default;

    bool operator() (const HuffmanNode &node_1, const HuffmanNode &node_2) {
        return node_1.frequency < node_2.frequency;
    }
};

class HuffmanTree {
public:
    HuffmanTree(HuffmanNode *root)
    : root(root) {}
    ~HuffmanTree() {
        if (root) {
            destroy_tree(root);
        }
    }
    size_t get_code(byte value) {
        size_t size = 0;
        int result = -1;
        if (root->left) result = get_code_internal(root->left, value, 0, size, 1);
        if (root->right && result < 0) result = get_code_internal(root->right, value, 1, size, 1);
        assert(result >= 0);
        return result << (32 - size);
    }
    size_t get_code_size(size_t code) {
        return get_code_size_internal(root, code);
    }
    void to_buffer(Buffer &buffer) {
        to_buffer_internal(buffer, root);
    }
    HuffmanNode *get_root() {
        return root;
    }
    HuffmanNode *get_node_by_bit(byte bit) {
        if (bit == 0 && current_node->left) {
            current_node = current_node->left;
            return current_node;
        }
        if (bit == 1 && current_node->right) {
            current_node = current_node->right;
            return current_node;
        }
        if (bit == 0) {
            current_node = root->left;
            return current_node;
        }
        if (bit == 1) {
            current_node = root->right;
            return current_node;
        }
        return root;
    }
private:
    void destroy_tree(HuffmanNode *node) {
        if (node->left) destroy_tree(node->left);
        if (node->right) destroy_tree(node->right);
        delete node;
    }
    int get_code_internal(HuffmanNode *node, byte value, int result, size_t &size, size_t tmp_size) {
        int tmp = -1;
        if (node->leaf && node->value == value) {
            size = tmp_size;
            return result;
        }
        if (node->left) {
            tmp = get_code_internal(node->left, value, result << 1, size, tmp_size + 1);
        }
        if (node->right && tmp < 0) {
            tmp = get_code_internal(node->right, value, (result << 1) + 1, size, tmp_size + 1);
        }
        return tmp;
    }
    size_t get_code_size_internal(HuffmanNode *node, unsigned int code) {
        if (node->leaf) return 0;
        unsigned int mask = 1 << 31;
        if (code & mask) {
            return get_code_size_internal(node->right, code << 1) + 1;
        }
        else {
            return get_code_size_internal(node->left, code << 1) + 1;
        }
    }
    void to_buffer_internal(Buffer &buffer, HuffmanNode *node) {
        if (node->leaf) {
            buffer.write_bit(1);
            buffer.write_byte(node->value);
        }
        else {
            to_buffer_internal(buffer, node->left);
            to_buffer_internal(buffer, node->right);
            buffer.write_bit(0);
        }
    }
    HuffmanNode *root;
    HuffmanNode *current_node = root;
};

void copy_stream(IInputStream &input, IOutputStream &output) {
    byte value;
    while (input.Read(value)) {
        output.Write(value);
    }
}

void Encode(IInputStream &original, IOutputStream &compressed) {
    // Тест
    // copy_stream(original, compressed);
    // return;

    // Считываем ввод в буфер
    Buffer input_buffer;
    byte value = 0;
    while (original.Read(value)) {
        input_buffer.write_byte(value);
    }
    // Считаем частотности
    std::unordered_map<byte, size_t> frequency_table;
    while (input_buffer.can_read_byte()) {
        value = input_buffer.read_byte();
        frequency_table[value]++;
    }
    // Для каждой буквы создаем узел и добавляем в min heap
    std::vector<HuffmanNode> vector;
    for (const auto &[key, frequency]: frequency_table) {
        vector.push_back(HuffmanNode(key, frequency));
    }
    Heap<HuffmanNode, HuffmanNodeComparator> min_heap(vector);
    // Строим дерево Хаффмана
    while (min_heap.get_size() > 1) {
        HuffmanNode *node_1 = new HuffmanNode(min_heap.extract());
        HuffmanNode *node_2 = new HuffmanNode(min_heap.extract());
        HuffmanNode new_node(0, node_1->frequency + node_2->frequency, false);
        new_node.left = node_1;
        new_node.right = node_2;
        min_heap.add(new_node);
    }
    HuffmanNode *root = new HuffmanNode(min_heap.top());
    HuffmanTree huffman_tree(root);
    // Создаем таблицу кодов
    std::unordered_map<byte, size_t> code_table;
    for (const auto &[key, frequency]: frequency_table) {
        code_table[key] = huffman_tree.get_code(key);
    }
    // Запись числа символов в буфер
    Buffer output_buffer;
    size_t alphabet_number = vector.size();
    output_buffer.write_byte(alphabet_number - 1);
    // Запись дерева в буфер
    huffman_tree.to_buffer(output_buffer);
    // Кодирование сообщения
    input_buffer.start_read();
    Buffer message_buffer;
    while (input_buffer.can_read_byte()) {
        byte value = input_buffer.read_byte();
        unsigned int code = code_table[value];
        unsigned int size = huffman_tree.get_code_size(code);
        unsigned int mask = 1 << 31;
        for (size_t i = 0; i < size; i++) {
            unsigned int new_code = code << i;
            byte bit = ((code << i) & mask) >> 31;
            message_buffer.write_bit(bit);
        }
    }
    // Запись числа последних бит
    size_t number_of_last_bits = (output_buffer.get_number_of_last_bits() + message_buffer.get_number_of_last_bits()) % 8;
    if (number_of_last_bits == 0) number_of_last_bits = 8;
    output_buffer.write_byte(number_of_last_bits);
    // Запись кодированного сообщения
    while (message_buffer.can_read_byte()) {
        output_buffer.write_byte(message_buffer.read_byte());
    }
    while (message_buffer.can_read_bit()) {
        output_buffer.write_bit(message_buffer.read_bit());
    }
    // Запись в выходной поток
    while (output_buffer.can_read_byte()) {
        compressed.Write(output_buffer.read_byte());
    }
    if (output_buffer.can_read_bit()) {
        compressed.Write(output_buffer.read_byte());
    }
}

void Decode(IInputStream &compressed, IOutputStream &original) {
    // Тест
    // copy_stream(compressed, original);
    // return;

    // Запись в буфер
    Buffer input_buffer;
    byte value;
    while (compressed.Read(value)) {
        input_buffer.write_byte(value);
    }
    // Читаем первый байт с числом букв алфавита
    size_t alphabet_number = input_buffer.read_byte() + 1;
    // Читаем дерево Хаффмана
    std::stack<HuffmanNode *> stack;
    for (size_t i = 0; (i < alphabet_number) || (stack.size() > 1);) {
        size_t size = stack.size();
        byte bit = input_buffer.read_bit();
        if (bit) {
            value = input_buffer.read_byte();
            HuffmanNode *node = new HuffmanNode(value, 0);
            stack.push(node);
            i++;
        }
        else {
            HuffmanNode *node_1 = stack.top();
            stack.pop();
            HuffmanNode *node_2 = stack.top();
            stack.pop();
            HuffmanNode *new_node = new HuffmanNode(0, 0, false);
            new_node->right = node_1;
            new_node->left = node_2;
            stack.push(new_node);
        }
    }
    HuffmanTree huffman_tree(stack.top());
    stack.pop();
    // Читаем число последних бит
    size_t number_of_last_bits = input_buffer.read_byte();
    // Считываем закодированную последовательность и записываем в буфер
    Buffer output_buffer;
    HuffmanNode *node = nullptr;
    while (!input_buffer.on_last_byte() || number_of_last_bits) {
        if (input_buffer.on_last_byte()) number_of_last_bits--;
        byte bit = input_buffer.read_bit();
        node = huffman_tree.get_node_by_bit(bit);
        if (node->leaf) output_buffer.write_byte(node->value);
    }
    // Запись в выходной поток
    while (output_buffer.can_read_byte()) {
        original.Write(output_buffer.read_byte());
    }
}