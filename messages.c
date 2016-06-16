#include "messages.h"

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
        mpack_node_t payload = mpack_node_map_cstr(root, "data");
        const char *payload_data = mpack_node_data(payload);
        size_t payload_size = mpack_node_data_len(payload);
        GBytes *payload_bytes = g_bytes_new_static(payload_data, payload_size);

        if(mpack_node_error(root) != mpack_ok) {
            goto error;
        }

        return request_task(payload_bytes, id);
    } else if(strncmp(type, "stop", 4) == 0) {
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
        mpack_write_cstr(&writer, "data");
        mpack_write_bin(&writer, data, data_len);

        mpack_finish_map(&writer);
    } else if(request->type == REQUEST_SHUTDOWN) {
        mpack_start_map(&writer, 1);

        mpack_write_cstr(&writer, "type");
        mpack_write_cstr(&writer, "stop");

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

response_t response_parse(GBytes *request) {
    // extract size and pointer from gbytes
    size_t size;
    const char *data = g_bytes_get_data(request, &size);

    // create new mpack tree to examine request
    mpack_tree_t msg;
    mpack_tree_init(&msg, data, size);
    mpack_node_t root = mpack_tree_root(&msg);

    // extract the "type" field from the message
    // check if this is a solution or not
    bool done = mpack_node_bool(mpack_node_map_cstr(root, "done"));

    // extract id
    int id = mpack_node_i32(mpack_node_map_cstr(root, "id"));

    // check for eventual errors
    if(mpack_node_error(root) != mpack_ok) {
        goto error;
    }

    if(done == true) {
        // extract data
        mpack_node_t payload = mpack_node_map_cstr(root, "data");
        const char *payload_data = mpack_node_data(payload);
        size_t payload_size = mpack_node_data_len(payload);
        GBytes *payload_bytes = g_bytes_new_static(payload_data, payload_size);

        if(mpack_node_error(root) != mpack_ok) {
            goto error;
        }

        return response_solution(payload_bytes, id);
    } else {
        mpack_node_t divs_node = mpack_node_map_cstr(root, "divs");
        GList *divs_list = NULL;

        for(int i = 0; i < mpack_node_array_length(divs_node); i++) {
            mpack_node_t cur_node = mpack_node_array_at(divs_node, i);

            const char *payload_data = mpack_node_data(cur_node);
            size_t payload_size = mpack_node_data_len(cur_node);
            GBytes *payload_bytes = g_bytes_new_static(payload_data, payload_size);

            divs_list = g_list_append(divs_list, payload_bytes);
        }

        if(mpack_node_error(root) != mpack_ok) {
            goto error;
        }

        return response_diverge(divs_list, id);
    }

error:
    return response_error();
}

GBytes *response_create(const response_t *response) {
    if(response->type == RESPONSE_ERROR) {
        return NULL;
    }

    char *data;
    size_t size;

    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &data, &size);

    mpack_start_map(&writer, 3);

    if(response->type == RESPONSE_SOLUTION) {
        mpack_write_cstr(&writer, "done");
        mpack_write_bool(&writer, true);

        size_t solution_len;
        const char *solution = g_bytes_get_data(response->data.solution, &solution_len);
        mpack_write_cstr(&writer, "data");
        mpack_write_bin(&writer, solution, solution_len);
    } else if(response->type == RESPONSE_DIVERGES) {
        mpack_write_cstr(&writer, "done");
        mpack_write_bool(&writer, false);

        GList *diverges = response->data.diverges;
        mpack_write_cstr(&writer, "divs");
        mpack_start_array(&writer, g_list_length(diverges));

        for(; diverges != NULL; diverges = diverges->next) {
            size_t diverge_len;
            const char *diverge = g_bytes_get_data(diverges->data, &diverge_len);
            mpack_write_bin(&writer, diverge, diverge_len);
        }

        mpack_finish_array(&writer);
    }

    mpack_write_cstr(&writer, "id");
    mpack_write_i32(&writer, response->id);

    mpack_finish_map(&writer);

    if(mpack_writer_destroy(&writer) != mpack_ok) {
        if(data) free(data);
        return NULL;
    }

    return g_bytes_new_with_free_func(data, size, free, data);
}
