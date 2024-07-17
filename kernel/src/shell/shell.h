#pragma once
#include <kata_renderer/kata_renderer.h>
#include <liba/stdio.h>

#define INVALID_MODE -1
#define COMMAND_MODE 0
#define CALCULATOR_MODE 1

static int state = COMMAND_MODE;

void init_ata_shell();