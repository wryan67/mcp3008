

// standard includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <getopt.h>
#include <math.h>

// engenerring
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "mcp3008.h"

using namespace std;
// using namespace std::chrono;

static volatile float refVolts = 5.0;


	// SPI Options
	bool loadSPIDriver = false;
	int  spiHandle = -1;
	int  spiChannel = 0;
    bool spiOverride = false;
    int  spiSpeed = 500000;

int channelType = MCP3008_SINGLE;

unsigned int readChannel(int channel)
{
    unsigned char buffer[3] = { 1, 0, 0 };
	buffer[1] = (channelType + channel) << 4;

    wiringPiSPIDataRW(spiChannel, buffer, 3);

	return ((buffer[1] & 3) << 8) + buffer[2];   
}

float getVolts(int channel) {
    auto bits = readChannel(channel);
	return ((bits)*refVolts) / 1024.0;
}


bool setup() {


	
	if (int ret = wiringPiSetup()) {
		fprintf(stderr, "Wiring Pi setup failed, ret=%d\n", ret);
		exit(EXIT_FAILURE);
	}

	// The speed parameter is an integer in the range 500,000 through 32,000,000 and represents the SPI clock speed in Hz


	if ((spiHandle = wiringPiSPISetup(spiChannel, spiSpeed)) < 0)
	{
		fprintf(stderr, "opening SPI bus failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}


// random number generator
	int seed;
	FILE *fp;
	fp = fopen("/dev/urandom", "r");
	fread(&seed, sizeof(seed), 1, fp);
	fclose(fp);
	srand(seed);


	return true;
}


int main(int argc, char **argv)
{
	if (!setup()) {
		printf("setup failed\n");
		exit(2);
	}



    while (true) {
        for (int ch=0; ch<MCP3008_CHANNELS; ++ch) {
          printf("%d: %5.3f | ", ch, getVolts(ch));
          delay(10);
        }
        printf("\r");
    }



}    




