#include "helpers.h"

TEST(request_create_does_not_work_with_error) {
    request_t request = request_error();

    size_t size = 77;
    char *data = request_create(&size, &request);

    assertEquals(data, NULL);
    assertEquals(size, 0);
}

TEST(request_create_works_with_shutdown) {
    request_t request = request_shutdown();

    size_t size;
    char *data = request_create(&size, &request);
    assertNotEquals(size, 0);
    assertNotEquals(data, NULL);

    request_t parsed = request_parse(data, size);
    assertEquals(parsed.type, REQUEST_SHUTDOWN);
    free(data);
}

TEST(request_create_works_with_task) {
    char payload[] = {
        31, 23, 43, 23, -3, 43, 99, 12,
        43, 12, 32, 54, 43, 53, 12, 0,
        12, 32, 43, 54, 1,  12, 32, 0, 
        12, 23, 55, 33, 66, 77, 34, 99};
    size_t payload_size = 32;
    int id = 93;

    request_t request = request_task(payload, payload_size, id);

    size_t size;
    char *data = request_create(&size, &request);
    assertNotEquals(size, 0);
    assertNotEquals(data, NULL);

    request_t parsed = request_parse(data, size);
    assertEquals(parsed.type, REQUEST_TASK);
    assertEquals(parsed.id, id);
    assertEquals(parsed.size, payload_size);
    assertEquals(strncmp(parsed.payload, payload, payload_size), 0);
    free(data);
}
