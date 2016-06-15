#include "helpers.h"

TEST(request_error_works) {
    request_t request = request_error();
    assertEquals(request.type, REQUEST_ERROR);
    request_unref(&request);
}

TEST(request_shutdown_works) {
    request_t request = request_shutdown();
    assertEquals(request.type, REQUEST_SHUTDOWN);
    request_unref(&request);
}

TEST(request_task_works) {
    int id = 3243;
    GBytes *data = g_bytes_new(NULL, 0);
    request_t request = request_task(data, id);
    assertEquals(request.type, REQUEST_TASK);
    assertEquals(request.data, data);
    assertEquals(request.id, id);
    request_unref(&request);
}
