#include <string>
#include "vars.h"

std::string status;

extern "C" const char *get_var_status()
{
    return status.c_str();
}

extern "C" void set_var_status(const char *value)
{
    status = value;
}
