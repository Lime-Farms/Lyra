#include <lyra/error.h>
#include <lyra/ring.h>
#include <stdlib.h>

uint16_t ring_new(struct ring *this, uint16_t len) {
  if(len < 3) {
    return RING_TOO_SMALL;
  }

  this->buflen = len + 1;
  this->buffer = malloc(new->buflen);

  if(this->buffer == NULL) {
    return KERNEL_MALLOC_FAIL;
  }

  this->begin = this->buffer;
  this->end = this->buffer + len;
  this->reader = this->buffer;
  this->writer = this->buffer;
  return LYRA_SUCCESS;
}

uint16_t ring_del(struct ring *this) {
  buf->buflen = 0;
  buf->begin = NULL;
  buf->end = NULL;
  buf->reader = NULL;
  buf->writer = NULL;
  free(buf->buffer);
  return LYRA_SUCCESS;
}

uint16_t ring_write(struct ring *this, char *str, uint16_t len) {
  while(len > 0) {
    if(this->writer == this->end && this->reader == this->begin) {
      break;
    } else if(this->writer == this->end + 1) {
      this->writer = this->begin;
    } 

    if(this->writer == this->reader - 1) {
      break;
    }

    *this->writer = *str;
    len -= 1;
    str += 1;
    this->writer += 1;
  }

  return len;
}

char *ring_read(struct ring *this, uint16_t *len) {
  char *str = malloc(*len);
  uint16_t idx = 0;

  if(str == NULL) {
    *len = KERNEL_MALLOC_FAIL;
    return NULL;
  }

  while(idx < *len && buf->reader != buf->writer) {
    str[idx] = *buf->reader;
    idx += 1;
    buf->reader += 1;

    if(buf->reader == buf->end + 1) {
      buf->reader = buf->begin;
    }
  }

  if(idx == 0) {
    *len = RING_EMPTY;
    return NULL;
  } else {
    *len -= idx;
    return str;
  }
}

char *ring_readln(struct ring *buf, uint16_t *len) {
  *len = 0;
  char *idx = buf->reader;

  while(*idx != '\n' && idx != buf->writer) {
    idx += 1;
    *len += 1;

    if(idx == buf->end + 1) {
      idx = buf->begin;
    }
  }

  if(*idx != '\n') {
    *len = 0;
    return NULL;
  }

  uint16_t line_len = *len + 1;
  char *str = ring_read(buf, &line_len);

  if(str != NULL) {
    str[*len] = '\0';
  }

  return str;
}
