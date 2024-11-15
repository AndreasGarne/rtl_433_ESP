#include "decoder.h"

// Define the types of devices this file supports (uses expected bitlen)
#define DATA_SIZE 13

static int nexa_doorbell_decode(r_device *decoder, bitbuffer_t *bitbuffer)
{
    data_t *data;
    int r;
    int device;
    uint8_t *b;

//     r = bitbuffer_find_repeated_row(bitbuffer, bitbuffer->num_rows > 5 ? 5 : 3, 12); // 32

//     decoder_logf(decoder, -2, __func__, "rows: %i", r);

    // decoder_log(decoder, -2, __func__, "invert buffer");

    // bitbuffer_invert(bitbuffer);

    // if (r < 0) {
    //     return DECODE_ABORT_LENGTH;
    // }

    // decoder_logf(decoder, -2, __func__, "bits per row: %i", bitbuffer->bits_per_row[r]);

    // if (bitbuffer->bits_per_row[r] != DATA_SIZE) {
    //     return DECODE_ABORT_EARLY;
    // }

    if (bitbuffer->bits_per_row[0] < 50)
    {
        return DECODE_ABORT_LENGTH;
    }
    int i;
    for (i = 0; i < bitbuffer->num_rows; i++)
    {
        decoder_logf(decoder, -2, __func__,"row: %i, data length: %i, hex: %x, leading sync bits: %i", i, bitbuffer->bits_per_row[i], bitbuffer->bb[i], bitbuffer->syncs_before_row[i]);
    }

    decoder_logf(decoder, -2, __func__,"num_rows: %i, i: %i", bitbuffer->num_rows, i);
    // b = bitbuffer->bb[r];


    return DECODE_ABORT_EARLY;
    /* clang-format off */
    data = data_make(
            "id", "", DATA_STRING, "NEXA_DOORBELL",
            "button",            "",                 DATA_STRING, "OFF",
            // "data",          "",                 DATA_STRING, b,
            // "dataLength",    "",                 DATA_INT,    strlen(b),
            // "data",               "Data",        DATA_STRING, sprintf("%i %i %i %i %i %i %i %i %i %i %i %i %i", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], b[8], b[9], b[10], b[11], b[12]),
            "num_rows", "", DATA_INT, bitbuffer->num_rows,
            "repeated_rows", "", DATA_INT, r,
            // "free_row", "", DATA_INT, bitbuffer->free_row,
            // "bits_per_row", "", DATA_INT, bitbuffer->bits_per_row[r],
            // "syncs_before_row", "", DATA_INT, bitbuffer->syncs_before_row[r],
            NULL);
    /* clang-format on */
    decoder_output_data(decoder, data);
    return 1;
}
/*
typedef struct bitbuffer {
    uint16_t num_rows;                      ///< Number of active rows
    uint16_t free_row;                      ///< Index of next free row
    uint16_t bits_per_row[BITBUF_ROWS];     ///< Number of active bits per row
    uint16_t syncs_before_row[BITBUF_ROWS]; ///< Number of sync pulses before row
    bitarray_t bb;                          ///< The actual bits buffer
} bitbuffer_t;
*/

static char const *output_fields[] = {
        "id",
        "button",
        // "data",
        // "dataLength",
        "num_rows",
        "repeated_rows",
        // "free_row",
        // "bits_per_row",
        // "syncs_before_row",
        NULL,
};

// note TX141W, TX145wsdth: m=OOK_PWM, s=256, l=500, r=1888, y=748
r_device nexa_doorbell = {
        .name        = "NEXA door bell",
        .modulation  = OOK_PULSE_PPM,
        .short_width = 254,  // 1:1
        .long_width  = 1270, // 1:5
        .sync_width  = 2540, // 1:10
        .tolerance   = 100,
        .gap_limit   = 1500,
        .reset_limit = 10688,
        // .modulation  = OOK_PULSE_PWM,
        // .short_width = 256,  // short pulse is 208 us + 417 us gap
        // .long_width  = 320,  // long pulse is 417 us + 208 us gap
        // .sync_width  = 0,  // sync pulse is 833 us + 833 us gap
//        .tolerance   = 200,
        // .gap_limit   = 260,  // long gap (with short pulse) is ~417 us, sync gap is ~833 us
        // .reset_limit = 10688, // maximum gap is 1250 us (long gap + longer sync gap on last repeat)
        .decode_fn   = &nexa_doorbell_decode,
        .fields      = output_fields,
};
