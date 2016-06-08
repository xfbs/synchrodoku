#include <mpack-node.h>
#include <stdio.h>
#include <stdbool.h>

int
main(int argc, char *argv[])
{
    // fill up buffer from stdin
    char input[256];
    size_t input_len = fread(input, 1, 256, stdin);

    // make sure it worked
    if(!feof(stdin)) {
        fprintf(stderr, "Error reading from stdin!\n");
    }

    // initialize the tree to use for parsing
    // the data
    mpack_tree_t msg;
    mpack_tree_init(&msg, input, input_len);
    mpack_node_t root = mpack_tree_root(&msg);

    // we are parsing some data that looks
    // somewhat like this:
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

    char *message = mpack_node_cstr_alloc(mpack_node_map_cstr(root, "type"), 256);
    int size = (int) mpack_node_i32(mpack_node_map_cstr(root, "size"));

    mpack_node_t data_node = mpack_node_map_cstr(root, "data");
    unsigned char *data = malloc(mpack_node_array_length(data_node) * sizeof(unsigned char));
    for(int i = 0; i < mpack_node_array_length(data_node); i++) {
        data[i] = mpack_node_u8(mpack_node_array_at(data_node, i));
    }

    mpack_node_t attr = mpack_node_map_cstr(root, "attr");
    bool readonly = mpack_node_bool(mpack_node_map_cstr(attr, "readonly"));
    bool public = mpack_node_bool(mpack_node_map_cstr(attr, "public"));


    if(mpack_node_error(root) != mpack_ok) {
        fprintf(stderr, "Something went wrong while parsing :/\n");
    }

    printf("type: %s\n", message);
    printf("size: %i\n", size);
    printf("data: [\n");
    for(int i = 0; i < mpack_node_array_length(data_node); i++) {
        printf("\t%u,\n", (unsigned int) data[i]);
    }
    printf("]\n");
    printf("attr {\n");
    printf("\treadonly: %s\n", readonly ? "true" : "false");
    printf("\tpublic: %s\n", public ? "true" : "false");
    printf("}\n");

    free(message);

    return 0;
}
