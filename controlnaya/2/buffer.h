#ifndef INC_2_BUFFER_H
#define INC_2_BUFFER_H


class buffer {
private:
    char *_buf;
    int _capacity;
    int _size;
    int _flag;
    std::pair <std::promise<void>, std::future<void>> p_read_;
    std::pair <std::promise<void>, std::future<void>> p_write_;
    std::mutex lock;

public:
    explicit buffer(int cap) {
        _buf = new char[cap];
        _capacity = cap;
        _size = 0;
        _flag = 0;

        p_read_.second  = p_write_.first.get_future();
        p_write_.second = p_read_.first.get_future();
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
        lock.lock();
        _size += heh;
        lock.unlock();
    }

    void minus_size(int heh) {
        lock.lock();
        _size -= heh;
        lock.unlock();
    }

    int flag() {
        return _flag;
    }

    void change_flag(int new_flag) {
        _flag = new_flag;
    }

    void read_set() {
        p_read_.first.set_value();
    }

    void write_set() {
        p_write_.first.set_value();
    }

    void read_get() {
        p_read_.second.get();

        p_write_.first = std::promise<void> ();
        p_read_.second = p_write_.first.get_future();
    }

    void write_get() {
        p_write_.second.get();

        p_read_.first = std::promise<void> ();
        p_write_.second = p_read_.first.get_future();
    }
};


#endif //INC_2_BUFFER_H
