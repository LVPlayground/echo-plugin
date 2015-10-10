// Copyright 2007 Las Venturas Playground. All rights reserved.
// Use of this source code is governed by the GPLv2 license, a copy of which can
// be found in the LICENSE file.

#include "sdk/amx.h"
#include "sdk/plugincommon.h"

#include <string.h>
#include <string>

#ifdef WIN32
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
typedef UINT_PTR socket_t;
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
typedef int socket_t;
#endif

typedef void (*logprintf_t)(char* format, ...);

logprintf_t logprintf;
void **ppPluginData;
extern void *pAMXFunctions;

int iEchoPort;
socket_t echoSocket;
struct hostent *h;

#define CHECK_PARAMS(n) { if (params[0] != n * sizeof(cell)) { logprintf("SCRIPT: Bad parameter count (%d != %d): ", params[0], n); return 0; } }

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()  {
  return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load( void **ppData )  {
  if (!echoSocket) {
    if ((echoSocket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
      logprintf("[LVP Echo] Error starting socket!");
      return 0;
    }
  }

#ifdef WIN32
  WSADATA data;
  int iResult = WSAStartup(MAKEWORD(1,1), &data);
#endif

  pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
  logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

  logprintf( "Plugin echo.dll got loaded." );
  return true;
}

//----------------------------------------------------------
// The Unload() function is called when the server shuts down,
// meaning this plugin gets shut down with it.

PLUGIN_EXPORT void PLUGIN_CALL Unload( ) {
#ifdef WIN32
  closesocket(echoSocket);
#else
  close(echoSocket);
#endif

  logprintf( "Plugin echo.dll got unloaded." );
}

//----------------------------------------------------------

// native EchoMessage(message[])
static cell AMX_NATIVE_CALL n_EchoMessage( AMX* amx, cell* params ) {
  CHECK_PARAMS(1);

  char* message = 0;
  sockaddr_in to;

  amx_StrParam(amx, params[1], message);
  std::string command = "[extern] ";
  command.append(message);

  to.sin_family = AF_INET;
  to.sin_port = htons(iEchoPort);
#ifdef WIN32
  to.sin_addr = *((struct in_addr *)h->h_addr);
#else
  memcpy(&to.sin_addr, h->h_addr, h->h_length);
#endif

  return sendto(echoSocket, command.c_str(), command.length(), 0, (sockaddr*)&to, sizeof(to));
}

// native SetEchoDestination(hostname[], port)
static cell AMX_NATIVE_CALL n_SetEchoDestination( AMX* amx, cell* params ) {
  CHECK_PARAMS(2);

  char* destination = 0;
  amx_StrParam(amx, params[1], destination);
  iEchoPort = int(params[2]);

  if ((h=gethostbyname(destination)) == NULL) {
    logprintf("[LVP Echo] Unable to resolve hostname (%s).", destination);
    return 0;
  }

  return 1;
}

AMX_NATIVE_INFO g_echoNatives[] = {
  { "EchoMessage",        n_EchoMessage },
  { "SetEchoDestination", n_SetEchoDestination },
  { 0, 0 }
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)  {
  return amx_Register(amx, g_echoNatives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx) {
  return AMX_ERR_NONE;
}
