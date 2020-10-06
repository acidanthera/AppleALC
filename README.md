AppleALC
========

[![Build Status](https://travis-ci.com/acidanthera/AppleALC.svg?branch=master)](https://travis-ci.com/acidanthera/AppleALC) [![Scan Status](https://scan.coverity.com/projects/16166/badge.svg?flat=1)](https://scan.coverity.com/projects/16166)

An open source kernel extension enabling native macOS HD audio for not officially supported codecs without any filesystem modifications.

#### Translate Language
- [简体中文](https://github.com/acidanthera/AppleALC/blob/master/README_CN.md)
- English ( Current )

#### Features
- Digital and analog audio support starting from the OS installation
- Recovery HD/macOS Installer audio support
- Automated codec detection
- Unsupported audio controller enabling (internal and external)
- Arbitrary kext patching
- Custom platform/layout injection
- Works with SIP / El Capitan
- Currently compatible with 10.8-11.0

#### Credits
- [Apple](https://www.apple.com) for macOS  
- [Onyx The Black Cat](https://github.com/gdbinit/onyx-the-black-cat) by [fG!](https://reverse.put.as) for the base of the kernel patcher  
- [capstone](https://github.com/aquynh/capstone) by [Nguyen Anh Quynh](https://github.com/aquynh) for the disassembler module  
- [toleda](https://github.com/toleda), [Mirone](https://github.com/Mirone) and certain others for audio patches and layouts
- [Pike R. Alpha](https://github.com/Piker-Alpha) for [lzvn](https://github.com/Piker-Alpha/LZVN) decompression and certain HDMI patches  
- [07151129](https://github.com/07151129) for some code parts and suggestions  
- [vit9696](https://github.com/vit9696) for writing the software and maintaining it
- [vandroiy2013](https://github.com/vandroiy2013) for maintaining the codec database

#### Installation
The minimal instruction is available on the [wiki](https://github.com/vit9696/AppleALC/wiki).  
The prebuilt binaries are available on [releases](https://github.com/vit9696/AppleALC/releases) page.

#### Contribution
To support more audio codecs in the binary packages you are asked to submit your configurations. Please read the [wiki](https://github.com/vit9696/AppleALC/wiki) for more details. For the contributors with programming skills the headers are filled with AppleDOC comments.

#### Support and discussion
[InsanelyMac topic](http://www.insanelymac.com/forum/topic/311293-applealc-—-dynamic-applehda-patching/) in English  
[AppleLife topic](https://applelife.ru/threads/applealc-dinamicheskij-patching-applehda.1171672/) in Russian

#### Donations
Writing and supporting code is fun but it takes time. If you want to thank the author for his work consider contributing, bugreporting, or providing the support to other users.
####
yeah you are right that perfect code takes time
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <errno.h>

#define CMD_HISTORY_SIZE 10 

static char * cmd_history[CMD_HISTORY_SIZE];
static int cmd_history_count = 0;


static void exec_cmd(const char * line)
{
	char * CMD = strdup(line);
	char *params[10];
	int argc = 0;

	params[argc++] = strtok(CMD, " ");
	while(params[argc-1] != NULL){	
		params[argc++] = strtok(NULL, " ");
	}

	argc--; 


	int background = 0;
	if(strcmp(params[argc-1], "&") == 0){
		background = 1; 
		params[--argc] = NULL;	
	}

	int fd[2] = {-1, -1};	

	while(argc >= 3){


		if(strcmp(params[argc-2], ">") == 0){	// output


			fd[1] = open(params[argc-1], O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP|S_IWGRP);
			if(fd[1] == -1){
				perror("open");
				free(CMD);
				return;
			}

			params[argc-2] = NULL;
			argc -= 2;
		}else if(strcmp(params[argc-2], "<") == 0){ // input
			fd[0] = open(params[argc-1], O_RDONLY);
			if(fd[0] == -1){
				perror("open");
				free(CMD);
				return;
			}
			params[argc-2] = NULL;
			argc -= 2;
		}else{
			break;
		}
	}

	int status;
	pid_t pid = fork();	
	switch(pid){
		case -1:	
			perror("fork");
			break;
		case 0:	// child
			if(fd[0] != -1){	
				if(dup2(fd[0], STDIN_FILENO) != STDIN_FILENO){
					perror("dup2");
					exit(1);
				}
			}
			if(fd[1] != -1){	
				if(dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO){
					perror("dup2");
					exit(1);
				}
			}
			execvp(params[0], params);
			perror("execvp");
			exit(0);
		default: // parent
			close(fd[0]);close(fd[1]);
			if(!background)
				waitpid(pid, &status, 0);
			break;
	}
	free(CMD);
}


static void add_to_history(const char * cmd){
	if(cmd_history_count == (CMD_HISTORY_SIZE-1)){	
		int i;
		free(cmd_history[0]);	

		for(i=1; i < cmd_history_count; i++)
			cmd_history[i-1] = cmd_history[i];
		cmd_history_count--;
	}
	cmd_history[cmd_history_count++] = strdup(cmd);	
}



static void run_from_history(const char * cmd){
	int index = 0;
	if(cmd_history_count == 0){
		printf("No commands in history\n");
		return ;
	}

	if(cmd[1] == '!') 
		index = cmd_history_count-1;
	else{
		index = atoi(&cmd[1]) - 1;	
		if((index < 0) || (index > cmd_history_count)){ 
			fprintf(stderr, "No such command in history.\n");
			return;
		}
	}
	printf("%s\n", cmd);	
	exec_cmd(cmd_history[index]);	
}



static void list_history(){
	int i;
	for(i=cmd_history_count-1; i >=0 ; i--){
		printf("%i %s\n", i+1, cmd_history[i]);
	}
}



static void signal_handler(const int rc){
	switch(rc){
		case SIGTERM:
		case SIGINT:
			break;
		
		case SIGCHLD:
			
			while (waitpid(-1, NULL, WNOHANG) > 0);
			break;
	}
}

// main

int main(int argc, char *argv[]){

	

	struct sigaction act, act_old;
	act.sa_handler = signal_handler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	if(	(sigaction(SIGINT,  &act, &act_old) == -1) ||
		(sigaction(SIGCHLD,  &act, &act_old) == -1)){ 
		perror("signal");
		return 1;
	}

	

	size_t line_size = 100;
	char * line = (char*) malloc(sizeof(char)*line_size);
	if(line == NULL){	
		perror("malloc");
		return 1;	
	}

	int inter = 0; 
	while(1){
		if(!inter)
			printf("mysh > ");
		if(getline(&line, &line_size, stdin) == -1){	
			if(errno == EINTR){	  
				clearerr(stdin); 
				inter = 1;	 
				continue; 
			}
			perror("getline");
			break;
		}
		inter = 0; 

		int line_len = strlen(line);
		if(line_len == 1){	
			continue;
		}
		line[line_len-1] = '\0'; 

		
		if(strcmp(line, "exit") == 0){ 
			break;
		}else if(strcmp(line, "history") == 0){ 
			list_history();
		}else if(line[0] == '!'){ 
			run_from_history(line);
		}else{ 
			add_to_history(line); 
			exec_cmd(line); 
		}
	}

	free(line);
	return 0;
}
