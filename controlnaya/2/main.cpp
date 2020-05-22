#include <cstdio>
#include <thread>
#include <mutex>
#include <future>
#include "buffer.h"


//#define DEBUG


#ifdef DEBUG
#define PRINTF printf
#else
#define PRINTF ;
#endif

const int max_val = 512;

int main(int argc, char** argv) {
    FILE* file_1 = fopen(argv[1], "rb");
    FILE* file_2 = stdout;

    buffer buf(1024);

    std::thread thread_read([](buffer& buf, FILE* file_read) {
        buf.change_flag(1);

        int ch_read = 0;
        int cur_offset = 0;
        do {
            if (cur_offset >= buf.capacity()) {
                PRINTF("\nReader: Oops, waiting\n\n");
                buf.read_get();

                cur_offset = 0;
            }

            int to_read = (buf.capacity() - buf.size() >= max_val)? max_val : buf.capacity() - buf.size();
            ch_read = fread(buf.buf() + cur_offset, 1, to_read, file_read);
            buf.plus_size(ch_read);
            cur_offset += ch_read;

            PRINTF("\nReader: read %d\n", ch_read);
            PRINTF("Reader: buf size: %d, my offset: %d\n\n", buf.size(), cur_offset);

            if (buf.size()*100 > 80*buf.capacity()) {
                PRINTF("\nReader: Calling Writer!\n\n");
                buf.read_set();
            }

            //std::this_thread::sleep_for(std::chrono::seconds(1));
        } while (ch_read != 0);

        if (buf.size() != 0) {
            buf.read_set();
        }

        buf.change_flag(0);
    }, std::ref(buf), file_1);

    std::thread thread_write([](buffer& buf, FILE *file_write) {
        int ch_write = 0;
        int cur_offset = 0;
        do {
            if (buf.size() == 0) {
                PRINTF("\nWriter: Oops, waiting x1\n\n");
                buf.write_get();

                cur_offset = 0;
            }

            if (cur_offset >= buf.capacity()) {
                PRINTF("\nWriter: Oops, waiting x2\n\n");
                buf.write_get();

                cur_offset = 0;
            }

            int to_write = (buf.size() >= max_val)? max_val : buf.size();
            ch_write = fwrite(buf.buf() + cur_offset, 1, to_write, file_write);
            buf.minus_size(ch_write);
            cur_offset += ch_write;

            PRINTF("\nWriter: wrote %d\n", ch_write);
            PRINTF("Writer: buf size: %d, my offset: %d\n\n", buf.size(), cur_offset);

            if (buf.size()*100 < 20*buf.capacity()) {
                PRINTF("\nWriter: Calling Reader!\n\n");
                buf.write_set();
            }

            //std::this_thread::sleep_for(std::chrono::seconds(1));
        } while (buf.size() != 0 || buf.flag() == 1);
    }, std::ref(buf), file_2);

    thread_read.join();
    thread_write.join();
}
