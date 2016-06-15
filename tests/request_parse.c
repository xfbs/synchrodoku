#include "helpers.h"

TEST(parsing_empty_request_yields_error) {
    int i = 3;
    request_t request = request_parse((const char *)&i, 0);
    assertEquals(request.type, REQUEST_ERROR);
}

TEST(parsing_malformed_request_yields_error) {
    char data[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    request_t request = request_parse(data, 10);
    assertEquals(request.type, REQUEST_ERROR);
}

TEST(parsing_shutdown_request_works) {
    /* generated from ruby with:
     * {"type" => "shutdown"}.to_msgpack.bytes.to_a
     */
    char data[] = {129, 164, 116, 121, 112, 101, 168, 115, 104, 117, 116, 100, 111, 119, 110};
    size_t data_len = 15;

    request_t request = request_parse(data, data_len);
    assertEquals(request.type, REQUEST_SHUTDOWN);
}

TEST(parsing_task_request_works) {
    /* generated from ruby with:
     * {"type" => "task", "id" => 5, "payload" => "asdf"}.to_msgpack.bytes.to_a
     */
    char data[] = {
        131, 164, 116, 121, 112, 101, 164, 116, 
         97, 115, 107, 162, 105, 100,   5, 167, 
        112,  97, 121, 108, 111,  97, 100, 164, 
         97, 115, 100, 102};
    size_t data_len = 28;

    request_t request = request_parse(data, data_len);
    assertEquals(request.type, REQUEST_TASK);
    assertEquals(request.id, 5);
    
    size_t payload_size;
    const char *payload = g_bytes_get_data(request.data, &payload_size);
    assertEquals(payload_size, 4);
    assertEquals(strncmp(payload, "asdf", 4), 0);
}

