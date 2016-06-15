#include "helpers.h"

TEST(response_error_works) {
    response_t response = response_error();
    assertEquals(response.type, RESPONSE_ERROR);

    response_unref(&response);
}

TEST(response_solution_works) {
    int id = 234;
    GBytes *bytes = g_bytes_new(NULL, 0);
    
    response_t response = response_solution(bytes, id);
    
    assertEquals(response.type, RESPONSE_SOLUTION);
    assertEquals(response.data.solution, bytes);
    assertEquals(response.id, id);

    response_unref(&response);
}

TEST(response_diverge_works) {
    int id = 234;
    GList *diverges = g_list_alloc();
    diverges->data = g_bytes_new(NULL, 0);
    
    response_t response = response_diverge(diverges, id);

    assertEquals(response.type, RESPONSE_DIVERGES);
    assertEquals(response.data.diverges, diverges);
    assertEquals(response.id, id);

    response_unref(&response);
}
