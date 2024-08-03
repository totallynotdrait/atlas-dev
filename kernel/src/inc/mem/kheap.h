#pragma once

#include <stdint.h>
#include <stddef.h>
#include <log.h>

void* kmalloc(size_t size);
void kfree(void* ptr);