#include <cstdio>
#include <thread>
#include <mutex>
#include <future>
#include "buffer.h"

std::mutex lock;

const int max_val = 512;

int main(int argc, char** argv) {
    FILE* file_1 = fopen(argv[1], "rb");
    FILE* file_2 = stdout;

    std::promise<int> pr_read;
    std::future<int> fut_write = pr_read.get_future();

    buffer buf = buffer(1024);

    std::thread thread_read([](buffer& buf, FILE* file_read, std::promise<int>& pr_read) {
        int ch_read = 0;
        int cur_offset = 0;
        buf.change_flag(1);
        int flag = 0;

        do {
            if (flag == 1) {
                lock.lock();
                if (buf.size() < 0.2*buf.capacity()) {
                    continue;
                }
                cur_offset = buf.size();
                lock.unlock();
                flag = 0;
            } else if (cur_offset >= buf.capacity()) {
                cur_offset = 0;
                flag = 1;

                continue;
            }

            ch_read = fread(buf.buf() + cur_offset, 1, max_val, file_read);
            buf.plus_size(ch_read);
            cur_offset += ch_read;

            //printf("\nEto ya read %d\n", ch_read);
            //printf("buf size: %d, my offset: %d\n\n", buf.size(), cur_offset);

            //std::this_thread::sleep_for(std::chrono::seconds(1));
            //lock.unlock();
        } while (ch_read != 0);

        buf.change_flag(0);
        // pr_read.set_value(100);
    }, std::ref(buf), file_1, std::ref(pr_read));

    std::thread thread_write([](buffer& buf, FILE *file_write, std::future<int>& fut_write) {
        //std::this_thread::sleep_for(std::chrono::seconds(1));

        int ch_write = 0;
        int cur_offset = 0;
        int flag = 0;
        do {
            //lock.lock();
            if (flag == 1) {
                lock.unlock();
                if (buf.size() < 0.8*buf.capacity()) {
                    continue;
                }
                lock.unlock();
                flag = 0;
            } else if (cur_offset >= buf.capacity()) {
                cur_offset = 0;
                flag = 1;

                continue;
            }

            int to_write = (buf.size() >= max_val)? max_val : buf.size();
            ch_write = fwrite(buf.buf() + cur_offset, 1, to_write, file_write);
            buf.minus_size(ch_write);
            cur_offset += ch_write;

            //printf("\n\nEto ya write %d\n", ch_write);
            //printf("buf size: %d, my offset: %d\n\n", buf.size(), cur_offset);

            //std::this_thread::sleep_for(std::chrono::seconds(1));
            //lock.unlock();
        } while (buf.size() != 0 || buf.flag() == 1);
    }, std::ref(buf), file_2, std::ref(fut_write));

    //printf("A ya jdu\n");
    thread_read.join();
    thread_write.join();
}
