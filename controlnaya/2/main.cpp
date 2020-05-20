#include <cstdio>
#include <thread>
#include <mutex>
#include <future>
#include "buffer.h"

std::mutex lock;

const int max_val = 512;

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
    std::future<int> fut_write = pr_read.get_future();

    buffer buf = buffer(1024);

    std::thread thread_read([](buffer& buf, FILE* file_read, std::promise<int>& pr_read) {
        buf.change_flag(1);

        int flag = 0;
        int ch_read = 0;
        int cur_offset = 0;
        do {
            if (flag == 1) {
                PRINTF("\nReader: Can I write in buffer?\n\n");

                lock.lock();
                if (buf.size() > 0.2*buf.capacity()) {
                    PRINTF("\nReader: No, I can't.\n\n");

                    lock.unlock();
                    continue;
                }

                PRINTF("\nReader: Yes, configuring myself!\n\n");
                cur_offset = buf.size();
                lock.unlock();

                flag = 0;
            } else if (cur_offset >= buf.capacity()) {
                PRINTF("\nReader: Buffer is full. Waiting.\n\n");

                cur_offset = 0;
                flag = 1;

                continue;
            }

            lock.lock();
            ch_read = fread(buf.buf() + cur_offset, 1, max_val, file_read);
            buf.plus_size(ch_read);
            lock.unlock();

            cur_offset += ch_read;

            PRINTF("\nReader: read %d\n", ch_read);
            PRINTF("Reader: buf size: %d, my offset: %d\n\n", buf.size(), cur_offset);

            std::this_thread::sleep_for(std::chrono::seconds(1));
        } while (ch_read != 0);

        buf.change_flag(0);
    }, std::ref(buf), file_1, std::ref(pr_read));

    std::thread thread_write([](buffer& buf, FILE *file_write, std::future<int>& fut_write) {
        //std::this_thread::sleep_for(std::chrono::seconds(1));

        int ch_write = 0;
        int cur_offset = 0;
        int flag = 1;
        do {
            if (flag == 1) {
                PRINTF("\nWriter: Can I read from buffer?\n\n");

                lock.lock();
                if (buf.size() < 0.8*buf.capacity()) {
                    PRINTF("\nWriter: No, I can't.\n\n");

                    lock.unlock();
                    continue;
                }

                PRINTF("\nWriter: Yes, Let's go!.\n\n");
                lock.unlock();

                flag = 0;
            } else if (cur_offset >= buf.capacity()) {
                PRINTF("\nWriter: Buffer is empty. Waiting.\n\n");

                cur_offset = 0;
                flag = 1;

                continue;
            }

            lock.lock();
            int to_write = (buf.size() >= max_val)? max_val : buf.size();
            ch_write = fwrite(buf.buf() + cur_offset, 1, to_write, file_write);
            buf.minus_size(ch_write);
            lock.unlock();

            cur_offset += ch_write;

            PRINTF("\nWriter: wrote %d\n", ch_write);
            PRINTF("Writer: buf size: %d, my offset: %d\n\n", buf.size(), cur_offset);

            std::this_thread::sleep_for(std::chrono::seconds(1));
        } while (buf.size() != 0 || buf.flag() == 1);
    }, std::ref(buf), file_2, std::ref(fut_write));

    thread_read.join();
    thread_write.join();
}
