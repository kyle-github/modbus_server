/*
 * This file under the MPL 2.0 license.  See the LICENSE file in the root directory of this project.
 */

#include <errno.h>
#include <modbus/modbus.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_STATUS_COILS    (10)
#define NUM_STATUS_INPUTS   (10)
#define NUM_HOLDING_REGS    (10)
#define NUM_INPUT_REGS      (10)

int main(int argc, const char **argv)
{
    for(int i=0; i < argc; i++) {
        printf("Arg %d: %s\n", i, argv[i]);
    }

    /* 
     * straight from Stack Overflow 
     * https://stackoverflow.com/questions/29602698/create-a-simple-client-server-using-modbus-in-c
     */

    modbus_mapping_t *mapping = modbus_mapping_new(NUM_STATUS_COILS, NUM_STATUS_INPUTS, NUM_HOLDING_REGS, NUM_INPUT_REGS);
    if (!mapping) {
        fprintf(stderr, "Failed to allocate the mapping: %s\n", modbus_strerror(errno));
        exit(1);
    }

    //Example: set register 12 to integer value 623
    mapping->tab_registers[12] = 623;

    modbus_t *ctx = modbus_new_tcp("127.0.0.1", 1502);
    if (!ctx) {
        fprintf(stderr, "Failed to create the context: %s\n", modbus_strerror(errno));
        exit(1);
    }

    //Set the Modbus address of this server (to 3)
    //modbus_set_slave(ctx, 3);

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Unable to connect: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        exit(1);
    }


    uint8_t req[MODBUS_RTU_MAX_ADU_LENGTH];// request buffer
    int len;// length of the request/response

    while(1) {
        len = modbus_receive(ctx, req);
        if (len == -1) break;

        len = modbus_reply(ctx, req, len, mapping);
        if (len == -1) break;
    }

    printf("Exit the loop: %s\n", modbus_strerror(errno));

    modbus_mapping_free(mapping);
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}