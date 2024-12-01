#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include "interface.h"

int main(int argc, char *argv[])
{
    interface start;
    start.get_args_of_comline(argc, argv);
    return 0;
}
