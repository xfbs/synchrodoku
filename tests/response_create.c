#include "helpers.h"

TEST(response_create_does_not_work_with_error) {
    response_t response = response_error();
    GBytes *data = response_create(&response);
    
    assertEquals(data, NULL);
    response_unref(&response);
}

TEST(response_create_works_with_solution) {
    char solution_data[] = {
        31, 23, 43, 23, -3, 43, 99, 12,
        43, 12, 32, 54, 43, 53, 12, 0,
        12, 32, 43, 54, 1,  12, 32, 0, 
        12, 23, 55, 33, 66, 77, 34, 99};
    size_t solution_size = 32;
    int id = 93;
    
    GBytes *solution = g_bytes_new_static(solution_data, solution_size);

    response_t response = response_solution(solution, id);
    GBytes *data = response_create(&response);

    assertNotEquals(data, NULL);
    assertNotEquals(g_bytes_get_size(data), 0);

    response_t parsed = response_parse(data);
    assertEquals(parsed.type, RESPONSE_SOLUTION);
    assertEquals(parsed.id, id);

    size_t size;
    const char *parsed_solution = g_bytes_get_data(response.data.solution, &size);
    assertEquals(size, solution_size);
    assertEquals(strncmp(parsed_solution, solution_data, solution_size), 0);

    g_bytes_unref(data);
    g_bytes_unref(solution);
    response_unref(&parsed);
    response_unref(&response);
}
