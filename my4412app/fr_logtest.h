#ifndef _FR_LOG
#define _FR_LOG

typedef struct log_param{
	unsigned int max_size;
	int pipe_wtFd;
	int pipe_rdFd;
}LOG_PARAM_THREAD;

enum log_loop_ctl { 
	LOG_CTL_INIT = 0,
	LOG_CTL_ING,
	LOG_CTL_FAULT
} LOG_LOOP_CTL;