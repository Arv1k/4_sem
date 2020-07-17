#include <cstdio>
#include <thread>
#include <mutex>
#include <future>
#include "../buffer.h"


/*#ifdef DEBUG
//#define fprintf(stderr, ) fprintf(stderr,
#else
#define PRINTF ;
#endif*/


const int max_val = 4;

std::mutex lock;

void cp_src_to_dest (FILE* file_1, FILE* file_2, buffer& buf);

int main(int argc, char** argv) {
    buffer buf(1024);

    if (argc == 1) {
        cp_src_to_dest(stdin, stdout, buf);
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        FILE *file_1 = fopen(argv[i], "rb");

        cp_src_to_dest(file_1, stdout, buf);
        if (i != argc - 1) {
            fprintf(stderr, "\n");
        }
    }
}

void cp_src_to_dest (FILE* file_1, FILE* file_2, buffer& buf) {
    std::thread thread_read([](buffer &buf, FILE *file_read) {
        buf.change_flag(1);

        int ch_read = 0;
        int cur_offset = 0;
        do {
            if (cur_offset >= buf.capacity()) {
                fprintf(stderr ,"\nReader: Oops, waiting\n\n");

                buf.read_get();
                cur_offset = 0;

                //lock.lock();
                buf.flag_write = 0;
                //lock.unlock();
            }

            int to_read = (buf.capacity() - buf.size() >= max_val) ?
                          max_val : buf.capacity() - buf.size();

            ch_read = fread(buf.buf() + cur_offset,1, to_read, file_read);
            buf.plus_size(ch_read);
            cur_offset += ch_read;

            fprintf(stderr, "\nReader: read %d\n", ch_read);
            fprintf(stderr, "Reader: buf size: %d, my offset: %d\n\n",
                    buf.size(), cur_offset);

            //lock.lock();
            if ((buf.size()*100 > 80*buf.capacity()) && buf.flag_read != 1) {
                fprintf(stderr, "\nReader: Calling Writer!\n\n");
                buf.read_set();


                buf.flag_read = 1;
            }
            //lock.unlock();

        } while (ch_read != 0);

        if (buf.size() != 0 && buf.flag() != 1) {
            buf.read_set();
        }

        buf.change_flag(0);
    }, std::ref(buf), file_1);

    std::thread thread_write([](buffer &buf, FILE *file_write) {
        int ch_write = 0;
        int cur_offset = 0;

        int flag = 0;
        do {
            if (buf.size() == 0) {
                fprintf(stderr, "\nWriter: Oops, waiting x1\n\n");

                buf.write_get();
                cur_offset = 0;

                //lock.lock();
                buf.flag_read = 0;
                //lock.unlock();
            } /*else if (cur_offset >= buf.capacity()) {
                fprintf(stderr, "\nWriter: Oops, waiting x2\n\n");

                buf.write_get();
                cur_offset = 0;

                //lock.lock();
                buf.flag_read = 0;
                //lock.unlock();
            }*/

            int to_write = (buf.size() >= max_val) ? max_val : buf.size();
            ch_write = fwrite(buf.buf() + cur_offset, 1,
                              to_write, file_write);

            buf.minus_size(ch_write);
            cur_offset += ch_write;

            fprintf(stderr, "\nWriter: wrote %d\n", ch_write);
            fprintf(stderr, "Writer: buf size: %d, my offset: %d\n\n",
                    buf.size(), cur_offset);

            //lock.lock();
            if ((buf.size() * 100 < 20 * buf.capacity()) && buf.flag_write != 1) {
                fprintf(stderr, "\nWriter: Calling Reader!\n\n");

                buf.write_set();

                buf.flag_write = 1;
            }
            //lock.unlock();
        } while (buf.size() != 0 || buf.flag() == 1);
    }, std::ref(buf), file_2);

    thread_read.join();
    thread_write.join();
}
