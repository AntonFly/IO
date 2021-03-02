#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daniil Tolstov Avramenko Anton");
MODULE_DESCRIPTION("Lab 1 io-systems");
MODULE_VERSION("0.1");

struct cdev device;

#define MAX_VALUES 100
#define BUFSIZE 1000
static struct proc_dir_entry* entry;
#define MY_MAJOR       42
#define MY_MAX_MINORS  1
static const char d_name[] = "var1";
static const char d_greeting[] = "[VAR1]: ";
static struct proc_dir_entry* entry;
static int values[MAX_VALUES];
static int values_idx;
static dev_t d_number;
static struct cdev c_dev;
static struct class *cl;
static int dev_major;
static int dev_minor;

static void read(char *buf, int * const length)
{
	char *str_ptr = buf;
	str_ptr += sprintf(buf, d_greeting);

	int i;
	for (i = 0; i < values_idx; i++)
	{
		str_ptr += sprintf(str_ptr, "%d ", values[i]);
	}
	*(str_ptr++) = '\n';
	*(str_ptr++) = '\0';

	*length = str_ptr - buf;
}

static ssize_t proc_write(struct file *file, const char __user * buf, size_t count, loff_t* off) 
{
	return -1;
}

static ssize_t proc_read(struct file *file, char __user * buf, size_t len, loff_t* off) 
{
	char str[MAX_VALUES * sizeof(int) + sizeof(d_greeting)];
	int length;
	read(str, &length);

	if (*off > 0 || len < length)
	{
		printk(KERN_INFO "[VAR1 (%d %d)]: FAILED to read proc file: invalid offset", dev_major, dev_minor);
		return 0;
	}

	if (copy_to_user(buf, str, length) != 0)
	{
		printk(KERN_INFO "[VAR1 (%d %d)]: FAILED to read proc file: failed copy to user buffer", dev_major, dev_minor);
		return -EFAULT;
	}

	*off += length;
	return length;
}

static ssize_t module_read(struct file *file, char __user *buff,
                   size_t size, loff_t *off)
{
	char str[MAX_VALUES * sizeof(int) + sizeof(d_greeting)];
	int length;
	read(str, &length);

	printk(KERN_INFO "[VAR1 %d %d]: %s", dev_major, dev_minor, str);
	return 0;
}

static int module_open(struct inode *i, struct file *f)
{
	return 0;
}

static ssize_t module_write(struct file *f, const char __user *ubuf,  size_t len, loff_t *off)
{
	char buf[BUFSIZE];

	if (*off > 0 || len > BUFSIZE)
	{
		printk(KERN_INFO "[VAR1 (%d %d)]: error occour, buffer to long\n", dev_major, dev_minor);
		return -EFAULT;
	}

	if (copy_from_user(buf, ubuf, len))
		return -EFAULT;

	int i;
	int num = 0;
	for (i = 0; i < len; i++)
	{
				char ch = buf[i];
				if (ch != '\n')
					num++;
	}

	if (values_idx >= MAX_VALUES)
		return -EFAULT; 

	values[values_idx++] = num;

	int str_len = strlen(buf);
	*off += str_len;
	return str_len;
}

static int module_close(struct inode *i, struct file *f)
{
	return 0;
}

const struct file_operations module_fops = {
    .owner = THIS_MODULE,
    .read = module_read,
    .write = module_write,
    .open = module_open,
    .release = module_close,
};

static struct file_operations proc_fops = {
	.owner = THIS_MODULE,
	.read = proc_read,
	.write = proc_write,
};

static char *set_devnode(struct device *dev, umode_t *mode)
{
	if (mode != NULL)
		*mode = 0666;
	return NULL;
}


static int __init proc_example_init(void)
{
	if (alloc_chrdev_region(&d_number, 0, 1, d_name) != 0)
		return -EFAULT;

    if ((cl = class_create(THIS_MODULE, d_name)) == NULL)
		goto dev_region_destroy;

	cl->devnode = set_devnode;

    if (device_create(cl, NULL, d_number, NULL, d_name) == NULL)
		goto class_destroy;

    cdev_init(&c_dev, &module_fops);

    if (cdev_add(&c_dev, d_number, 1) < 0)
		goto dev_destroy;
	
	if ((entry = proc_create(d_name, 0444, NULL, &proc_fops)) == NULL)
		goto dev_destroy;

	dev_major = MAJOR(d_number);
	dev_minor = MINOR(d_number);

	printk(KERN_INFO "[VAR1 (%d %d)]: initialized\n", dev_major, dev_minor);
	return 0;

dev_destroy:
	device_destroy(cl, d_number);
class_destroy:
	class_destroy(cl);
dev_region_destroy:
	unregister_chrdev_region(d_number, 1);
	return -EFAULT;
}

static void __exit proc_example_exit(void)
{
	proc_remove(entry);
	printk(KERN_INFO "%s: proc file is deleted\n", THIS_MODULE->name);
	device_destroy(cl, d_number);
	cdev_del(&device);
	class_destroy(cl);
	//unregister_chrdev_region(MKDEV(MY_MAJOR, 0), 1);
	printk(KERN_INFO "Device deleted\n");
}

module_init(proc_example_init);
module_exit(proc_example_exit);

