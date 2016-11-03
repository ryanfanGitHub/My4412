#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/module.h>

#include <asm/io.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/platform_device.h>
#include <mach/regs-gpio.h>

//#define LED_MA 200
#define LED_MI 1
#define DEV_NAME "fr_led"

#define LED_MAGIC 0x66
#define LED_ON  _IOW(LED_MAGIC, 0, int)
#define LED_OFF _IOW(LED_MAGIC, 1, int)

#define LED1 EXYNOS4_GPL2(0)

/*
typedef struct{
	struct cdev cdev;
	int lednum;
	char *buf;
}LedCdev;

LedCdev ledcdev;
*/


static int led_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int led_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static long led_unlocked_ioctl(struct file *filp, unsigned int cmd,unsigned long arg)
{
	switch(cmd){
		case LED_ON:
			gpio_set_value(LED1, 1);
			break;
		case LED_OFF:
			gpio_set_value(LED1, 0);
			break;
		default:
			printk("Wrong cmd\n");
			return -EINVAL;

	}
	return 0;
}

static struct file_operations led_fops ={
	.owner = THIS_MODULE,
	.open = led_open,
	.release = led_release,
	.unlocked_ioctl = led_unlocked_ioctl,
};

static struct miscdevice miscled = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEV_NAME,
	.fops = &led_fops,
};


/*
led low level init
*/
static int led_gpioinit(void)
{
	int ret;
	
	ret = gpio_request(LED1,DEV_NAME);
	if(ret){
		printk("request gpio failed\n")
		return ret;
	}
	s3c_gpio_cfgpin(LED1,S3C_GPIO_OUTPUT);
	gpio_set_value(LED1,1);
	return 0;
}


static int led_probe(struct platform_device *pdev)
{
	int ret;

	led_gpioinit();
	ret = misc_register(&miscled);
	if(ret != 0){
		printk("register led failed\n");
		return ret;
	}
	return 0;
	
}

static int led_remove(struct platform_device *pdev)
{

}

static int led_suspend(struct platform_device *pdev)
{
	printk("led suspend\n");
	return 0;
}

static struct platform_driver led_driver = {
	.probe = led_probe,
	.remove = led_remove,
	.suspend = led_suspend,
	.driver = {
		.name = DEV_NAME,
		.owner = THIS_MODULE,
	},
}
static void __init led_init(void)
{
	platform_driver_unregister(&led_driver);
}

static void __exit led_exit(void)
{
	return platform_driver_register(&led_driver);
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("Dual BSD/GPL");
