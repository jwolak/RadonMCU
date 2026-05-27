/*-
 * BSD 3-Clause License
 *
 * No Copyrights 2026, Janusz Wolak
 * All rights not reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifndef __RINGBUFFER_H_
#define __RINGBUFFER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define RING_BUFFER_SIZE 128u

typedef struct
{
  uint8_t buffer[RING_BUFFER_SIZE];
  size_t head;
  size_t tail;
  size_t count;
} ring_buffer_t;

struct RingBuffer
{
  /* public members */
  void (*init)(struct RingBuffer *this);
  bool (*is_empty)(struct RingBuffer *this);
  bool (*is_full)(struct RingBuffer *this);
  size_t (*size)(struct RingBuffer *this);
  bool (*push)(struct RingBuffer *this, uint8_t data);
  bool (*pop)(struct RingBuffer *this, uint8_t *data);

  /* private members */
  ring_buffer_t buffer_;
};

extern const struct RingBufferClass
{
  /* Returns a pointer to a single global ring buffer instance. */
  struct RingBuffer *(*instance)(void);
  struct RingBuffer (*new)();
} RingBuffer;

#endif /* __RINGBUFFER_H_ */