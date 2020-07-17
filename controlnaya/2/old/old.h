#ifndef INC_2_OLD_H
#define INC_2_OLD_H


class buffer {
private:
    char *buf_;
    int capacity_;
    int size_;
    int flag_;

    std::pair <std::promise<void>, std::future<void>> p_read_;
    std::pair <std::promise<void>, std::future<void>> p_write_;

    std::mutex lock_;

public:
    explicit buffer(int cap) {
        buf_ = new char[cap];
        capacity_ = cap;
        size_ = 0;
        flag_ = 0;

        p_read_.second  = p_write_.first.get_future();
        p_write_.second = p_read_.first.get_future();
    };

    ~buffer() {
        delete[] buf_;
        capacity_ = -1;
        size_ = -1;
        flag_ = -1;
    }

    int size() {
        return size_;
    }

    int capacity() {
        return capacity_;
    }

    char *buf() {
        return buf_;
    }

    void plus_size(int heh) {
        lock_.lock();
        size_ += heh;
        lock_.unlock();
    }

    void minus_size(int heh) {
        lock_.lock();
        size_ -= heh;
        lock_.unlock();
    }

    int flag() {
        return flag_;
    }

    void change_flag(int new_flag) {
        flag_ = new_flag;
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

    int flag_read = 0;
    int flag_write = 0;
};


#endif //INC_2_OLD_H
