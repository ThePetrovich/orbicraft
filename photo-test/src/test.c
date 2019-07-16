#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

int main(){
	char cmdbuf[200];
	int n = 0, iso = 100;
	printf("Camera test\n");
	//off, auto, night, nightpreview, verylong
	char exp[6][16] = {"off", "auto", "night", "nightpreview", "verylong"};
	for(int i = 0; i < 16; i++){
		printf("ISO calibration image #%d (no lights)\n", n);
		sprintf(cmdbuf, "raspistill -t 1000 -v -o img%d.jpg -ISO %d", n, 100 + 50*i);
		printf("%s\n", cmdbuf);
		system(cmdbuf);
		n++;
		sleep(1);
	}
	printf("Waiting 10 seconds before next image series\n");
	sleep(10);
	for(int i = 0; i < 16; i++){
		printf("ISO calibration image #%d (with lights)\n", n);
		sprintf(cmdbuf, "raspistill -t 1000 -v -o img%d.jpg -ISO %d", n, 100 + 50*i);
		printf("%s\n", cmdbuf);
		system(cmdbuf);
		n++;
		sleep(1);
	}
	printf("Enter optimal ISO:");
	scanf("%d", &iso);
	for(int i = 0; i < 5; i++){
		printf("Exposure calibration image #%d (no lights)\n", n);
		sprintf(cmdbuf, "raspistill -t 1000 -v -o img%d.jpg -ISO %d -ex %s", n, iso, exp[i]);
		printf("%s\n", cmdbuf);
		system(cmdbuf);
		n++;
		sleep(1);
		printf("Exposure calibration image #%d (no lights)\n", n);
		sprintf(cmdbuf, "raspistill -t 1000 -v -o img%d.jpg -ISO %d -ex %s", n, iso, exp[i]);
		printf("%s\n", cmdbuf);
		system(cmdbuf);
		n++;
		sleep(1);
	}
	printf("Waiting 10 seconds before next image series\n");
	sleep(10);
	for(int i = 0; i < 5; i++){
		printf("Exposure calibration image #%d (with lights)\n", n);
		sprintf(cmdbuf, "raspistill -t 1000 -v -o img%d.jpg -ISO %d -ex %s", n, iso, exp[i]);
		printf("%s\n", cmdbuf);
		system(cmdbuf);
		n++;
		sleep(1);
		printf("Exposure calibration image #%d (with lights)\n", n);
		sprintf(cmdbuf, "raspistill -t 1000 -v -o img%d.jpg -ISO %d -ex %s", n, iso, exp[i]);
		printf("%s\n", cmdbuf);
		system(cmdbuf);
		n++;
		sleep(1);
	}
}
