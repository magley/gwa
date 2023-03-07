#pragma once

#include "util/string/string2.h"

string2 from_file(const char* file);
void to_file(const char* file, const string2& str);