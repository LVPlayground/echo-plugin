// Copyright 2007 Las Venturas Playground. All rights reserved.
// Use of this source code is governed by the GPLv2 license, a copy of which can
// be found in the LICENSE file.

native EchoMessage(message[]);
native SetEchoDestination(hostname[], port);

main() {
  SetEchoDestination("127.0.0.1", 8001);
  EchoMessage("Hello, world!");
}
