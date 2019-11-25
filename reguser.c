#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ioctlreg.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

struct ioctl_data userdata;
int fd;
static int read_reg(uint8_t ch, uint32_t address);
int main(void)
{
	uint8_t ch = 0;

	fd = open("/dev/read_reg", O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(-2);
    }
	for(;ch < 8; ch++)
	{
		if(read_reg(ch, 0x78))
			printf("read ch %d 0x%x: %#x\n", ch, userdata.address, userdata.val);			
		if(read_reg(ch, 0x80))
			printf("read ch %d 0x%x: %#x\n", ch, userdata.address, userdata.val);			
	printf("------------------------\n");
	}
	
	close(fd);
}
static int read_reg(uint8_t ch, uint32_t address)
{
	userdata.ch = ch;
	userdata.val = 0;
	userdata.address = address;
	int ret = ioctl(fd,READREG,&userdata);
    if (ret) {
        perror("ioctl wrong");
        return 0;
    }
	return 1;
}
