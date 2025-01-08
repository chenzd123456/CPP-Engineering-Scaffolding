

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>

#include <cmocka.h>

#include "write_file.h"
// #include "gtest/gtest.h"

// Mock functions
int __wrap_open(const char *pathname, int flags, mode_t mode) {
    check_expected(pathname); // 检查传入的参数是否符合预期
    check_expected(flags);
    check_expected(mode);
    return mock_type(int);
}

ssize_t __wrap_write(int fd, const void *buf, size_t count) {
    check_expected(fd); // 检查传入的参数是否符合预期
    check_expected(buf);
    check_expected(count);
    return mock_type(ssize_t);
}

int __wrap_close(int fd) {
    check_expected(fd); // 检查传入的参数是否符合预期
    return mock_type(int);
}

static void WriteFileTest_SuccessCase(void **state) {
    (void)state; // unused

    // Setup mock expectations
    expect_string(__wrap_open, pathname, "test.txt");
    expect_value(__wrap_open, flags, O_WRONLY | O_CREAT | O_TRUNC);
    expect_value(__wrap_open, mode, 0644);
    will_return(__wrap_open, 3);   // return fd=3
    
    expect_value(__wrap_write, fd, 3);
    expect_memory(__wrap_write, buf, "test data", 10);
    expect_value(__wrap_write, count, 10);
    will_return(__wrap_write, 10); // write 10 bytes
    
    expect_value(__wrap_close, fd, 3);
    will_return(__wrap_close, 0);  // close success

    const char *filename = "test.txt";
    const char *data = "test data";
    size_t data_size = 10;

    // Execute
    int result = write_file(filename, data, data_size);

    // Verify
    assert_int_equal(result, 0);
}

// TEST(WriteFileTest, OpenFailure) {
//     // Setup mock expectations
//     will_return(__wrap_open, -1); // open failure

//     const char *filename = "test.txt";
//     const char *data = "test data";
//     size_t data_size = 10;

//     // Execute
//     int result = write_file(filename, data, data_size);

//     // Verify
//     ASSERT_EQ(result, -1);
// }

// TEST(WriteFileTest, WriteFailure) {
//     // Setup mock expectations
//     will_return(__wrap_open, 3);   // return fd=3
//     will_return(__wrap_write, -1); // write failure
//     will_return(__wrap_close, 0);  // close success

//     const char *filename = "test.txt";
//     const char *data = "test data";
//     size_t data_size = 10;

//     // Execute
//     int result = write_file(filename, data, data_size);

//     // Verify
//     ASSERT_EQ(result, -1);
// }

int main(int argc, char **argv) {
    (void)argc; // unused
    (void)argv; // unused

    printf("start cmocka\n");
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(WriteFileTest_SuccessCase),
        // cmocka_unit_test(WriteFileTest, OpenFailure),
        // cmocka_unit_test(WriteFileTest, WriteFailure),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
