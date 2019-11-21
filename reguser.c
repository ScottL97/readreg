#include <stdio.h>
#include <sys/types.h>
#include "ioctlreg.h"

int main(void)
{
	uint32_t val;
	int ret;
	int fd;
	fd = open("/dev/read_reg", O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(-2);
    }

	ret = ioctl(fd, IOCGREG, &val);
    if (ret) {
        perror("ioctl wrong");
        exit(1);
    }
    printf("read: %#x\n", val);
	close(fd);
}
