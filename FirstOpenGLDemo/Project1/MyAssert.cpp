#include "MyAssert.h"

void assert_msg(const char *msg)
{
	if (msg)std::cerr << msg;
	std::abort();
}
