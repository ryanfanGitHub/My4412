#ifndef _FR_LOG
#define _FR_LOG

typedef struct log_param{
	unsigned int max_size;
	int pipe_wtFd;
	int pipe_rdFd;
}LOG_PARAM_THREAD;
