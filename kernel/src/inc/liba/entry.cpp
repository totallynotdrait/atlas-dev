#include <panic/panic.h>

extern "C" void __cxa_pure_virtual()
{
    Panic("VIRTUAL_FUNCTION_DEFINITION_NOT_FOUND", nullptr);
    while (true);
}

extern "C" void __stack_chk_fail()
{
    Panic("STACK_CHECK_FAILED", nullptr);
    while (true);
}
