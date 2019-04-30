#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>

#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>

#include <linux/syscalls.h>
#include <linux/fcntl.h>

MODULE_LICENSE("GPL");

#define L1 18
#define L2 23
#define L3 27
#define L4 16
#define L5 17
#define L6 26
#define L7 13
#define L8 22

MODULE_LICENSE("GPL");
//int num_leds;
long num_leds;
int i;
int leds[10];
struct file *fp;
// Create variables
struct file *f;
char buf[128];
mm_segment_t fs;
int i;
int result;

static ssize_t set_period_callback(struct device* dev,
    struct device_attribute* attr,
    const char* buf,
    size_t count)
{
    long period_value = 0;
    
    //int period_value = 0;
    if (kstrtol(buf, 10, &period_value) < 0)
        return -EINVAL;
    if (period_value < 10)	//Safety check
    	return - EINVAL;
    //printk("%ld", period_value);
    num_leds = period_value / 10;
    //s_BlinkPeriod = period_value;
    for(i = 1; i < num_leds + 1; i++){
      gpio_set_value(leds[i], true);
    }
    for(i = num_leds + 1; i < 10; i ++){
      gpio_set_value(leds[i], false);
    }
    return count;
}

static DEVICE_ATTR(period, S_IRWXU | S_IRWXG, NULL, set_period_callback);

static struct class *s_pDeviceClass;
static struct device *s_pDeviceObject;

/*
struct file *file_open(const char *path, int flags, int rights) 
{
    struct file *filp = NULL;
    mm_segment_t oldfs;
    int err = 0;

    oldfs = get_fs();
    set_fs(get_ds());
    filp = filp_open(path, flags, rights);
    set_fs(oldfs);
    if (IS_ERR(filp)) {
        err = PTR_ERR(filp);
        return NULL;
    }
    return filp;
}

void file_close(struct file *file) 
{
    filp_close(file, NULL);
}


int file_read(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size) 
{
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    //ret = vfs_read(file, data, size, &offset);
    ret = read(file, data, size);

    set_fs(oldfs);
    return ret;
} 
*/
static int __init pix_init(void){
  //https://www.fsl.cs.sunysb.edu/kernel-api/re244.html
  leds[1] = 18;
  leds[2] = 23;
  leds[3] = 27;
  leds[4] = 16;
  leds[5] = 17;
  leds[6] = 26;
  leds[7] = 13;
  leds[8] = 22;
  
  printk(KERN_INFO "Initialized");

  gpio_request(leds[1], "L1");
  gpio_direction_output(leds[1], true);
  gpio_set_value(leds[1], false);

  gpio_request(leds[2], "L2");
  gpio_direction_output(leds[2], true);
  gpio_set_value(leds[2], false);

  gpio_request(leds[3], "L3");
  gpio_direction_output(leds[3], true);
  gpio_set_value(leds[3], false);

  gpio_request(leds[4], "L4");
  gpio_direction_output(leds[4], true);
  gpio_set_value(leds[4], false);

  gpio_request(leds[5], "L5");
  gpio_direction_output(leds[5], true);
  gpio_set_value(leds[5], false);

  gpio_request(leds[6], "L6");
  gpio_direction_output(leds[6], true);
  gpio_set_value(leds[6], false);

  gpio_request(leds[7], "L7");
  gpio_direction_output(leds[7], true);
  gpio_set_value(leds[7], false);

  gpio_request(leds[8], "L8");
  gpio_direction_output(leds[8], true);
  gpio_set_value(leds[8], false);
  
  //gpio_request(leds[9], "L9");
  //gpio_direction_output(leds[9], true);

  printk(KERN_INFO "Finished Initalization");
  //char *buf = kmalloc(48, GFP_KERNEL);

  // Read the cpu status from /proc/stat
  //fp = file_open("cpu_file", O_WRONLY|O_CREAT, 0644);
  //file_read(fp, 48, buf, sizeof(char));
  //file_close(fp);
  
  //num_leds = buf;
  
  // Init the buffer with 0
/*  for(i=0;i<128;i++)
      buf[i] = 0;
  // To see in /var/log/messages that the module is operating
  printk(KERN_INFO "My module is loaded\n");
  // I am using Fedora and for the test I have chosen following file
  // Obviously it is much smaller than the 128 bytes, but hell with it =)
  //f = filp_open("/home/pi/Desktop/ics612/ICS-612-Project-5/cpu_file", O_RDONLY, 0);
  //f = filp_open("cpu_file", O_RDONLY, 0);
  f = filp_open("/etc/fedora-release", O_RDONLY, 0);
  if(f == NULL)
      printk(KERN_ALERT "filp_open error!!.\n");
  else{
      // Get current segment descriptor
      fs = get_fs();
      // Set segment descriptor associated to kernel space
      set_fs(get_ds());
      // Read the file
      //http://www.voidcn.com/article/p-hyfwozve-bps.html
      f->f_pos = 0;
      f->f_op->read(f, buf, 128, &f->f_pos);
      //f->f_op->read(f, buf, 1, 0);
      // Restore segment descriptor
      set_fs(fs);
      // See what we read from file
      //printk(KERN_INFO "buf:%s\n",buf);
  }
  filp_close(f,NULL);
  */
  
    s_pDeviceClass = class_create(THIS_MODULE, "LedBlink");
    BUG_ON(IS_ERR(s_pDeviceClass));

    s_pDeviceObject = device_create(s_pDeviceClass, NULL, 0, NULL, "LedBlink");
    BUG_ON(IS_ERR(s_pDeviceObject));

    result = device_create_file(s_pDeviceObject, &dev_attr_period);
    BUG_ON(result < 0);
  /*
  num_leds = 7;
  for(i = 1; i < num_leds + 1; i++){
    gpio_set_value(leds[i], true);
  }
  for(i = num_leds + 1; i < 10; i ++){
    gpio_set_value(leds[i], false);
  }*/
  return 0;
}

static void __exit pix_exit(void){
  printk(KERN_INFO "Removing module");

  gpio_free(leds[1]);
  gpio_free(leds[2]);
  gpio_free(leds[3]);
  gpio_free(leds[4]);
  gpio_free(leds[5]);
  gpio_free(leds[6]);
  gpio_free(leds[7]);
  gpio_free(leds[8]);
  //gpio_free(leds[9]);

  device_remove_file(s_pDeviceObject, &dev_attr_period);
  device_destroy(s_pDeviceClass, 0);
  class_destroy(s_pDeviceClass);

  printk(KERN_INFO "Finished removing");
}

module_init(pix_init);
module_exit(pix_exit);
