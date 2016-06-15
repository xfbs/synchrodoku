#include "helpers.h"

TEST(parsing_empty_request_yields_error) {
    int i = 3;
    GBytes *req = g_bytes_new_static((const char *)&i, 0);
    request_t request = request_parse(req);
    assertEquals(request.type, REQUEST_ERROR);
    request_unref(&request);
}

TEST(parsing_malformed_request_yields_error) {
    char data[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    GBytes *req = g_bytes_new_static(data, 10);
    request_t request = request_parse(req);
    assertEquals(request.type, REQUEST_ERROR);
    request_unref(&request);
}

TEST(parsing_shutdown_request_works) {
    // generated from ruby with:
    // {"type" => "stop"}.to_msgpack.bytes.to_a
    char data[] = {129, 164, 116, 121, 112, 101, 164, 115, 116, 111, 112};
    size_t data_len = 11;
    GBytes *bytes = g_bytes_new_static(data, data_len);

    request_t request = request_parse(bytes);
    assertEquals(request.type, REQUEST_SHUTDOWN);

    request_unref(&request);
}

TEST(parsing_task_request_works) {
    /* generated from ruby with:
     * {"type" => "task", "id" => 5, "payload" => "asdf"}.to_msgpack.bytes.to_a
     */
    char data[] = {
        131, 164, 116, 121, 112, 101, 164, 116, 97, 
        115, 107, 162, 105, 100, 5, 164, 100,
        97, 116, 97, 164, 97, 115, 100, 102};
    size_t data_len = 25;
    GBytes *bytes = g_bytes_new_static(data, data_len);

    request_t request = request_parse(bytes);
    assertEquals(request.type, REQUEST_TASK);
    assertEquals(request.id, 5);
    
    size_t payload_size;
    const char *payload = g_bytes_get_data(request.data, &payload_size);
    assertEquals(payload_size, 4);
    assertEquals(strncmp(payload, "asdf", 4), 0);

    request_unref(&request);
}

