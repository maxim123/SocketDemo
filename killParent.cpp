#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>
#include <stdlib.h>
using namespace std;

int main(int argc, char* argv[]) {
	//Yes, I know this is a terrible way to do this
	stringstream temp;
	for(int i = 1; i < argc; ++i) {
		temp << argv[i]<<" ";
	}
	system(temp.str().c_str());
	system("tmux kill-session -t socketSession");
	//kill(getppid(), 9);
}
