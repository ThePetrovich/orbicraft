//Schsat Daemon
//Сейчас тут нет целевых алгоритмов, но они будут добавлены позже
//Демон работает и умеет запускать потоки

#if !defined(_GNU_SOURCE)
	#define _GNU_SOURCE 1
	#define _USE_GNU
#endif


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <sys/file.h>
#include <sys/resource.h>
#include <execinfo.h>
#include <unistd.h>
#include <errno.h>
#include <wait.h>
#include <ucontext.h>

#define FD_LIMIT			1024*10

#define CHILD_NEED_WORK			1
#define CHILD_NEED_TERMINATE	2

#define PID_FILE "/var/run/my_daemon.pid"

void WriteLog(char* Msg, ...)
{
	va_list args;
    va_start(args, Msg);
    vprintf(Msg, args);
    va_end(args);
}

int LoadConfig(char* FileName)
{
	return 1;
}

int ReloadConfig()
{
	return 1;
}
void DestroyWorkThread()
{
}

int InitWorkThread()
{
	WriteLog("[schsatd] Initializing work thread\n");
	return 0;
}

static void signal_error(int sig, siginfo_t *si, void *ptr)
{
	void*  ErrorAddr;
	void*  Trace[16];
	int    x;
	int    TraceSize;
	char** Messages;

	WriteLog("[schsatd] A critical error has occured while running schsatd.\n");
	WriteLog("[schsatd] schsatd will be restarted.\n");
	WriteLog("[schsatd] Error: %s, Addr: 0x%0.16X\n", strsignal(sig), si->si_addr);

	#if __WORDSIZE == 64
		ErrorAddr = (void*)((ucontext_t*)ptr)->uc_mcontext.gregs[REG_RIP];
	#else
		ErrorAddr = (void*)((ucontext_t*)ptr)->uc_mcontext.gregs[REG_EIP];
	#endif

	TraceSize = backtrace(Trace, 16);
	Trace[1] = ErrorAddr;

	Messages = backtrace_symbols(Trace, TraceSize);
	if (Messages)
	{
		WriteLog("--------------------------------Call-stack--------------------------------\n");

		for (x = 1; x < TraceSize; x++)
		{
			WriteLog("%s\n", Messages[x]);
		}

		WriteLog("--------------------------------------------------------------------------\n");
		free(Messages);
	}

	WriteLog("[schsatd] schsatd stopped\n");

	DestroyWorkThread();

	exit(CHILD_NEED_WORK);
}

int SetFdLimit(int MaxFd)
{
	struct rlimit lim;
	int           status;

	lim.rlim_cur = MaxFd;

	lim.rlim_max = MaxFd;

	status = setrlimit(RLIMIT_NOFILE, &lim);

	return status;
}


int WorkProc()
{
	struct sigaction sigact;
	sigset_t         sigset;
	int              signo;
	int              status;

	sigact.sa_flags = SA_SIGINFO;

	sigact.sa_sigaction = signal_error;

	sigemptyset(&sigact.sa_mask);

	sigaction(SIGFPE, &sigact, 0);
	sigaction(SIGILL, &sigact, 0);
	sigaction(SIGSEGV, &sigact, 0);
	sigaction(SIGBUS, &sigact, 0);

	sigemptyset(&sigset);

	sigaddset(&sigset, SIGQUIT);

	sigaddset(&sigset, SIGINT);

	sigaddset(&sigset, SIGTERM);

	sigaddset(&sigset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sigset, NULL);

	SetFdLimit(FD_LIMIT);

	WriteLog("[schsatd] schsatd started\n");

	status = InitWorkThread();

	if (!status){
		int fd;
		uint16_t cmd;
	    char * cmdQueue = "/tmp/schsatQueue";
	    char buf[256];

	    fd = open(cmdQueue, O_RDONLY);

		for (;;){
			/*sigwait(&sigset, &signo);

			if (signo == SIGUSR1){
				status = ReloadConfig();
				if (status == 0){
					WriteLog("[schsatd] Config reload failed\n");
				}
				else {
					WriteLog("[schsatd] Config succesfully reloaded\n");
				}
			}
			else {
				break;
			}*/
			read(fd, buf, 255);
    		printf("Received: %s\n", buf);
			sscanf(buf, "cmd+%x", &cmd);
			switch(cmd){
				case 0x00:
					break;
					break;
				case 0x01:
					system("/home/non-root/src/cubesat-src/build/sstest");
					break;
				case 0x02:
					system("/home/non-root/src/cubesat-src/build/photos");
					break;
				case 0x03:
					system("/home/non-root/src/cubesat-src/build/schsat-test1");
					break;
			}

		}
		close(fd);
		DestroyWorkThread();
	}
	else {
		WriteLog("[schsatd] Create work thread failed\n");
	}

	WriteLog("[schsatd] schsatd stopped\n");

	return CHILD_NEED_TERMINATE;
}


void SetPidFile(char* Filename)
{
	FILE* f;

	f = fopen(Filename, "w+");
	if (f){
		fprintf(f, "%u", getpid());
		fclose(f);
	}
}



int MonitorProc()
{
	int       pid;
	int       status;
	int       need_start = 1;
	sigset_t  sigset;
	siginfo_t siginfo;

	sigemptyset(&sigset);

	sigaddset(&sigset, SIGQUIT);

	sigaddset(&sigset, SIGINT);

	sigaddset(&sigset, SIGTERM);

	sigaddset(&sigset, SIGCHLD);

	sigaddset(&sigset, SIGCHLD);

	sigaddset(&sigset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sigset, NULL);

	WriteLog("[schsatd-mon] Monitor started\n");
	SetPidFile(PID_FILE);
	for (;;){
		if (need_start){
			pid = fork();
		}

		need_start = 1;

		if (pid == -1){
			WriteLog("[schsatd-mon] Process startup failed with code (%s)\n", strerror(errno));
		}
		else if (!pid){
			status = WorkProc();
			exit(status);
		}
		else {
			sigwaitinfo(&sigset, &siginfo);

			if (siginfo.si_signo == SIGCHLD){
				wait(&status);

				status = WEXITSTATUS(status);

				if (status == CHILD_NEED_TERMINATE){
					WriteLog("[schsatd-mon] Child process terminated\n");
					break;
				}
				else if (status == CHILD_NEED_WORK){
					WriteLog("[schsatd-mon] Child process restart\n");
				}
			}
			else if (siginfo.si_signo == SIGUSR1){
				kill(pid, SIGUSR1);
				need_start = 0;
			}
			else {
				WriteLog("[schsatd-mon] Caught signal %s\n", strsignal(siginfo.si_signo));

				kill(pid, SIGTERM);
				status = 0;
				break;
			}
		}
	}
	WriteLog("[schsatd-mon] Stopped\n");

	unlink(PID_FILE);

	return status;
}


int main(int argc, char** argv)
{
	int status;
	int pid;

	if (argc != 2){
		printf("Usage: ./my_daemon filename.cfg\n");
		return -1;
	}

	status = LoadConfig(argv[1]);

	if (!status){
		printf("Error: Load config failed\n");
		return -1;
	}

	pid = fork();

	if (pid == -1){
		printf("Start Daemon Error: %s\n", strerror(errno));

		return -1;
	}
	else if (!pid){
		umask(0);

		setsid();

		chdir("/");

		status = MonitorProc();

		return status;
	}
	else {
		return 0;
	}
}
