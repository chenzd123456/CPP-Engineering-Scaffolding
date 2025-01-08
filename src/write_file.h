#ifndef __WRITE_FILE_H__
#define __WRITE_FILE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <unistd.h>

int write_file(const char *filename, const char *data, size_t data_size);

#ifdef __cplusplus
}
#endif

#endif // __WRITE_FILE_H__
