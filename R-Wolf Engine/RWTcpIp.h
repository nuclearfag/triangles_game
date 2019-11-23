#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define memreset(a) memset(a, 0, sizeof(*a))
#define memdrop(a) a(0)

namespace RWInet
{

}