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

#include "RingBuffer.h"

static void init(struct RingBuffer *this)
{
  this->buffer_.head = 0;
  this->buffer_.tail = 0;
  this->buffer_.count = 0;
}

static bool is_empty(struct RingBuffer *this)
{
  return this->buffer_.count == 0;
}

static bool is_full(struct RingBuffer *this)
{
  return this->buffer_.count == RING_BUFFER_SIZE;
}

static size_t size(struct RingBuffer *this)
{
  return this->buffer_.count;
}

static bool push(struct RingBuffer *this, uint8_t data)
{
  if (this->is_full(this))
  {
    return false;
  }

  this->buffer_.buffer[this->buffer_.head] = data;
  this->buffer_.head = (this->buffer_.head + 1u) % RING_BUFFER_SIZE;
  this->buffer_.count++;
  return true;
}

static bool pop(struct RingBuffer *this, uint8_t *data)
{
  if (this->is_empty(this))
  {
    return false;
  }

  *data = this->buffer_.buffer[this->buffer_.tail];
  this->buffer_.tail = (this->buffer_.tail + 1u) % RING_BUFFER_SIZE;
  this->buffer_.count--;
  return true;
}

static struct RingBuffer newRingBuffer(void)
{
  struct RingBuffer rb = {
      .init = init,
      .is_empty = is_empty,
      .is_full = is_full,
      .size = size,
      .push = push,
      .pop = pop,
  };
  rb.init(&rb);
  return rb;
}

const struct RingBufferClass RingBuffer = {
    .new = newRingBuffer,
};
