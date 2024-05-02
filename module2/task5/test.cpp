#define private public
#include "main.cpp"
#undef private

#include <iostream>
#include <cassert>
#include <fstream>

class OutputStringStream: public IOutputStream {
public:
    OutputStringStream() = default;
    void Write(byte value) {
        string += value;
    }
    std::string get_string() const {
        return string;
    }
private:
    std::string string;
};

class InputStringStream: public IInputStream {
public:
    InputStringStream(const std::string_view &str)
    : string(str) {}
    InputStringStream(const OutputStringStream &stream)
    : string(stream.get_string()) {}
    bool Read(byte &value) {
        if (pos == string.size()) return false;
        value = string[pos++];
        return true;
    }
    std::string get_string() {
        return string;
    }
private:
    std::string string;
    size_t pos = 0;
};

class InputFileStream: public IInputStream {
public:
    InputFileStream(char *file_name) {
        file = std::ifstream(file_name, std::ios::binary);
    }
    bool Read(byte &value) {
        if (file.is_open()) {
            char ch;
            if (file.get(ch)) {
                value = ch;
                return true;    
            }
            else {
                file.close();
                return false;
            }
        }
        else return false;
    }
    void close_file() {
        file.close();
    }
private:
    std::ifstream file;
};

class OutputFileStream: public IOutputStream {
public:
    OutputFileStream(char *filename) {
        file = std::ofstream(filename, std::ios::binary);
    }
    void Write(byte value) {
        if (file.is_open()) {
            char ch = value;
            file.put(ch);
        }
    }
    void close_file() {
        file.close();
    }
private:
    std::ofstream file;
};

void test_Buffer();
void test_frequency_table();
void test_Encode();
void test();

int main() {
    test();
}

void test_Buffer() {
    {
        Buffer buffer;
        buffer.write_byte('a');
        buffer.write_byte(234);
        assert(buffer.read_byte() == 'a');
        assert(buffer.read_byte() == 234);
    }
    {}
    {
        Buffer buffer;
        buffer.write_bit(0);
        buffer.write_bit(0);
        buffer.write_bit(0);
        buffer.write_bit(0);
        buffer.write_bit(1);
        buffer.write_bit(1);
        buffer.write_bit(1);
        buffer.write_bit(1);
        buffer.write_bit(1);
        buffer.write_bit(1);
        assert(buffer.read_bit() == 0);
        assert(buffer.read_bit() == 0);
        assert(buffer.read_bit() == 0);
        assert(buffer.read_bit() == 0);
        assert(buffer.read_bit() == 1);
        assert(buffer.read_bit() == 1);
        assert(buffer.read_bit() == 1);
        assert(buffer.read_bit() == 1);
        assert(buffer.read_bit() == 1);
        assert(buffer.read_bit() == 1);
    }
    {
        Buffer buffer;
        buffer.write_bit(1);
        buffer.write_bit(0);
        buffer.write_byte('a');
        buffer.write_bit(1);
        buffer.write_byte('b');
        buffer.write_bit(1);
        buffer.write_bit(1);
        buffer.write_bit(1);
        buffer.write_bit(0);
        buffer.write_bit(0);
        buffer.write_byte('c');
        buffer.write_bit(0);
        buffer.write_bit(1);
        assert(buffer.read_bit() == 1);
        assert(buffer.read_bit() == 0);
        assert(buffer.read_byte() == 'a');
        assert(buffer.read_bit() == 1);
        assert(buffer.read_byte() == 'b');
        assert(buffer.read_bit() == 1);
        assert(buffer.read_bit() == 1);
        assert(buffer.read_bit() == 1);
        assert(buffer.read_bit() == 0);
        assert(buffer.read_bit() == 0);
        assert(buffer.read_byte() == 'c');
        assert(buffer.read_bit() == 0);
        assert(buffer.read_bit() == 1);
    }
    {
        Buffer buffer;
        buffer.write_byte('a');
        buffer.write_byte('b');
        assert(!buffer.on_last_byte());
        buffer.read_bit();
        assert(!buffer.on_last_byte());
        buffer.read_bit();
        assert(!buffer.on_last_byte());
        buffer.read_bit();
        assert(!buffer.on_last_byte());
        buffer.read_bit();
        assert(!buffer.on_last_byte());
        buffer.read_bit();
        assert(!buffer.on_last_byte());
        buffer.read_bit();
        assert(!buffer.on_last_byte());
        buffer.read_bit();
        assert(!buffer.on_last_byte());
        buffer.read_bit();
        assert(buffer.on_last_byte());
        buffer.read_bit();
        assert(buffer.on_last_byte());
        buffer.read_bit();
        assert(buffer.on_last_byte());
        buffer.read_bit();
        assert(buffer.on_last_byte());
        buffer.read_bit();
        assert(buffer.on_last_byte());
        buffer.read_bit();
        assert(buffer.on_last_byte());
        buffer.read_bit();
        assert(buffer.on_last_byte());
        buffer.read_bit();
        assert(buffer.on_last_byte());
    }
    std::cout << "Buffer test OK" << std::endl;
}

void test_frequency_table() {
    {
        Buffer buffer;
        std::string str = "abracadabra";
        for (char ch: str) {
            buffer.write_byte(ch);
        }
        std::unordered_map<byte, size_t> frequency_table;
        while (buffer.can_read_byte()) {
            byte value = buffer.read_byte();
            frequency_table[value]++;
        }
        assert(frequency_table['a'] == 5);
        assert(frequency_table['b'] == 2);
        assert(frequency_table['r'] == 2);
        assert(frequency_table['c'] == 1);
        assert(frequency_table['d'] == 1);
    }
    std::cout << "frequency_table test OK" << std::endl;
}

void test_run() {
    {
        std::string string = "abracadabraa";
        InputStringStream original_1(string);
        OutputStringStream compressed_1;
        Encode(original_1, compressed_1);
        InputStringStream compressed_2(compressed_1);
        OutputStringStream original_2;
        Decode(compressed_2, original_2);
        assert(original_1.get_string() == original_2.get_string());
    }
    {
        std::string string = "abracadabra";
        InputStringStream original_1(string);
        OutputStringStream compressed_1;
        Encode(original_1, compressed_1);
        InputStringStream compressed_2(compressed_1);
        OutputStringStream original_2;
        Decode(compressed_2, original_2);
        assert(original_1.get_string() == original_2.get_string());
    }
    {
        std::string string = {-1, -40, -11, -32, 0, 16, 74};
        InputStringStream original_1(string);
        OutputStringStream compressed_1;
        Encode(original_1, compressed_1);
        InputStringStream compressed_2(compressed_1);
        OutputStringStream original_2;
        Decode(compressed_2, original_2);
        assert(original_1.get_string() == original_2.get_string());
    }
    {
        char file_name_1[] = "test_in_1.txt";
        char file_name_2[] = "test_compressed_1.txt";
        char file_name_3[] = "test_out_1.txt";
        InputFileStream input_stream_1(file_name_1);
        OutputFileStream output_stream_1(file_name_2);
        Encode(input_stream_1, output_stream_1);
        input_stream_1.close_file();
        output_stream_1.close_file();
        InputFileStream input_stream_2(file_name_2);
        OutputFileStream output_stream_2(file_name_3);
        Decode(input_stream_2, output_stream_2);
        input_stream_2.close_file();
        output_stream_2.close_file();
    }
    {
        char file_name_1[] = "test_in_2.jpg";
        char file_name_2[] = "test_compressed_2.jpg";
        char file_name_3[] = "test_out_2.jpg";
        InputFileStream input_stream_1(file_name_1);
        OutputFileStream output_stream_1(file_name_2);
        Encode(input_stream_1, output_stream_1);
        input_stream_1.close_file();
        output_stream_1.close_file();
        InputFileStream input_stream_2(file_name_2);
        OutputFileStream output_stream_2(file_name_3);
        Decode(input_stream_2, output_stream_2);
        input_stream_2.close_file();
        output_stream_2.close_file();
    }
    {
        char file_name_1[] = "test_in_3.jpg";
        char file_name_2[] = "test_compressed_3.jpg";
        char file_name_3[] = "test_out_3.jpg";
        InputFileStream input_stream_1(file_name_1);
        OutputFileStream output_stream_1(file_name_2);
        Encode(input_stream_1, output_stream_1);
        input_stream_1.close_file();
        output_stream_1.close_file();
        InputFileStream input_stream_2(file_name_2);
        OutputFileStream output_stream_2(file_name_3);
        Decode(input_stream_2, output_stream_2);
        input_stream_2.close_file();
        output_stream_2.close_file();
    }
    {
        char file_name_1[] = "test_in_4.jpg";
        char file_name_2[] = "test_compressed_4.jpg";
        char file_name_3[] = "test_out_4.jpg";
        InputFileStream input_stream_1(file_name_1);
        OutputFileStream output_stream_1(file_name_2);
        Encode(input_stream_1, output_stream_1);
        input_stream_1.close_file();
        output_stream_1.close_file();
        InputFileStream input_stream_2(file_name_2);
        OutputFileStream output_stream_2(file_name_3);
        Decode(input_stream_2, output_stream_2);
        input_stream_2.close_file();
        output_stream_2.close_file();
    }
    std::cout << "run test OK" << std::endl;
}

void test() {
    test_Buffer();
    test_frequency_table();
    test_run();
}