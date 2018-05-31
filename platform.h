#ifndef PLATFORM_H
#define PLATFORM_H

#include "malloc.h"
#include "string.h"
#include "stddef.h"
#include "stdint.h"

#include "stdio.h"

#define karma_malloc malloc
#define karma_free free
#define karma_memset memset
#define karma_memcpy memcpy

#endif
