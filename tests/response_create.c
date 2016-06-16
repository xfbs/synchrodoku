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
    response_unref(&parsed);
    response_unref(&response);
}

TEST(response_create_works_with_diverge) {
    char diverge_one_data[] = {
        31, 23, 43, 23, -3, 43, 99, 12,
        43, 12, 32, 54, 43, 53, 12, 0,
        12, 32, 43, 54, 1,  12, 32, 0, 
        12, 23, 55, 33, 66, 77, 34, 99};
    char diverge_two_data[] = {
        12, 32, 43, 54, 1,  12, 32, 0, 
        43, 12, 32, 54, 43, 53, 12, 4};
    size_t diverge_one_size = 32;
    size_t diverge_two_size = 16;
    int id = 93;
    
    GBytes *diverge_one = g_bytes_new_static(diverge_one_data, diverge_one_size);
    GBytes *diverge_two = g_bytes_new_static(diverge_two_data, diverge_two_size);

    GList *diverges = NULL;
    diverges = g_list_append(diverges, diverge_one);
    diverges = g_list_append(diverges, diverge_two);

    response_t response = response_diverge(diverges, id);
    GBytes *data = response_create(&response);

    assertNotEquals(data, NULL);
    assertNotEquals(g_bytes_get_size(data), 0);

    response_t parsed = response_parse(data);
    assertEquals(parsed.type, RESPONSE_DIVERGES);
    assertEquals(parsed.id, id);

    GList *parsed_diverges = parsed.data.diverges;
    GBytes *parsed_one = g_list_nth_data(parsed_diverges, 0);
    GBytes *parsed_two = g_list_nth_data(parsed_diverges, 1);
    const char *parsed_diverge;
    size_t size;

    parsed_diverge = g_bytes_get_data(parsed_one, &size);
    assertEquals(size, diverge_one_size);
    assertEquals(strncmp(parsed_diverge, diverge_one_data, size), 0);

    parsed_diverge = g_bytes_get_data(parsed_two, &size);
    assertEquals(size, diverge_two_size);
    assertEquals(strncmp(parsed_diverge, diverge_two_data, size), 0);

    g_bytes_unref(data);
    response_unref(&parsed);
    response_unref(&response);
}
