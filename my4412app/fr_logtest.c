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
			Log_Process_Loop();
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

void Log_Process_Loop(LOG_PARAM_THREAD *para)
{
	int ret,tmp;
	int fd = -1;
	char file_name[32];
	int offset = 0;
	char *buf;
	time_t timer;
	struct tm *time_tmp;
	if( (buf = (char *)malloc(BUF_SIZE)) == NULL ){
		printf("malloc log buffer failed: %s\n",sterror(errno));
		return;
	}
	
	while(1){
		switch LOG_LOOP_CTL{
			case LOG_CTL_INIT:
				timer = time(NULL);
				time_tmp = localtime(&timer);
				sprintf(file_name, "%4d_%02d_%02d_%02d_%02d_%02d"".log", \
						time_tmp->tm_year+1900, time_tmp->tm_mon+1, time_tmp->tm_mday, \
						time_tmp->tm_hour, time_tmp->tm_min, time_tmp->tm_sec);
				fd = open(file_name, O_RDWR | O_CREAT, 0644);
				if(fd <0){
					printf("open log file failed: %s\n",sterror(errno));
					goto exit_;
				}
				LOG_LOOP_CTL = LOG_CTL_ING;
				break;
				
			case LOG_CTL_ING:
				
				break;
				
			case LOG_CTL_FAULT:
				
				break;
				
			default:
				printf("log loop ctl error\n");
	
		}
	}
	
exit1_:
	close(fd);

exit_:
	free(buf);
	close(para->pipe_rdFd);
	return;
	
}