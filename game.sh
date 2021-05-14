#!/bin/bash
gcc -w -o server server.c
gcc -w -o client client.c
gnome-terminal -e ./server && gnome-terminal -e ./client && gnome-terminal -e ./client



