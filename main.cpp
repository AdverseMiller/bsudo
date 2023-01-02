#include <sys/types.h>
#include <pwd.h>
#include <shadow.h>
#include <crypt.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <stdint.h>
#include "functions.hpp"

using namespace std;

int duration = 300;
string authFile = "/etc/bsudo";


int main(int argc, char* argv[]) {
    if(geteuid() != 0) {
        cout << "bSudo requires the 's' flag to be set to use privilege elevation\n"; 
        return 1;
    }

    if(argc == 1) {
        cout << "usage: " << argv[0] << " command [args]\n"; 
        return 1;
    }

    

    if(!FileExists(authFile)) createAuthFile(authFile);
    
    
	if(std::time(0) < getTime(authFile)) execCommand(argc, argv);
    else {
        cout << "Password for " << getpwuid(getuid())->pw_name << ": ";
        
        if(CheckPassword(getPassword().c_str())==0) {
            writeAuthFile(authFile, duration);
            execCommand(argc, argv);
        }
    }
    
}