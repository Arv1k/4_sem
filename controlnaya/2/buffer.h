#ifndef INC_2_BUFFER_H
#define INC_2_BUFFER_H


class buffer {
private:
    char *_buf;
    int _capacity;
    int _size;
    int _flag;

public:
    explicit buffer(int cap) {
        _buf = new char[cap];
        _capacity = cap;
        _size = 0;
        _flag = 0;
    };

    ~buffer() {
        delete[] _buf;
        _capacity = -1;
        _size = -1;
        _flag = -1;
    }

    int size() {
        return _size;
    }

    int capacity() {
        return _capacity;
    }

    char *buf() {
        return _buf;
    }

    void plus_size(int heh) {
        _size += heh;
    }

    void minus_size(int heh) {
        _size -= heh;
    }

    int flag() {
        return _flag;
    }

    void change_flag(int new_flag) {
        _flag = new_flag;
    }
};


#endif //INC_2_BUFFER_H
