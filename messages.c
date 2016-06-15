#include "messages.h"

//request_t request_parse(const char *mesg, size_t length) {
request_t request_parse(GBytes *request) {
    // extract size and pointer from gbytes
    size_t size;
    const char *data = g_bytes_get_data(request, &size);

    // create new mpack tree to examine request
    mpack_tree_t msg;
    mpack_tree_init(&msg, data, size);
    mpack_node_t root = mpack_tree_root(&msg);

    // extract the "type" field from the message
    const char *type = mpack_node_str(mpack_node_map_cstr(root, "type"));

    // check for eventual errors
    if(mpack_node_error(root) != mpack_ok) {
        goto error;
    }

    if(strncmp(type, "task", 4) == 0) {
        int id = mpack_node_i32(mpack_node_map_cstr(root, "id"));
        mpack_node_t payload = mpack_node_map_cstr(root, "payload");
        const char *payload_data = mpack_node_data(payload);
        size_t payload_size = mpack_node_data_len(payload);
        GBytes *payload_bytes = g_bytes_new_static(payload_data, payload_size);

        if(mpack_node_error(root) != mpack_ok) {
            goto error;
        }

        return request_task(payload_bytes, id);
    } else if(strncmp(type, "shutdown", 8) == 0) {
        return request_shutdown();
    }

error:
    return request_error();
}

request_t request_shutdown() {
    return (request_t){
        .type = REQUEST_SHUTDOWN,
        .data = NULL,
        .id = 0
    };
}

request_t request_error() {
    return (request_t){
        .type = REQUEST_ERROR,
        .data = NULL,
        .id = 0
    };
}

request_t request_task(GBytes *data, int id) {
    return (request_t){
        .type = REQUEST_TASK,
        .data = data,
        .id = id
    };
}

GBytes *request_create(const request_t *request) {
    // we can't create an error message
    if(request->type == REQUEST_ERROR) {
        return NULL;
    }

    char *buffer;
    size_t size;
    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &buffer, &size);
    
    if(request->type == REQUEST_TASK) {
        mpack_start_map(&writer, 3);

        mpack_write_cstr(&writer, "type");
        mpack_write_cstr(&writer, "task");

        mpack_write_cstr(&writer, "id");
        mpack_write_i32(&writer, request->id);

        size_t data_len;
        const char *data = g_bytes_get_data(request->data, &data_len);
        mpack_write_cstr(&writer, "payload");
        mpack_write_bin(&writer, data, data_len);

        mpack_finish_map(&writer);
    } else if(request->type == REQUEST_SHUTDOWN) {
        mpack_start_map(&writer, 1);

        mpack_write_cstr(&writer, "type");
        mpack_write_cstr(&writer, "shutdown");

        mpack_finish_map(&writer);
    } 

    if(mpack_writer_destroy(&writer) != mpack_ok) {
        if(buffer) free(buffer);
        return NULL;
    }

    return g_bytes_new_with_free_func(buffer,size, free, buffer);
}

void request_unref(request_t *request) {
    if(request->type == REQUEST_TASK) {
        g_bytes_unref(request->data);
    }
}

response_t response_error() {
    return (response_t){
        .type = RESPONSE_ERROR,
        .data.solution = NULL,
        .id = 0
    };
}

response_t response_solution(GBytes *solution, int id) {
    return (response_t){
        .type = RESPONSE_SOLUTION,
        .data.solution = solution,
        .id = id
    };
}

response_t response_diverge(GList *diverges, int id) {
    return (response_t){
        .type = RESPONSE_DIVERGES,
        .data.diverges = diverges,
        .id = id
    };
}

void response_unref(response_t *response) {
    if(response->type == RESPONSE_SOLUTION) {
        g_bytes_unref(response->data.solution);
    }

    if(response->type == RESPONSE_DIVERGES) {
        g_list_free_full(response->data.diverges, (void (*)(void *))g_bytes_unref);
    }
}

/*
sudoku_status worker_handle_sudoku(sudoku_puzzle_t *in) {
    if(solve_simple(in)) {
        return (sudoku_status){
            .type = SUDOKU_SOLVED,
            .solution = in
        };
    } else {
        return (sudoku_status){
            .type = SUDOKU_DIVERGE,
            .diverges = solve_diverge(in)
        };
    }
}

void worker_response_solved(char *buffer, int *len, sudoku_puzzle_t *solution, int task_id) {
    // pack the solution into char array
    // FIXME: magic constant 92
    unsigned char packed[92];
    sudoku_puzzle_pack(packed, solution);

    // write to mpack format
    mpack_writer_t writer;
    mpack_writer_init(&writer, buffer, BUFSIZE);
    
    mpack_start_map(&writer, 3);

    // marks this as solved
    mpack_write_cstr(&writer, "solved");
    mpack_write_bool(&writer, true);

    // adds the solution sudoku
    // FIXME magic constant 92
    mpack_write_cstr(&writer, "solution");
    mpack_write_str(&writer, (char*) packed, 92);

    // adds the id of the sudoku
    mpack_write_cstr(&writer, "id");
    mpack_write_i32(&writer, task_id);

    mpack_finish_map(&writer);

    // write the length of the buffer
    *len = mpack_writer_buffer_used(&writer);

    if(mpack_writer_destroy(&writer) != mpack_ok) {
        fprintf(stderr, "an error occured while encoding the data");
    }
}

void worker_response_diverge(char *buffer, int *len, GList *diverges, int task_id) {
    // write to mpack format
    mpack_writer_t writer;
    mpack_writer_init(&writer, buffer, BUFSIZE);
    
    mpack_start_map(&writer, 3);

    // marks this as unsolved
    mpack_write_cstr(&writer, "solved");
    mpack_write_bool(&writer, false);

    // adds each divergin sudoku to response
    mpack_write_cstr(&writer, "diverges");
    mpack_start_array(&writer, g_list_length(diverges));
    for(GList *iter = diverges; iter != NULL; iter = iter->next) {
        // get current sudoku
        sudoku_puzzle_t *sudoku = iter->data;

        // pack the solution into char array
        // FIXME: magic constant 92
        unsigned char packed[92];
        sudoku_puzzle_pack(packed, sudoku);

        mpack_write_str(&writer, (char*) packed, 92);
    }

    mpack_finish_array(&writer);

    // adds the id of the divering sudoku
    mpack_write_cstr(&writer, "id");
    mpack_write_i32(&writer, task_id);

    mpack_finish_map(&writer);

    // write the length of the buffer
    *len = mpack_writer_buffer_used(&writer);

    if(mpack_writer_destroy(&writer) != mpack_ok) {
        fprintf(stderr, "an error occured while encoding the data");
    }
}
*/
