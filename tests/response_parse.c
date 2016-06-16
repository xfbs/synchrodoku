#include "helpers.h"

TEST(parsing_empty_response_yields_error) {
    int i = 3;
    GBytes *res = g_bytes_new_static((const char *)&i, 0);

    response_t response = response_parse(res);

    assertEquals(response.type, RESPONSE_ERROR);
    response_unref(&response);
}

TEST(parsing_malformed_response_yields_error) {
    char data[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    GBytes *res = g_bytes_new_static(data, 10);

    response_t response = response_parse(res);

    assertEquals(response.type, RESPONSE_ERROR);
    response_unref(&response);
}

TEST(parsing_solution_response_works) {
    // generated from ruby with:
    // {"done" => true, "data" => "asdf", "id" => 345}.to_msgpack.bytes.to_a
    char data[] = {
        131, 164, 100, 111, 110, 101, 195, 164, 
        100, 97, 116, 97, 164, 97, 115, 100, 
        102, 162, 105, 100, 205, 1, 89};
    size_t data_len = 23;

    GBytes *bytes = g_bytes_new_static(data, data_len);

    response_t response = response_parse(bytes);
    assertEquals(response.type, RESPONSE_SOLUTION);
    assertEquals(response.id, 345);

    const char *payload = g_bytes_get_data(response.data.solution, &data_len);
    assertEquals(data_len, 4);
    assertEquals(strncmp(payload, "asdf", data_len), 0);

    g_bytes_unref(bytes);
    response_unref(&response);
}

/*
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
    // generated from ruby with:
    // {"type" => "task", "id" => 5, "payload" => "asdf"}.to_msgpack.bytes.to_a
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
*/
