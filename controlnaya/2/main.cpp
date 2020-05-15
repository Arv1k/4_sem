#include <unistd.h>
#include <malloc.h>
#include <fcntl.h>


enum {
    STD_OUT      = 1,
    STD_IN       = 0,
};


int save_write(int fd, char* buf, int ch_read);
void cp_src_to_dest (int fd_src, int fd_dest);


int main(int argc, char** argv) {
    int num_files = (argc > 1) ? (argc - 1) : 1;

    for (int i = 1; i <= num_files; i++) {
        int in_fd = 0;

        if (argc == 1)
            in_fd = STD_IN;
        else {
            in_fd = open(argv[i], O_RDONLY);
            if (in_fd < 0) {
                perror("Can't open the file! ");

                continue;
            }
        }

        //int fd_out = STD_OUT;
        cp_src_to_dest (in_fd, STD_OUT);
    }
}

const int MAX_STR_SIZE = 1024*1024;
void cp_src_to_dest (int fd_src, int fd_dest) {
    for (;;) {
        char buf[MAX_STR_SIZE];
        int ch_read = read(fd_src, buf, MAX_STR_SIZE);
        if(ch_read < 0) {
            perror("Can't read the data! ");

            break;
        }

        if (ch_read == 0) break;

        int ch_write = save_write(fd_dest, buf, ch_read);
        if(ch_write < 0) {
            perror("Can't write the data! ");

            break;
        }
    }

    close(fd_src);
}

int save_write(int fd, char* buf, int ch_read) {
    int ch_write     = 0;
    int ch_to_write  = ch_read;
    int cur_ch_write = 0;

    for (;;) {
        cur_ch_write = write(fd, &buf[ch_write], ch_to_write);

        if (ch_write == ch_read) return ch_write;
        if (cur_ch_write < 0)    return -1;

        ch_write    += cur_ch_write;
        ch_to_write -= ch_write;
    }
}
