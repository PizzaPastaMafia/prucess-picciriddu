#!/bin/sh

gcc src/client.c -o bin/client -lncurses;
gcc src/server.c -o bin/server;
