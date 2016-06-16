#include "helpers.h"

TEST(request_create_does_not_work_with_error) {
    request_t request = request_error();
    GBytes *data = request_create(&request);
    assertEquals(data, NULL);
    request_unref(&request);
}

TEST(request_create_works_with_shutdown) {
    request_t request = request_shutdown();

    GBytes *data = request_create(&request);
    assertNotEquals(data, NULL);
    assertNotEquals(g_bytes_get_size(data), 0);

    request_t parsed = request_parse(data);
    assertEquals(parsed.type, REQUEST_SHUTDOWN);

    g_bytes_unref(data);
    request_unref(&request);
    request_unref(&parsed);
}

TEST(request_create_works_with_task) {
    char payload[] = {
        31, 23, 43, 23, -3, 43, 99, 12,
        43, 12, 32, 54, 43, 53, 12, 0,
        12, 32, 43, 54, 1,  12, 32, 0, 
        12, 23, 55, 33, 66, 77, 34, 99};
    size_t payload_size = 32;
    int id = 93;
    GBytes *payload_bytes = g_bytes_new_static(payload, payload_size);

    request_t request = request_task(payload_bytes, id);

    GBytes *data = request_create(&request);
    assertNotEquals(data, NULL);
    assertNotEquals(g_bytes_get_size(data), 0);

    request_t parsed = request_parse(data);
    assertEquals(parsed.type, REQUEST_TASK);
    assertEquals(parsed.id, id);

    size_t size;
    const char *parsed_payload = g_bytes_get_data(parsed.data, &size);
    assertEquals(size, payload_size);
    assertEquals(strncmp(parsed_payload, payload, payload_size), 0);

    g_bytes_unref(data);
    request_unref(&parsed);
    request_unref(&request);
}
