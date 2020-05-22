#include <cstdio>
#include <thread>
#include <mutex>
#include <future>
#include "buffer.h"

std::mutex lock;

const int max_val = 1024;

    #define DEBUG

#ifdef DEBUG
#define PRINTF printf
#else
#define PRINTF ;
#endif

int main(int argc, char** argv) {
    FILE* file_1 = fopen(argv[1], "rb");
    FILE* file_2 = stdout;

    std::promise<int> pr_read;
    std::promise<int> pr_write;

    std::future<int> fut_read = pr_write.get_future();
    std::future<int> fut_write = pr_read.get_future();

    buffer buf = buffer(1096);

    std::thread thread_read([](buffer& buf, FILE* file_read,
                               std::promise<int>& pr_read,
                               std::future<int>& fut_write) {
        buf.change_flag(1);

        int ch_read = 0;
        int cur_offset = 0;
        do {
            lock.lock();
            ch_read = fread(buf.buf() + cur_offset, 1, max_val, file_read);
            buf.plus_size(ch_read);
            cur_offset += ch_read;

            PRINTF("\nReader: read %d\n", ch_read);
            PRINTF("Reader: buf size: %d, my offset: %d\n\n", buf.size(), cur_offset);

            if (buf.size() > 0.8*buf.capacity()) {
                pr_read.set_exception(0);
                pr_read.set_exception(0);
            }
            lock.unlock();

            if (cur_offset >= buf.capacity()) {
                fut_write.wait();

                cur_offset = 0;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        } while (ch_read != 0);

        buf.change_flag(0);
    }, std::ref(buf), file_1, std::ref(pr_read), std::ref(fut_write));

    std::thread thread_write([](buffer& buf, FILE *file_write,
                                std::promise<int>& pr_write,
                                std::future<int>& fut_read) {
        //std::this_thread::sleep_for(std::chrono::seconds(1));

        int ch_write = 0;
        int cur_offset = 0;
        int flag = 1;
        do {
            /*lock.lock();
            if (buf.size() == 0) {
                lock.unlock();

                fut_read.valid();

                cur_offset = 0;
            }*/

            lock.lock();
            int to_write = (buf.size() >= max_val)? max_val : buf.size();
            ch_write = fwrite(buf.buf() + cur_offset, 1, to_write, file_write);
            buf.minus_size(ch_write);
            cur_offset += ch_write;

            if (buf.size() < 0.2*buf.capacity()) {
                pr_write.set_exception(0);
            }
            lock.unlock();

            if (cur_offset >= buf.capacity()) {
                fut_read.wait();

                cur_offset = 0;
            }

            PRINTF("\nWriter: wrote %d\n", ch_write);
            PRINTF("Writer: buf size: %d, my offset: %d\n\n", buf.size(), cur_offset);

            std::this_thread::sleep_for(std::chrono::seconds(1));
        } while (buf.size() != 0 || buf.flag() == 1);
    }, std::ref(buf), file_2, std::ref(pr_write), std::ref(fut_read));

    thread_read.join();
    thread_write.join();
}
