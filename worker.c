#include "worker.h"
#define BUFSIZE 1024

typedef enum {
    SUDOKU_SOLVED,
    SUDOKU_DIVERGE
} sudoku_status_type;

typedef enum {
    MESG_SHUTDOWN,
    MESG_TASK,
    MESG_ERROR,
} worker_mesg_type;

typedef struct {
    worker_mesg_type type;
    sudoku_puzzle_t sudoku;
    int id;
} worker_task;
    
typedef struct {
    sudoku_status_type type;
    union {
        GList *diverges;
        sudoku_puzzle_t *solution;
    };
} sudoku_status;

worker_task worker_handle_mesg(char *mesg, int length);

sudoku_status worker_handle_sudoku(sudoku_puzzle_t *in);

void worker_response_solved(char *buffer, int *len, sudoku_puzzle_t *solution, int task_id);

void worker_response_diverge(char *buffer, int *len, GList *diverges, int task_id);

void *
worker_loop(void *opts)
{
    // the ID of the worker is passed as
    // only argument.
    int worker_id = (int) opts;

    // this is the socket over which the 
    // worker gets tasks sent to by the
    // manager
    void *tasks = zmq_socket(context, ZMQ_PULL);

    // this is the socket it uses to
    // reply back to the manager
    void *responses = zmq_socket(context, ZMQ_PUSH);

#ifdef DEBUG
    printf("[worker #%i] started up\n", worker_id);
#endif

    int ret;

    // connect to the sockets
    ret = zmq_connect(tasks, "inproc://tasks");
    assert(ret == 0);
    ret = zmq_connect(responses, "inproc://responses");
    assert(ret == 0);

#ifdef DEBUG
    printf("[worker #%i] connected\n", worker_id);
#endif

    bool shutdown = false;

    while(!shutdown) {
        // receive a message, we block here
        // because if there is no message, then
        // there is nothing to do
        char buffer[BUFSIZE];
        int len = zmq_recv(tasks, buffer, 256, 0);

#ifdef DEBUG
        printf("[worker #%i] got message\n", worker_id);
#endif

        // check if there was an error
        if(len == -1) {
            if(errno == EAGAIN) {
                // if we got EAGAIN, simply try
                // again.
#ifdef DEBUG
                printf("[worker #%i] got EAGAIN\n", worker_id);
#endif
                continue;
            } else {
                // if anything else happened,
                // there is not much we can do
                // right now, so we just exit.
                fprintf(stderr, "[worker #%i] error happened (errno: %i)\n", worker_id, errno);
                goto bailout;
            }
        }

        // process the message
        worker_task task = worker_handle_mesg(buffer, len);

        if(task.type == MESG_SHUTDOWN) {
#ifdef DEBUG
            printf("[worker #%i] got shutdown message\n", worker_id);
#endif
            shutdown = true;
            continue;
        }

        if(task.type == MESG_ERROR) {
            fprintf(stderr, "[worker #%i] got erroneous message!\n", worker_id);
        }

        // at this point, the message can only
        // be a MESG_TASK, so we handle the
        // supplied sudoku.
        sudoku_status status = worker_handle_sudoku(&task.sudoku);
        
        // check what happened with the sudoku and
        // create message to return to manager
        if(status.type == SUDOKU_SOLVED) {
            worker_response_solved(buffer, &len, status.solution, task.id);
        } else {
            worker_response_diverge(buffer, &len, status.diverges, task.id);

            // since the GList is allocated in 
            // worker_handle_sudoku(), we have to
            // free it here because we're done using
            // it.
            g_list_free(status.diverges);
        }

        if(len == -1) {
            fprintf(stderr, "[worker #%i] got error in response\n", worker_id);
        }

        // pass the result back to the manager
        ret = zmq_send(responses, buffer, len, ZMQ_DONTWAIT);

        if(ret == -1) {
            fprintf(stderr, "[worker #%i] error while sending response\n", worker_id);
        }
    }

    // FIXME
    // before exiting, do we want to pipe all incoming tasks
    // to responses as diverging sudokus so they don't get
    // lost?

bailout:
    zmq_close(tasks);
    zmq_close(responses);
    pthread_exit(NULL);
    printf("[worker #%i] cleaned up\n", worker_id);

    return NULL;
}

worker_task worker_handle_mesg(char *mesg, int length) {
    mpack_tree_t msg;
    mpack_tree_init(&msg, mesg, length);
    mpack_node_t root = mpack_tree_root(&msg);

    const char *type = mpack_node_str(mpack_node_map_cstr(root, "type"));

    if(mpack_node_error(root) != mpack_ok) {
        goto error;
    }

    if(strncmp(type, "shutdown", 8) == 0) {
        return (worker_task){.type = MESG_SHUTDOWN};
    }

    if(strncmp(type, "sudoku", 6) != 0) {
        goto error;
    }

    int id = mpack_node_i32(mpack_node_map_cstr(root, "id"));
    mpack_node_t sudoku = mpack_node_map_cstr(root, "sudoku");
    const char *sudoku_packed = mpack_node_str(sudoku);

    // FIXME hardcoded size of packed sudoku
    if(mpack_node_strlen(sudoku) < 92 || mpack_node_error(root) != mpack_ok) {
        goto error;
    }

    return (worker_task){
        .type = MESG_TASK,
        .sudoku = sudoku_puzzle_unpack(sudoku_packed),
        .id = id
    };
error:
    return (worker_task){.type = MESG_ERROR};
}

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
