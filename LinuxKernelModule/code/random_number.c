#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>        // Required for the copy to user function
#include <linux/random.h>         // Random class for random number

#define DEVICE_NAME "random_number"
#define CLASS_NAME "random_class"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Le Thanh Cong");
MODULE_DESCRIPTION("A simple Linux driver creates a random number");
MODULE_VERSION("1.0");

static DEFINE_MUTEX(random_number_mutex);
static int majorNumber;
static int numberOpens=0;        
static int rand;
static char message[256]={0};

static struct class* randomClass = NULL;
static struct device* randomDevice = NULL;

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *,size_t, loff_t *);

//we only need to create a character device with open, read, and close operations
//we do not need write operation because we only need to read random number
static struct file_operations fops =
{
	.open = dev_open,
	.read = dev_read,
	.release = dev_release,
};

static int __init random_number_init(void){
    mutex_init(&random_number_mutex);       /// Initialize the mutex lock dynamically at runtime

	printk(KERN_INFO "RandomNumber: Initializing the RandomNumber LKM\n");

	//try to dynamically a major number for the device
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber<0){
      printk(KERN_ALERT "RandomNumber failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "RandomNumber: registered correctly with major number %d\n", majorNumber);

   //register device class
   randomClass=class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(randomClass)){
   		unregister_chrdev(majorNumber,DEVICE_NAME);
   		printk(KERN_ALERT "Failed to register a device class\n");
   		return PTR_ERR(randomClass);
   }
   printk(KERN_INFO "RandomNumber: device class registerd correctly\n");

   //register the device driver
   randomDevice = device_create(randomClass,NULL, MKDEV(majorNumber,0),NULL,DEVICE_NAME);
   if (IS_ERR(randomDevice)){
   	class_destroy(randomClass);
   	unregister_chrdev(majorNumber,DEVICE_NAME);
   	printk(KERN_ALERT "Failed to create the device\n");
   	return PTR_ERR(randomDevice);
   }
   printk(KERN_INFO "RandomNumber: device class created correctly\n");

   return 0;

}

static void __exit random_number_exit(void){
	device_destroy(randomClass,MKDEV(majorNumber,0));
	class_unregister(randomClass);
	class_destroy(randomClass);
	unregister_chrdev(majorNumber,DEVICE_NAME);
	mutex_destroy(&random_number_mutex); 
	printk(KERN_INFO "RandomNumber: Goodbye from the LKM\n");
}

static int dev_open(struct inode *inodep, struct file *filep){
    if(!mutex_trylock(&random_number_mutex)){    
		printk(KERN_ALERT "RandomNumber: Device in use by another process");
      	return -EBUSY;
   	}	

    numberOpens++;
	printk(KERN_INFO "RandomNumber: Device has been opened %d time(s)\n",numberOpens);

	get_random_bytes(&rand, sizeof(int));
	//rand=rand%100; if you want to random in [-99,99]
	printk(KERN_INFO "RandomNumber: %d\n",rand);

	return 0;
}

static ssize_t dev_read(struct file *filep, char* buffer,size_t len, loff_t *offset){
   int error_count = 0;
   sprintf(message,"%d",rand);
  
   // copy_to_user has the format ( * to, *from, size) and returns 0 on success
   error_count = copy_to_user(buffer, message, strlen(message));

   if (error_count==0){         
      printk(KERN_INFO "RandomNumber: Sent random number to the user successfully\n");
      return 0;
   }
   else {	
      printk(KERN_INFO "RandomNumber: Failed to send the random number to the user\n");
      return -EFAULT;             
   }
}

static int dev_release(struct inode *inodep,struct file* filep){
	printk(KERN_INFO "The device is successfully closed\n");
	mutex_unlock(&random_number_mutex); 
	return 0;
}

module_init(random_number_init);
module_exit(random_number_exit);
