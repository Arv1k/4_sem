#ifndef INC_2_BUFFER_H
#define INC_2_BUFFER_H


class buffer {
private:
    int capacity_;
    int block_size_;
    int size_;
    char* data_;

    std::pair <std::promise<void>, std::future<void>> read_p_;
    std::pair <std::promise<void>, std::future<void>> write_p_;

    std::mutex lock_;

    int read_offset_ = 0;
    int write_offset_ = 0;

    bool read_pr_flag_ = true;
    bool write_pr_flag_ = true;

    void dump(FILE* there) {
        lock_.lock();
        fprintf(there, "\ncapacity_ = %d\n", capacity_);
        fprintf(there, "block_size_ = %d\n", block_size_);
        fprintf(there, "size_ = %d\n", size_);
        fprintf(there, "read_offset_ = %d\n", read_offset_);
        fprintf(there, "write_offset_ = %d\n", write_offset_);
        fprintf(there, "read_pr_flag = %d\n", read_pr_flag_);
        fprintf(there, "write_pr_flag_ = %d\n\n", write_pr_flag_);
        lock_.unlock();
    }

public:
    bool is_reading = false;

    explicit buffer(int capacity, int block_size):
    capacity_(capacity), block_size_(block_size), size_(0) {
        data_ = new char[capacity_];

        read_p_.second = write_p_.first.get_future();
        write_p_.second = read_p_.first.get_future();
    }

    ~buffer() {
        capacity_ = -1;
        block_size_ = -1;
        size_ = -1;

        delete [] data_;
        data_ = nullptr;
    }

    int read_from(int src_fd) {
        lock_.lock();
        int remaining = capacity_ - size_;
        int to_read = (remaining > block_size_)? block_size_ : remaining;
        int ch_read = read(src_fd, data_ + read_offset_, to_read);
        size_ += ch_read;
        lock_.unlock();

        read_offset_ += ch_read;

        // fprintf(stderr, "\nReader: ch_read = %d\n", ch_read);
        //dump(stderr);

        lock_.lock();
        if ((size_*100 > 80*capacity_) && read_pr_flag_) {
            read_p_.first.set_value();

            read_pr_flag_ = false;
        }
        lock_.unlock();

        if (read_offset_ == capacity_) {
            read_p_.second.get();

            lock_.lock();
            write_p_.first = std::promise<void> ();
            read_p_.second = write_p_.first.get_future();
            write_pr_flag_ = true;
            lock_.unlock();

            read_offset_ = 0;
        }

        return ch_read;
    }

    int write_in(int dest_fd) {
        lock_.lock();
        if (size_ == 0) {
            lock_.unlock();
            write_p_.second.get();

            lock_.lock();
            read_p_.first = std::promise<void> ();
            write_p_.second = read_p_.first.get_future();
            read_pr_flag_ = true;
            lock_.unlock();

            write_offset_ = 0;
        }
        lock_.unlock();

        lock_.lock();
        int to_write = (size_ > block_size_)? block_size_ : size_;
        int ch_write = write(dest_fd, data_ + write_offset_, to_write);
        size_ -= ch_write;
        lock_.unlock();

        write_offset_ += ch_write;

        //fprintf(stderr, "\nWriter: ch_write = %d\n", ch_write);
        //dump(stderr);

        lock_.lock();
        if ((size_*100 < 20*capacity_) && write_pr_flag_) {
            write_p_.first.set_value();

            write_pr_flag_ = false;
        }
        lock_.unlock();

        lock_.lock();
        if (size_ == 0 && !is_reading) {
            lock_.unlock();
            return 0;
        }
        lock_.unlock();

        return ch_write;
    }
};


#endif //INC_2_BUFFER_H
