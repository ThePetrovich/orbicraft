#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(){
	int fd;
	char * cmdQueue = "/tmp/schsatQueue";
	uint16_t cmd;
	mkfifo(cmdQueue, 0666);

	fd = open(cmdQueue, O_WRONLY);
	while(1){
		if(scanf("%x", &cmd) > 0){
			switch(cmd){
				case 0x00:
					write(fd, "cmd+0x00", sizeof("cmd+0x00")); //Quit command
					break;
				case 0x01:
					write(fd, "cmd+0x01", sizeof("cmd+0x01")); //Calibrate AVS
					break;
				case 0x02:
					write(fd, "cmd+0x02", sizeof("cmd+0x02")); //Calibrate AVS
					break;
				case 0x03:
					write(fd, "cmd+0x03", sizeof("cmd+0x03")); //Calibrate AVS
					break;
				default:
					break;
			}
		}
	}
}
