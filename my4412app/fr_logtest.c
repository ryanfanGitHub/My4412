#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "fr_logtest.h"
#include <stdarg.h>

LOG_PARAM_THREAD log_param_test;

//64K
#define LOG_MAX_SIZE 1024*64

#define BUF_SIZE 1024

static pid_t log_childPid;

int System_Log_Init(void)
{
	int pFds[2];
	int ret;
	LOG_PARAM_THREAD *para = &log_param_test;

	para->max_size = LOG_MAX_SIZE;
	
	ret = pipe(pFds);
	if(ret == 0){
		if( (log_childPid = fork()) == 0 ){ //chid proc 
			close(pFds[1]);
			para->pipe_rdFd = pFds[0]; //
		}else if(log_childPid > 0){ // father proc
			close(1);
			if(dup2(pFds[1], 1) < 0){
				printf("father dup2 failed: %s\n",sterror(errno));
				return -1;
			}
			para->pipe_wtFd = pFds[1]; // 
			close(pFds[0]);
		}
	}else{
		printf("log fork failed %s\n",sterror(errno));
		return -1;
	}
}
