//============================================================================
// Name        : uartR2.cpp
// Author      : Uonsong Ly
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//============================================================================
// Name        : uart2.cpp
// Author      : Uonsong Ly
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//**************************************************************
//*********************************************************************
//uart1.cpp - configures BeagleBone uart1 for tranmission and 9600 Baud
//and repeatedly sends the character G via uart1 tx pin (P9, 24)
//
//To configure the UART first do:
// # dtc -O dtb -o uart1-00A0.dtbo -b 0 -@ uart1.dts
// # cp uart1-00A0.dtbo /lib/firmware/uart1-00A0.dtbo
// # echo uart1 > /sys/devices/bone_capemgr.8/slots
//
//Check that the pinmux has been configured via:
// # cat /sys/kernel/debug/pinctrl/44e10800.pinmux/pinmux-pins
// pin 96 (44e10980): serial1_pinmux_helper.14 (GPIO UNCLAIMED)
//function pinmux_serial1_pins group pinmux_serial1_pins
// pin 97 (44e10984): serial1_pinmux_helper.14 (GPIO UNCLAIMED)
// function pinmux_serial1_pins group pinmux_serial1_pins
//
//*********************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/time.h>

#define SIZE 10

void delay_sec(float delay_value);
void delay_us(int desired_delay_us);

int main(void) {
	//define file handle for uart1
	//FILE *ofp_uart1_tx, *ofp_uart1_rx;
	//uart1 configuration using termios
	FILE *fRead;
	termios uart1;//uart2;
	int fd1; //fd2;

	/****************************** Debug **********************************/
	//open uart1 for tx/rx, not controlling device
	if((fd1 = open("/dev/ttyO1", O_RDONLY | O_NOCTTY)) < 0) //O_NOCTTY
		printf("Unable to open uart1 access.\n");
	//if((fd2 = open("/dev/ttyO2", O_RDWR | O_NOCTTY)) < 0)
			//printf("Unable to open uart2 access.\n");
	//get attributes of uart1
	if(tcgetattr(fd1, &uart1) < 0)
		printf("Could not get attributes of UART1 at ttyO1\n");
	//if(tcgetattr(fd2, &uart2) < 0)
			//printf("Could not get attributes of UART1 at ttyO1\n");
  //set Baud rate
  if(cfsetospeed(&uart1, B9600) < 0)
    printf("Could not set baud rate\n");
  //if(cfsetospeed(&uart2, B9600) < 0)
      //printf("Could not set baud rate\n");
  //else
    //printf("Baud rate: 9600\n");
	/****************************** Debug **********************************/

  //set attributes of uart1
  uart1.c_iflag = 0;
  uart1.c_oflag = 0;
  uart1.c_lflag = 0;

  struct timeval tv_now;
  struct timeval tv_start;

  tcsetattr(fd1, TCSANOW, &uart1);
  //tcsetattr(fd1, TCSANOW, &uart2);
  //char byte_outA[] = {0x41,0x42,0x43,0x44,0x45,0x46};
  /*char byte_outB[] = {0x42};
  char byte_outC[] = {0x43};*/
  //printf("%d\n", strlen(byte_outA));
  //char byte_empty[SIZE];
  //set ASCII character G repeatedly
  //int testWrite = 0; //testWrite2 = 0, testRead = 0;
  //int n = 0;
  char *test = (char*) malloc(50);
  printf("Input file name(.txt): ");
  scanf("%s", test);
  fRead = fopen(test,"a+");

  char *ain_str = (char*)malloc(2);
  ain_str[1]= '\0';
  int timedif = 0;
  //int first = 1;
  int testRead = 0;
  while(1) {


  	/**read(fd1, ain_str, 11);**/
  	//printf("Byte: %d", testRead);
  	//if (testRead != 11)
  		//printf("Read error\n");

  	//printf("%s\n", ain_str);

  	/**fRead = fopen("testR.txt", "a+");**/
  	//if (n == 0) {
  	gettimeofday(&tv_start, NULL);
  	testRead = read(fd1, ain_str, 1);


  	if (testRead != 1)
  		printf("Read error\n");
  		//fRead = fopen("testR.txt", "a+");
  		//fprintf(fRead,"%s",ain_str);
  	//} else {
  		//testRead = read(fd1, ain_str, 11);
  		//fRead = fopen("testR.txt", "a+");
  	if (ain_str[0] == ';')
  		fprintf(fRead,"%s\n",ain_str);
  	else
  		fprintf(fRead,"%s",ain_str);
  //	}
  	//printf("Byte: %d", testRead);
  	//printf("String: %s\n", ain_str);
  	//fclose(fRead);

  	if (ain_str[0] == 'x')
  		break;
  	//n++;
  	gettimeofday(&tv_now, NULL);
  	timedif = tv_now.tv_usec - tv_start.tv_usec;
  	printf("time: %d\n", timedif);

  	//delay_us(10000);
  }


  close(fd1);
  fclose(fRead);
}



void delay_sec(float delay_value) {
  time_t now, later;
  now = time(NULL);
  later = time (NULL);
  while(difftime(later, now) < delay_value){
    later = time(NULL);
    //keep checking time
  }
}

void delay_us(int desired_delay_us) {
	struct timeval tv_start; //start time hack
	struct timeval tv_now; //current time hack
	int elapsed_time_us;
	gettimeofday(&tv_start, NULL);
	elapsed_time_us = 0;
	while(elapsed_time_us < desired_delay_us) {
		gettimeofday(&tv_now, NULL);
		if(tv_now.tv_usec >= tv_start.tv_usec)
			elapsed_time_us = tv_now.tv_usec - tv_start.tv_usec;
		else
			elapsed_time_us = (1000000 - tv_start.tv_usec) + tv_now.tv_usec;
		//printf("start: %ld \n", tv_start.tv_usec);
		//printf("now: %ld \n", tv_now.tv_usec);
		//printf("desired: %d \n", desired_delay_ms);
		//printf("elapsed: %d \n\n", elapsed_time_ms);
	}
}

//***********************************************************

