#pragma once
#include <iostream>
#include <cstdlib>
#include <cassert>

#define JASSERT(expr,msg) (void)((expr)?((void)0):assert_msg(msg))

void assert_msg(const char *msg = nullptr);



