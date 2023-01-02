using namespace std;



void execCommand(int argc, char* argv[]) {
    setresuid(0, 0, 0);
    setresgid(0, 0, 0);
    
    
    int i;
    char *arguments[argc];
    
    for(i = 0; i < argc; i++) arguments[i] = argv[i+1];
    string executable = argv[0];
    arguments[argc] = NULL;
    
    if(execvp(arguments[0], arguments) == -1) cout << executable << ": " << arguments[0] << ": command not found\n";
    
}

bool FileExists(const string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}


string getPassword()
{
    struct termios tty, original;
    tcgetattr(STDIN_FILENO, &tty);
    original = tty;
    tty.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
    string pass;
    cin >> pass;
    cout << "\n";
    tcsetattr(STDIN_FILENO, TCSANOW, &original);
    return pass;
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

void createAuthFile(string authFile) {
    fstream create;
    create.open(authFile, ios::out);
    if(!create) cout << "Could not create file " << authFile << "\n";
    create.close();
}

int getTime(string authFile) {
    ifstream read;
    read.open(authFile);
    if(!read) cout << "Could not read file " << authFile << "\n";
    string line;
    read >> line;
    read.close();
    if(line.length() == 0) return 0;
    return stoi(line.data());
}

void writeAuthFile(string authFile, int duration) {
    fstream write;
    write.open(authFile, ios::out);
    if(!write) cout << "Could not write file " << authFile << "\n";
    write << std::time(0) + duration;
    write.close();
}