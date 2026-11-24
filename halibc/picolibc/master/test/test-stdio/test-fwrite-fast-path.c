/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright © 2026 Keith Packard
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define _GNU_SOURCE
#include <stdio.h>

#define BUFFER 128
#define CHUNK  16

static int    error;

static size_t write_ptr;

static ssize_t
cookie_write(void *cookie, const char *buf, size_t n)
{
    const unsigned char *b = (unsigned char *)buf;
    size_t               i;

    if (n > BUFFER) {
        printf("cookie buf overflow, writing %zd at %zd\n", n, write_ptr);
        error = 1;
    }
    (void)cookie;
    for (i = 0; i < n; i++) {
        if (b[i] != (unsigned char)write_ptr) {
            printf("cookie_write failed at offset %zd (got %02x want %02x)\n", write_ptr, b[i],
                   (unsigned char)write_ptr);
            error = 1;
        }
        write_ptr++;
    }
    return (ssize_t)n;
}

static int
cookie_close(void *cookie)
{
    (void)cookie;
    return 0;
}

static cookie_io_functions_t cookie_funcs = {
    .read = 0,
    .write = cookie_write,
    .seek = 0,
    .close = cookie_close,
};

static unsigned char buf[BUFFER];
static size_t        buf_ptr;
static char          file_buf[BUFFER + 1];

int
main(void)
{
    FILE  *f = fopencookie(&buf_ptr, "w", cookie_funcs);
    size_t s, t;

    if (!f) {
        perror("fopencookie");
        return 1;
    }
    setbuffer(f, file_buf, BUFFER);
    for (s = 0; s < BUFFER; s++) {
        buf[s] = (unsigned char)s;
    }
    for (t = 0; t < BUFFER; t += CHUNK) {
        if (fwrite(buf + t, sizeof(unsigned char), CHUNK, f) != CHUNK) {
            perror("fwrite");
            return 1;
        }
    }
    putc((char)s, f);
    fclose(f);
    if ((unsigned char)write_ptr != BUFFER + 1) {
        printf("wrote %zd but write func only saw %zd\n", (size_t)BUFFER + 1, write_ptr);
        error = 1;
    }
    return error;
}
