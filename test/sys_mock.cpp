
#include "sys_mock.h"

SystemApiMock* SystemApiSingleton::instancePtr = nullptr;

SystemApiMock& SystemApiSingleton::instance() {
    static SystemApiMock instance;
    return instance;
}

void SystemApiSingleton::setMock(SystemApiMock* mock) {
    instancePtr = mock;
}

SystemApiMock* SystemApiSingleton::getMock() {
    return instancePtr;
}

int __wrap_open(const char *pathname, int flags, mode_t mode) {
    auto* api = SystemApiSingleton::getMock();
    if (!api)
        return __real_open(pathname, flags, mode);
    return api->open(pathname, flags, mode);
}

ssize_t __wrap_write(int fd, const void *buf, size_t count) {
    auto* api = SystemApiSingleton::getMock();
    if (!api)
        return __real_write(fd, buf, count);
    return api->write(fd, buf, count);
}

int __wrap_close(int fd) {
    auto* api = SystemApiSingleton::getMock();
    if (!api)
        return __real_close(fd);
    return api->close(fd);
}
