#include <lyra/ring.h>
#include <stdlib.h>

struct ring *ring_new(uint16_t len) {
  if(len < 3) {
    return NULL;
  }

  struct ring *new = malloc(sizeof(struct ring));

  if(new == NULL) {
    return NULL;
  }

  new->buflen = len + 1;
  new->buffer = malloc(new->buflen);

  if(new->buffer == NULL) {
    free(new);
    return NULL;
  }

  new->begin = new->buffer;
  new->end = new->buffer + len;
  new->writer = new->buffer;
  new->reader = new->buffer;
  return new;
}

uint8_t ring_del(struct ring *buf) {
  buf->buflen = 0;
  buf->begin = NULL;
  buf->end = NULL;
  buf->writer = NULL;
  buf->reader = NULL;
  free(buf->buffer);
  free(buf);
  return 0;
}

uint16_t ring_write(struct ring *buf, char *str, uint16_t orig_len) {
  uint16_t len = orig_len;

  while(len > 0) {
    if(buf->writer == buf->end && buf->reader == buf->begin) {
      break;
    } else if(buf->writer == buf->end + 1) {
      buf->writer = buf->begin;
    } 

    if(buf->writer == buf->reader - 1) {
      break;
    }

    *buf->writer = *str;
    len -= 1;
    str += 1;
    buf->writer += 1;
  }

  return len;
}

char *ring_read(struct ring *buf, uint16_t *len) {
  char *str = malloc(*len);
  uint16_t idx = 0;

  if(str == NULL) {
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
    *len = 0;
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
