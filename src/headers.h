#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib")
#else
// #ifndef _WIN32 
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
/* In the Windows operating system, a socket() returns
   the SOCKET type, which is a wrapper over an
   unsigned integer, so this definition is
   necessary to ensure cross-platform compatibility */
#define SOCKET int
#endif

#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())
#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define GETSOCKETERRNO() (errno)
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
