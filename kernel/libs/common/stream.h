#ifndef _LIB_STREAM_H
#define _LIB_STREAM_H

#include "cubebox.h"

typedef int (*pfnStreamWriteBuf) (char*);

struct Stream {
    pfnStreamWriteBuf pfn_write_all;

    size_t buf_len;
    size_t buf_i;

    char* buf;
};

#endif