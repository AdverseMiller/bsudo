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

using namespace std;

int duration = 300;

void execCommand(int argc, char* argv[]) {
    seteuid(0);
    setuid(0);
    string argv_inputs;
    int i;

    for(i = 1; i < argc; i++) { argv_inputs.append(argv[i]).append(" ");}
    
    system(argv_inputs.c_str());
}

bool FileExists(const string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

void HidePassword()
{
    termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}


int CheckPassword(const char* password )
{
    struct passwd *p = getpwuid(getuid());
    if ( 0 != strcmp( p->pw_passwd, "x" ) )
    {
        
        return strcmp( p->pw_passwd, crypt(password, p->pw_passwd ) );
    }
    else
    {
        struct spwd* shadowEntry = getspnam(p->pw_name);
        if ( !shadowEntry )
        {
            printf("No shadow entry found for '%s'\n", p->pw_name );
            return 1;
        }

        return strcmp( shadowEntry->sp_pwdp, crypt( password, shadowEntry->sp_pwdp ) );
    }
}

int main(int argc, char* argv[]) {
    if(geteuid() != 0) {cout << "bSudo requires the 's' flag to be set to use privilege elevation\n"; exit(1);}

    
     int64_t time;

    //create file if it does not exist
    if(!FileExists("/etc/bsudo")) {
        fstream create;
        string write_file_name("/etc/bsudo");
        create.open(write_file_name, ios::out);
        if(!create) cout << "Could not create file /etc/bsudo";
    } else {
        //get file data
        ifstream read;
        string read_file_name("/etc/bsudo");
        read.open(read_file_name);
        if(!read) cout << "Could not read file /etc/bsudo";
        string line;
            read >> line;
            
            if(line.length() == 0) time = 0;
            else time = stoi(line.data());
    }
    
    
    
    
	if(std::time(0) < time) execCommand(argc, argv);
    else {
    
    cout << "Password for " << getpwuid(getuid())->pw_name << ": ";
    string pass;
    HidePassword();
    cin >> pass;
    cout << "\n";
    
    if(CheckPassword(pass.c_str())==0) {
        fstream write;
        string write_file_name("/etc/bsudo");
        write.open(write_file_name, ios::out);
        if(!write) cout << "Could not write file /etc/bsudo";
        write << std::time(0) + duration;

    execCommand(argc, argv);
    }
    }
    
}