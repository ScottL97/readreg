#include <linux/types.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <asm/io.h>
#include "ioctlreg.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("lxh");
MODULE_DESCRIPTION("Read register");

static inline uint32_t io_read_32(void __iomem *addr)
{
 return *(volatile uint32_t*)addr;
}
static inline void io_write_32(void __iomem *addr, uint32_t value)
{
 *(volatile uint32_t*)addr = value;
}

static long readreg_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret;
	struct ioctl_data data;
	static void __iomem *vaddr;
	/* 检查设备类型 */
    if (_IOC_TYPE(cmd) != IOC_MAGIC) {
        pr_err("[%s] command type [%c] error!\n", \
            __func__, _IOC_TYPE(cmd));
        return -ENOTTY; 
    }

    /* 检查序数 */
    if (_IOC_NR(cmd) > IOC_MAXNR) { 
        pr_err("[%s] command numer [%d] exceeded!\n", 
            __func__, _IOC_NR(cmd));
        return -ENOTTY;
    }    

	if(copy_from_user(&data, (struct ioctl_data *)arg, sizeof(struct ioctl_data))) {
		return -EFAULT;
	}
	switch(cmd) { 
		case READREG:
		{
    		//将I/O内存资源的物理地址映射到核心虚地址空间中，映射4字节的大小
    		vaddr = ioremap(0x80028780000 + data.ch * 0x10000 + data.offset, 4);
    		data.rval = io_read_32(vaddr); //offset = 0x8
			iounmap(vaddr);
			ret = copy_to_user((struct ioctl_data __user *)arg, \
                &data, sizeof(struct ioctl_data));
        	if (ret) 
            	return -EFAULT;
		}	break;
		case WRITEREG:
		{
    		vaddr = ioremap(0x80028780000 + data.ch * 0x10000 + data.offset, 4);
			io_write_32(vaddr, data.wval);
			iounmap(vaddr);
		}
		default:
			return -ENOTTY;
	}
    return 0x0;
}
static struct file_operations ops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = readreg_ioctl,
};
static struct miscdevice tmp_dev =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "read_temp",
	.fops = &ops,
};

static int __init init(void)
{
	printk(KERN_INFO "start\n");				
	misc_register(&tmp_dev);
	return 0;
}
static void __exit finished(void)
{
    printk(KERN_INFO "exit\n");
	misc_deregister(&tmp_dev);
}

module_init(init);
module_exit(finished);
