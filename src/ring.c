#include <lyra/error.h>
#include <lyra/ring.h>
#include <stdlib.h>

uint16_t ring_new(struct ring *this, uint16_t len) {
  if(len < 3) {
    return RING_TOO_SMALL;
  }

  this->buflen = len + 1;
  this->buffer = malloc(this->buflen);

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
  this->buflen = 0;
  this->begin = NULL;
  this->end = NULL;
  this->reader = NULL;
  this->writer = NULL;
  free(this->buffer);
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

  while(idx < (*len - 1) && this->reader != this->writer) {
    str[idx] = *this->reader;
    idx += 1;
    this->reader += 1;

    if(this->reader == this->end + 1) {
      this->reader = this->begin;
    }
  }

  if(idx == 0) {
    *len = RING_EMPTY;
    return NULL;
  } else {
    str[idx] = '\0';
    *len -= idx;
    return str;
  }
}

char *ring_readln(struct ring *this, uint16_t *len) {
  *len = 0;
  char *idx = this->reader;

  while(*idx != '\n' && idx != this->writer) {
    idx += 1;
    *len += 1;

    if(idx == this->end + 1) {
      idx = this->begin;
    }
  }

  if(*idx != '\n') {
    *len = RING_NO_LINE;
    return NULL;
  }

  uint16_t line_len = *len + 1;
  char *str = ring_read(this, &line_len);
  return str;
}
