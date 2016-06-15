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
    char data[] = {129, 164, 116, 121, 112, 101, 168, 115, 104, 117, 116, 100, 111, 119, 110};
    size_t data_len = 15;

    request_t request = request_parse(data, data_len);
    assertEquals(request.type, REQUEST_SHUTDOWN);
}
