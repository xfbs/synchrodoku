// vim: noet:ts=4:sw=4
#include <czmq.h>
#include <zsock.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include <glib.h>
#include <mpack/mpack.h>
#include <mpack/mpack-writer.h>
#include "manager.h"
#include "log.h"
#include "worker.h"

typedef struct {
    int id;
    unsigned char sudoku[92];
} manager_workset;

void *worker_task(void *);
void *context;
pthread_t manager_thread;

void manager_start(manager_options_t options)
{
    log("starting");

    // create a new zmq context for use in this program
    // this is shared among all threads
    context = zmq_ctx_new();

    // we can set the number of IO threads to 0 because
    // we are only using inter-thread communication
    zmq_ctx_set (context, ZMQ_IO_THREADS, 0);

    pthread_create(&manager_thread, NULL, manager_loop, &options);
}

void manager_stop(void)
{
	log("stopping");
    /*
    void *requests = zmq_socket(context, ZMQ_REQ);

    int ret = zmq_connect(requests, "inproc://control");
    assert(ret == 0);

    // send SHUTDOWN message?
    char *data;
    size_t size;
    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &data, &size);

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
    */

    pthread_join(manager_thread, NULL);
}

void *manager_loop(void *data)
{
    log("manager loop started");

	// grab options.
	manager_options_t *options = data;
	assert(options);
	assert(options->thread_count);

    // create sockets
	zsock_t *tasks = zsock_new_push("inproc://tasks");
	zsock_t *solutions = zsock_new_pub("inproc://solutions");
	zsock_t *requests = zsock_new_push("@inproc://requests");
	zsock_t *responses = zsock_new_pull(">inproc://responses");

	// make sure creation worked
	assert(requests);
	assert(solutions);
	assert(tasks);
	assert(responses);

    // create all workers
    //pthread_t workers[options->thread_count];
	worker_t workers[options->thread_count];
    for(int i = 0; i < options->thread_count; i++) {
		workers[i].id = i;
		workers[i].requests = zsock_new_pull(">inproc://requests");
		//workers[i].responses = zsock_new_push("@inproc://responses");
		assert(workers[i].requests);
		//assert(workers[i].responses);
		log("starting worker #%i", i);
        assert(pthread_create(&workers[i].thread, NULL, worker_loop, &workers[i]) == 0);
    }

    log("started all workers");
    //usleep(500000);
    /*

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
    */

	zsock_destroy(&requests);
	zsock_destroy(&responses);
	zsock_destroy(&tasks);
	zsock_destroy(&solutions);

	log("loop stopped");

    return NULL;
}

/*
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
*/
