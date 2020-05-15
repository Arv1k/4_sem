#include <cstdio>
#include <thread>
#include <mutex>

std::mutex lock;

int main(int argc, char** argv) {
    FILE* file_1 = fopen(argv[1], "rb");
    FILE* file_2 = stdout;

    char buf[1024*1024];

    std::thread thread_read([](char* buf, FILE* file_read) {
        int ch_read;
        do {
            lock.lock();
            ch_read = fread(buf, 1, 16, file_read);
            printf("Eto ya read\n");
            lock.unlock();
        } while (ch_read != 0);
    }, buf, file_1);
    thread_read.detach();

    std::thread thread_write([](char* buf, FILE* file_write) {
        int ch_write;
        do {
            lock.lock();
            ch_write = fwrite(buf, 1, 16, file_write);
            printf("Eto ya write\n");
            lock.unlock();
        } while (ch_write != 0);
    }, buf, file_2);
    thread_write.detach();

    printf("A ya jdu\n");

    /*for (;;) {
        int ch_read = fread(buf, 1, 16, file_1);

        if (ch_read == 0) break;

        int ch_write = fwrite(buf, 1, 16, file_2);
        if (ch_write < 0) {
            perror("Can't write the data! ");

            break;
        }
    }*/
}