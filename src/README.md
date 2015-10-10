# echo-plugin
This plugin provides two natives to the Pawn runtime intended to broadcast messages over UDP to a specified destination.

## native EchoMessage(message[]);
Distributes `message` to the previously set destination. The maximum length of `message` is defined by the Pawn runtime.

## native SetEchoDestination(hostname[], port);
Sets `hostname`:`port` to be the destination of the UDP broadcast messages. `hostname` will be resolved before being set.
