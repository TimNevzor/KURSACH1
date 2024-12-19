//main.cpp
#include <filesystem>
#include "interface.h"

int main(int argc, const char *argv[])
{
    interface start;
    start.get_args_of_comline(argc, argv);
    return 0;
}
