#pragma once

#include "acpi.h"

namespace ACPI
{
    extern uint16_t SLP_TYPa;
    extern uint16_t SLP_TYPb;

    class DSDT
    {
    public:
        void Get_S5_Object();

    private:
        SDTHeader header;
    };
};
