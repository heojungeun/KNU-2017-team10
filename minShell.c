#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

/*
cc minShell.c cd.c -o minshell
*/

#ifndef MINISHELL_H
#define MINISHELL_H

#define FALSE           0
#define TRUE            1
#define LINE_LEN        80
#define MAX_ARGS        64
#define MAX_ARG_LEN     64
#define MAX_PATHS       64
#define MAX_PATH_LEN    96
#define WHITESPACE      " .,\t&"
#define STD_INPUT       0
#define STD_OUTPUT      1
#ifndef NULL
#define NULL    0
#endif

static char commandInput = '\0';
static int buf_chars = 0;
static char *pathv[MAX_PATHS];
static char commandLine[LINE_LEN];

struct command_t {
	char *name;
	int argc;
	char *argv[MAX_ARGS];
};
struct command_t command;

void printPrompt();
void welcomeMessage();
int readCommand(char *commandLine, char *commandInput);
int parseCommand(char *commandLine, struct command_t *command);
char * lookupPath(char **, char **);
int parsePath(char **);

#endif // MINISHELL_H


// internal command clears terminal screen
void clearScreen() {
	printf("\033[2J\033[1;1H");
}

// not implemented yet
void cookies() {
	printf("i hungry and sleepy\n");
	printf("4 dollars!!!!\n");
	printf("This is A+ project\n");
}

// This function check is for built in commands
// and processes if there any
//
// @return boolean
int checkInternalCommand() {

	if (strcmp("cd", command.argv[0]) == 0) {
		cdnew(command.argc, command.argv);
		return 1;
	}
	if (strcmp("clear", command.argv[0]) == 0) {
		clearScreen();
		return 1;
	}
	if (strcmp("cookies", command.argv[0]) == 0) {
		cookies();
		return 1;
	}
	if (strcmp("cat", command.argv[0]) == 0) {
		catnew(command.argc, command.argv);
		return 1;
	}
	if (strcmp("cp", command.argv[0]) == 0) {
		cpnew(command.argc,command.argv);
		return 1;
	}
	if (strcmp("ls-alR", command.argv[0]) == 0) {
		ls_alRnew();
		return 1;
	}
	if (strcmp("mkdir", command.argv[0]) == 0) {
		mkdirnew(command.argc, command.argv);
		return 1;
	}
	if (strcmp("rmdir", command.argv[0]) == 0) {
		rmdirnew(command.argc, command.argv);
		return 1;
	}
	if (strcmp("mv", command.argv[0]) == 0) {
		mvnew(command.argc, command.argv);
		return 1;
	}
	if (strcmp("rm", command.argv[0]) == 0) {
		removenew(command.argc, command.argv);
		return 1;
	}
	if (strcmp("ps", command.argv[0]) == 0) {
		psnew(command.argc, command.argv);
		return 1;
	}
	if (strcmp("spwd", command.argv[0]) == 0) {
		spwdnew();
		return 1;
	}
	if (strcmp("who", command.argv[0]) == 0) {
		whonew();
		return 1;
	}
	return 0;
}

// excuteCommand() executes regular command, 
int excuteCommand() {

	int child_pid;
	int status;
	pid_t thisChPID;


	child_pid = fork();
	if (child_pid < 0) {
		fprintf(stderr, "Fork fails: \n");
		return 1;
	}
	else if (child_pid == 0) {
		/* CHILD */
		execve(command.name, command.argv, 0);
		printf("Child process failed\n");
		return 1;
	}
	else if (child_pid > 0) {
		/* PARENT */

		do {
			thisChPID = waitpid(child_pid, &status, WUNTRACED | WCONTINUED);
			if (thisChPID == -1) {
				perror("waitpid");
				exit(EXIT_FAILURE);
			}

			if (WIFEXITED(status)) {
				//printf("exited, status=%d\n", WEXITSTATUS(status));
				return 0;
			}
			else if (WIFSIGNALED(status)) {
				printf("killed by signal %d\n", WTERMSIG(status));
			}
			else if (WIFSTOPPED(status)) {
				printf("stopped by signal %d\n", WSTOPSIG(status));
			}
			else if (WIFCONTINUED(status)) {
				printf("continued\n");
			}
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		return 0;
	}

}


/*
main method
*/
int main(int argc, char* argv[]) {
	int i;
	int debug = 0;

	parsePath(pathv);
	welcomeMessage();

	// main loop
	while (TRUE) {
		printPrompt();

		commandInput = getchar(); //gets 1st char
		if (commandInput == '\n') { // if not input print prompt
			continue;
		}
		else {
			readCommand(commandLine, &commandInput); // read command

			if ((strcmp(commandLine, "exit") == 0) || (strcmp(commandLine, "quit") == 0))
				break;

			parseCommand(commandLine, &command); //parses command into argv[], argc

			if (checkInternalCommand() == 0) {
				command.name = lookupPath(command.argv, pathv);

				if (command.name == NULL) {
					printf("no cmd, please fix cmd\n");
					continue;
				}

				excuteCommand();
			}
		}
	}

	printf("\n");
	exit(EXIT_SUCCESS);
}

// print welcome message
void welcomeMessage() {
	printf("\nThis is SystemProgramming Proj\nKNU-2017-team10's minishell\n");
}

// print prompt
void printPrompt() {
	printf("mshell > ");
}

// looks up path using environment variable PATH
// function looks up is command is actually in path
// if it finds command in that path, it will return path
char * lookupPath(char **argv, char **dir) {
	char *result;
	char pName[MAX_PATH_LEN];
	if (*argv[0] == '/') {
		return argv[0];
	}
	else if (*argv[0] == '.') {
		if (*++argv[0] == '.') {
			if (getcwd(pName, sizeof(pName)) == NULL)
				perror("getcwd(): error\n");
			else {
				*--argv[0];
				asprintf(&result, "%s%s%s", pName, "/", argv[0]);
			}
			return result;
		}
		*--argv[0];
		if (*++argv[0] == '/') {
			if (getcwd(pName, sizeof(pName)) == NULL)
				perror("getcwd(): error\n");
			else {
				asprintf(&result, "%s%s", pName, argv[0]);
			}
			return result;
		}
	}

	// look in PAH directories, use access() to see if the
	// file is in the dir
	int i;
	for (i = 0; i < MAX_PATHS; i++) {
		if (dir[i] != NULL) {
			asprintf(&result, "%s%s%s", dir[i], "/", argv[0]);
			if (access(result, X_OK) == 0) {
				return result;
			}
		}
		else continue;
	}

	fprintf(stderr, "%s: command not found\n", argv[0]);
	return NULL;
}

// this function populates "pathv" with environment variable PATH
int parsePath(char* dirs[]) {
	int debug = 0;
	char* pathEnvVar;
	char* thePath;
	int i;

	for (i = 0; i < MAX_ARGS; i++)
		dirs[i] = NULL;
	pathEnvVar = (char*)getenv("PATH");
	thePath = (char*)malloc(strlen(pathEnvVar) + 1);
	strcpy(thePath, pathEnvVar);

	char* pch;
	pch = strtok(thePath, ":");
	int j = 0;
	// loop through the thePath for ':' delimiter between each path name
	while (pch != NULL) {
		pch = strtok(NULL, ":");
		dirs[j] = pch;
		j++;
	}

	//===================== debug ===============
	// print the directories if debugging
	if (debug) {
		printf("Directories in PATH variable\n");
		for (i = 0; i<MAX_PATHS; i++)
			if (dirs[i] != '\0')
				printf("    Directory[%d]: %s\n", i, dirs[i]);
	}
}

// this function parses commandLine into command.argv and command.argc
int parseCommand(char * commandLine, struct command_t * command) {
	int debug = 0;

	char * pch;
	pch = strtok(commandLine, " ");
	int i = 0;
	while (pch != NULL) {
		command->argv[i] = pch;
		pch = strtok(NULL, " ");
		i++;
	}
	command->argc = i;
	command->argv[i++] = NULL;

	// pay no attention to this
	//===================== debug ===============
	if (debug) {
		printf("Stub: parseCommand(char, struct);\n");
		printf("Array size: %i\n", sizeof(*command->argv));
		int j;
		for (j = 0; j<i; j++) {
			printf("command->argv[%i] = %s\n", j, command->argv[j]);
		}
		printf("\ncommand->argc = %i\n", command->argc);

		if (command->argv[0] != NULL) {
			//printf("*command->argv[%i] = %c\n", j, *command->argv[0]);
			char **p;
			for (p = &command->argv[1]; *p != NULL; p++) {
				printf("%s\n", *p);
			}
		}
	}
	return 0;
}

// this function read user input and save to commandLine
int readCommand(char * buffer, char * commandInput) {
	int debug = 0;
	buf_chars = 0;


	while ((*commandInput != '\n') && (buf_chars < LINE_LEN)) {
		buffer[buf_chars++] = *commandInput;
		*commandInput = getchar();
	}
	buffer[buf_chars] = '\0';

	//===================== debug ===============
	if (debug) {
		printf("Stub: readCommand(char *)\n");

		int i;
		for (i = 0; i<buf_chars; i++) {
			printf("buffer[%i] = %c\n", i, buffer[i]);
		}
		printf("\nlength: %i\n", buf_chars - 1);
		printf("\n1. buffer %s\n", buffer);
		printf("2. buffer[%i] = %c\n", buf_chars - 2, buffer[buf_chars - 2]);
		if (buffer[buf_chars - 1] == '\n')
			printf("3. buffer[%i] = '\\n'\n", buf_chars - 1);
		if (buffer[buf_chars] == '\0')
			printf("4. buffer[%i] = '\\0'\n", buf_chars);
	}
	return 0;
}
