#include <zmq.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include <glib.h>

typedef struct {
    int id;
    unsigned char sudoku[92];
} manager_workset;

void *worker_task(void *);
void *context;
pthread manager_thread;

int
main(int argc, char *argv[])
{
    return 0;
}

void manager_start(int thread_count)
{
    // create a new zmq context for use in this program
    // this is shared among all threads
    context = zmq_ctx_new();

    // we can set the number of IO threads to 0 because
    // we are only using inter-thread communication
    zmq_ctx_set (context, ZMQ_IO_THREADS, 0);

    pthread_create(&manager_thread, NULL, manager_loop, &thread_count);
}

void manager_stop(void)
{
    void *requests = zmq_socket(context, ZMQ_REQ);

    int ret = zmq_connect(requests, "inproc://control");
    assert(ret == 0);

    // send SHUTDOWN message?
    char *data;
    size_t size;
    mpack_writer_t writer;
    mapck_writer_init_growable(&writer, &data, &size);

    mpack_start_map(&writer, 1);
    mpack_write_cstr(&writer, "type");
    mpack_write_cstr(&writer, "shutdown");
    mpack_finish_map(&writer);

    if(mpack_writer_destroy(&writer) != mpack_ok) {
        assert(false);
    }

    zmq_send(context, data, size, 0);

    // zmq_revc() ?

    free(data);
    zmq_ctx_destroy(context);
}

void manager_loop(void *data)
{
    // create new socket
    void *requests = zmq_socket(context, ZMQ_REP);
    void *solutions = zmq_socket(context, ZMQ_PUB);
    void *tasks = zmq_socket(context, ZMQ_PUSH);
    void *responses = zmq_socket(context, ZMQ_PULL);
    
    printf("[manager] started\n");

    int ret;
    ret = zmq_bind (tasks, "inproc://tasks");
    assert(ret == 0);
    ret = zmq_bind(responses, "inproc://responses");
    assert(ret == 0);
    ret = zmq_bind(requests, "inproc://requests");
    assert(ret == 0);
    ret = zmq_bind(solutions, "inproc://responses");
    assert(ret == 0);

    printf("[manager] bound to sockets\n");

    // create all workers
    pthread_t workers[10];
    for(int i = 0; i < 10; i++) {
        pthread_create(&workers[i], NULL, worker_task, (void*) i);
    }

    printf("[manager] started workers\n");
    usleep(500000);

    // work queues
    GList *partial_solutions = g_list_alloc();
    GList *final_solutions = g_list_alloc();
    assert(partial_solutions != NULL);
    assert(final_solutions != NULL);

    bool shutdown = false;
    bool stop = false;

    while(!stop) {
        // send all workers some message
        for(int i = 0; i < 255; i++) {
            char mesg[] = {23, 56};
            zmq_send(tasks, mesg, 2, ZMQ_DONTWAIT);
        }
        
        printf("[manager] sent some messages\n");
        fflush(stdout);

        // wait for all responses
        for(int i = 0; i < 255; i++) {
            char data[255];
            int size = zmq_recv(responses, data, 255, 0);
            assert(size > 0);
        }
    }

        printf("[manager] got all replies\n");

    // send all workers kill message
    for(int i = 0; i < 10; i++) {
        char mesg[] = {0, 0};
        zmq_send(tasks, mesg, 2, 0);
    }

    printf("[manager] sent kill messages\n");

    // wait for all workers to quit
    for(int i = 0; i < 10; i++) {
        pthread_join(workers[i], NULL);
    }

    printf("[manager] all workers dead\n");

    zmq_close(tasks);
    zmq_close(responses);
    zmq_close(requests);
    zmq_close(solutions);

    return 0;
}

void *
worker_task(void *opts)
{
    int number = (int) opts;
    void *tasks = zmq_socket(context, ZMQ_PULL);
    void *responses = zmq_socket(context, ZMQ_PUSH);
    int ret;

    printf("[worker #%i] started up\n", number);

    ret = zmq_connect(tasks, "inproc://tasks");
    assert(ret == 0);
    ret = zmq_connect(responses, "inproc://responses");
    assert(ret == 0);

    printf("[worker #%i] connected\n", number);

    while(true) {
        // receive a message
        char *buffer[256];
        int len = zmq_recv(tasks, buffer, 256, 0);

        printf("[worker #%i] got message\n", number);

        // check if there was an error
        if(len == -1) {
            if(errno == EAGAIN) {
                printf("[worker #%i] got EAGAIN\n", number);
                continue;
            } else {
                printf("[worker #%i] error happened\n", number);
                goto bailout;
            }
        }

        // check if this is a goodbye note
        if(buffer[0] == 0) {
            printf("[worker #%i] got good bye note\n", number);
            goto bailout;
        }

        fflush(stdout);
        if(number == 4) {
            usleep(200000);
        } else {
            usleep(50000);
        }

        // otherwise, pass the data right back
        zmq_send(responses, buffer, len, ZMQ_DONTWAIT);
    }

bailout:
    zmq_close(tasks);
    zmq_close(responses);
    pthread_exit(NULL);
    printf("[worker #%i] cleaned up\n", number);

    return NULL;
}


