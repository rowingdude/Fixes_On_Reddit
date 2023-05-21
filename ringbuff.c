/**
 * reference LICENSE file provided.
 */

#include "ringbuffer.h"

#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <semaphore.h>

#ifdef DRINGBUFFER_DEBUG
#include <stdio.h>
#endif

void ringbuffer_ctor(ringbuffer* arg_p, size_t arg_len)
{
  arg_p->m_len = arg_len;
  arg_p->m_data = (uint8_t*)malloc(arg_p->m_len);
  memset(arg_p->m_data, 0, arg_p->m_len);
  arg_p->m_w = arg_p->m_data;
  arg_p->m_r = arg_p->m_data;

  sem_init(&arg_p->m_writable, 0, (uint32_t)arg_p->m_len);
  sem_init(&arg_p->m_readable, 0, 0);
}

void ringbuffer_dtor(ringbuffer* arg_p)
{
  memset(arg_p->m_data, 0, arg_p->m_len);
  free(arg_p->m_data);
  arg_p->m_len = 0;
  arg_p->m_data = NULL;
  arg_p->m_w = NULL;
  arg_p->m_r = NULL;

  sem_destroy(&arg_p->m_readable);
  sem_destroy(&arg_p->m_writable);
}

int32_t ringbuffer_write(ringbuffer* arg_p, uint8_t arg_c, const struct timespec* arg_to)
{
#ifdef DRINGBUFFER_DEBUG
  // get semaphore status.
  int sem_current_value;
  sem_getvalue(&arg_p->m_writable, &sem_current_value);
  printf("writable counter is: %d\n", sem_current_value);
#endif

  // take a writable semaphore.
  int32_t e_wait = sem_timedwait(&arg_p->m_writable, arg_to);
  if (e_wait == -1 && errno == ETIMEDOUT)
  {
    return -1;
  }
  else if (e_wait == -1)
  {
    return -2;
  }

  // write.
  *(arg_p->m_w) = arg_c;

  // increment write index.
  arg_p->m_w++;
  if (arg_p->m_w == (arg_p->m_data + arg_p->m_len))
    arg_p->m_w = arg_p->m_data;

  // release a readable semaphore.
  int32_t e_post = sem_post(&arg_p->m_readable);
  if (e_post == 0)
  {
    return 0;
  }
  else
  {
    return -2;
  }
}

int32_t ringbuffer_read(ringbuffer* arg_p, uint8_t* arg_c, const struct timespec* arg_to)
{
#ifdef DRINGBUFFER_DEBUG
  // get semaphore status.
  int sem_current_value;
  sem_getvalue(&arg_p->m_readable, &sem_current_value);
  printf("readable counter is: %d\n", sem_current_value);
#endif

  // take a readable semaphore.
  int32_t e_wait = sem_timedwait(&arg_p->m_readable, arg_to);
  if (e_wait == -1 && errno == ETIMEDOUT)
  {
    return -1;
  }
  else if (e_wait == -1)
  {
    return -2;
  }

  // read.
  *arg_c = *(arg_p->m_r);

  // increment read index.
  arg_p->m_r++;
  if (arg_p->m_r == (arg_p->m_data + arg_p->m_len))
    arg_p->m_r = arg_p->m_data;

  // release a writable semaphore.
  int32_t e_post = sem_post(&arg_p->m_writable);
  if (e_post == 0)
  {
    return 0;
  }
  else
  {
    return -2;
  }
}
