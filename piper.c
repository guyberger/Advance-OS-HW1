// Include

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef int s_pipe[2];

void close_pipes(s_pipe *pipes, int start, int end) {
	for (int i = start ; i < end; i++) {
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}

void run_cmds(char **cmds, int num, s_pipe *pipes)
{
	// cmds holds program name on index 0
	for (int i = 0; i < num; i++){
		pid_t pid = fork();
		if (pid < 0) {
			perror("Fork failed\n");
			return;
		}
		else if (pid > 0) {
			// parent
			continue;
		}
		// child
		char *args[2] = {cmds[i+1], NULL};
		if (i == 0) {
			dup2(pipes[i][1], STDOUT_FILENO);
			close(pipes[i][0]);
			// Close all other pipes on this child
			close_pipes(pipes, 1, num-1);
		}
		else if (i == num - 1) {
			dup2(pipes[i-1][0], STDIN_FILENO);
			close(pipes[i-1][1]);
			// Close all other pipes on this child
			close_pipes(pipes, 0, i-1);
		}
		else{
			dup2(pipes[i-1][0], STDIN_FILENO);
			dup2(pipes[i][1], STDOUT_FILENO);
			close(pipes[i][0]);
			close(pipes[i-1][1]);
			// Close all other pipes on this child
			close_pipes(pipes, i+1, num-1);
		}
		execvp(cmds[i+1], args);
	}
}

int main(int argc, char *argv[])
{
	int argnum = argc - 1;
	s_pipe *pipes = NULL;
	int ret = 0;

	if (argc < 3) {
		perror("Usage: piper <cmd> <cmd> [<cmd> ...]\n");
		return -1;
	}

	pipes = malloc(sizeof(s_pipe) * (argnum - 1));

	for (int i = 0; i < argnum - 1; i++) {
		if (pipe(pipes[i]) < 0) {
			perror("Pipe error occured\n");
			return -1;
		}
	}

	run_cmds(argv, argnum, pipes);
	
	close_pipes(pipes, 0, argnum-1);

	for (int i = 0; i < argnum; i++) {
		wait(NULL);
	}

	return 0;
}


