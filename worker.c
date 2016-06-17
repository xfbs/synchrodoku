#include "worker.h"

/*
typedef struct {
    int id;
    handle_request_func handle_request;
    void *zmq_ctx;
    const char *reqsock;
    const char *repsock;
} worker_options_t;

int ret;

void *worker_loop(worker_t *options) {
    // socket to recieve requests and send responses
    // back
    void *requests = zmq_socket(options->zmq_ctx, ZMQ_PULL);
    void *responses = zmq_socket(options->zmq_ctx, ZMQ_PUSH);

    // connect to sockets
    ret = zmq_connect(requests, options->reqsock);
    assert(ret == 0);
    ret = zmq_connect(responses, options->repsock);
    assert(ret == 0);

    bool shutdown = false;
    const int initial_bufsize = 1024;

    while(!shutdown) {
        // TODO: preseve byte array in between loops for
        // mor efficiency
        GByteArray *in = g_byte_array_new();

        in = g_byte_array_set_size(in, initial_bufsize);

        // recieve a message, blocking
        ret = zmq_recv(requests, in->data, in->len, 0);

        // if there was an error, try again
        if(ret < 0) {
            g_byte_array_unref(in);
            continue;
        }

        in = g_byte_array_set_size(in, ret);

        // if the message was too big, recieve the
        // rest now
        if(ret > in->len) {
            in = g_byte_array_set_size(in, ret);
            zmq_recv(requests, in->data+initial_bufsize, in->len-initial_bufsize, 0);
        }

        GBytes *in_bytes = g_byte_array_free_to_bytes(in);
        request_t request = request_parse(in_bytes);

        if(request.type == REQUEST_SHUTDOWN) {
            shutdown = true;
            continue;
        }

        response_t response = options->handle_request(&request);
        GBytes *out = response_create(&response);

        size_t len;
        const char *data = g_bytes_get_data(out, &len);

        ret = zmq_send(responses, data, len, ZMQ_DONTWAIT);
        assert(ret >= 0);

        g_bytes_unref(out);
        request_unref(&request);
        response_unref(&response);
    }

    // flush incoming work?

    zmq_close(requests);
    zmq_close(responses);

    return NULL;
}

void worker_launch(worker_t *worker) {
    pthread_create(&worker->thread, NULL, worker_loop, worker);
}

void worker_join(worker_t *worker) {
    pthread_join(&worker->thead, NULL);
}

worker_pool_t worker_pool_new(const char *reqs, const char *reps, int size) {
    worker_pool_t pool;

    pool.workers = NULL;
    pool.handle_request = NULL;
    pool.zmq_ctx = zmq_ctx_new();

    return pool;
}


void *
worker_loop(void *opts)
{
    // the ID of the worker is passed as
    // only argument.
    int worker_id = (int) opts;

    // this is the socket over which the 
    // worker gets tasks sent to by the
    // manager
    void *tasks = zmq_socket(_worker_zmq_context, ZMQ_PULL);

    // this is the socket it uses to
    // reply back to the manager
    void *responses = zmq_socket(_worker_zmq_context, ZMQ_PUSH);

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

*/
