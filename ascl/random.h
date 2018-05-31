#ifndef RANDOM_H
#define RANDOM_H

#include "platform.h"

int random_int();

int random_bytes(uint8_t *buf,size_t len);

#endif
