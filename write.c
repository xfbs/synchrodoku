#include <mpack-writer.h>

int
main(int argc, char *argv[])
{
    // initialize the mpack_writer, which takes
    // care of allocating a buffer large enough
    // in data, and keeps track of it's size.
    // this is pretty sweet!
    char* data;
    size_t size;
    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &data, &size);

    // for fun, let's encode this JSON formatted
    // message using mpack, to show off all of
    // it's features:
    //
    //  {
    //      "type": "message",
    //      "size": 67,
    //      "data": [45, 29, 56, 10, 5],
    //      "attr": {
    //          "readonly": false,
    //          "public": true
    //      }
    //  }
    mpack_start_map(&writer, 4);

    mpack_write_cstr(&writer, "type");
    mpack_write_cstr(&writer, "message");

    mpack_write_cstr(&writer, "size");
    mpack_write_u8(&writer, 67);

    mpack_write_cstr(&writer, "data");
    mpack_start_array(&writer, 5);
    mpack_write_u8(&writer, 45);
    mpack_write_u8(&writer, 29);
    mpack_write_u8(&writer, 56);
    mpack_write_u8(&writer, 10);
    mpack_write_u8(&writer, 5);
    mpack_finish_array(&writer);

    mpack_write_cstr(&writer, "attr");
    mpack_start_map(&writer, 2);
    mpack_write_cstr(&writer, "readonly");
    mpack_write_bool(&writer, 0);
    mpack_write_cstr(&writer, "public");
    mpack_write_bool(&writer, 1);
    mpack_finish_map(&writer);

    mpack_finish_map(&writer);

    // finish writing
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        fprintf(stderr, "An error occurred encoding the data!\n");
        return -1;
    }

    // print the data
    for(int i = 0; i < size; i++) {
        printf("%c", data[i]);
    }

    printf("\n");
    free(data);

    return 0;
}
