# bsudo
A simple sudo alternative for linux written in C++. 
Allows commands to be executed without password for up to 5 minutes after using password. This can optionally be changed to any duration by changing 'int duration'

This program requires the 's' flag to be set to allow elevate privileges. 
Run c.sh as root to compile the program and give it the 's' flag automatically.