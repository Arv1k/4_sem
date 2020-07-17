#include <cstdio>
#include <thread>
#include <mutex>
#include <future>
#include <unistd.h>
#include <fcntl.h>

#include "buffer.h"


enum std_fd {
    STD_OUT      = 1,
    STD_IN       = 0,
};

enum constants {
    BLOCK_SIZE = 32,
    BUFFER_SIZE = 512,
};


int cp_src_to_dest(int src_fd, int dest_fd, buffer& buffer);

int main(int argc, char** argv) {
    buffer buffer(BUFFER_SIZE, BLOCK_SIZE);

    int num_args = (argc > 1) ? (argc - 1) : 1;
    for (int i = 1; i <= num_args; i++) {
        int src_fd = 0;

        if (argc == 1) {
            src_fd = STD_IN;
        } else {
            src_fd = open(argv[i], O_RDONLY);
        }

        cp_src_to_dest(src_fd, STD_OUT, buffer);
    }
}

int cp_src_to_dest(int src_fd, int dest_fd, buffer& buffer) {
    std::thread read_thread([](class buffer& buffer, int src_fd) {
        buffer.is_reading = true;
        for (;;) {
            int ch_read = buffer.read_from(src_fd);

            if (ch_read == 0) {
                break;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        buffer.is_reading = false;
    }, std::ref(buffer), src_fd);

    std::thread write_thread([](class buffer& buffer, int dest_fd) {
        for (;;) {
            int ch_write = buffer.write_in(dest_fd);

            if (ch_write == 0) {
                break;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }, std::ref(buffer), dest_fd);


    read_thread.join();
    write_thread.join();
    return 1;
}