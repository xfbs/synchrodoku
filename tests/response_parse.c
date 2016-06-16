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

TEST(parsing_diverge_response_works) {
    // generated from ruby with:
    // {"done" => false, "divs" => ["asdf", "ghjk"], "id" => 432}.to_msgpack.bytes.to_a
    char data[] = {
        131, 164, 100, 111, 110, 101, 194, 164, 
        100, 105, 118, 115, 146, 164,  97, 115, 
        100, 102, 164, 103, 104, 106, 107, 162, 
        105, 100, 205,   1, 176};
    size_t data_len = 29;

    GBytes *bytes = g_bytes_new_static(data, data_len);

    response_t response = response_parse(bytes);
    assertEquals(response.type, RESPONSE_DIVERGES);
    assertEquals(response.id, 432);

    const char *payload;

    GBytes *first = g_list_nth_data(response.data.diverges, 0);
    payload = g_bytes_get_data(first, &data_len);
    assertEquals(data_len, 4);
    assertEquals(strncmp(payload, "asdf", data_len), 0);

    GBytes *second = g_list_nth_data(response.data.diverges, 1);
    payload = g_bytes_get_data(second, &data_len);
    assertEquals(data_len, 4);
    assertEquals(strncmp(payload, "ghjk", data_len), 0);

    g_bytes_unref(bytes);
    response_unref(&response);
}
