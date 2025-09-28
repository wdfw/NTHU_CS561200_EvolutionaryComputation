#pragma once
#include "config.h"
#include <map>
#include <string>
#include <vector>

void usage(const char* progname);
int parse_cmd(int argc, char * const argv[], Configuration& config);