#include "write_file.h"

#include <fcntl.h>

int write_file(const char *filename, const char *data, size_t data_size) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }

    ssize_t bytes_written = write(fd, data, data_size);
    if (bytes_written < 0) {
        close(fd);
        return -1;
    }

    if (close(fd) < 0) {
        return -1;
    }

    return 0;
}
