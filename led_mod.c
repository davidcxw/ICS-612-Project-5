// Michael Omori, David Wang
// ICS 612 Project 5
// LED LKM for Raspberry Pi 3 B+

#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>

//#include <stdio.h>
//#include <unistd.h>
//#include <wiringPi.h>
//#include <string.h>
//#include <stdlib.h>

#define L1 18
#define L2 23
#define L3 27
#define L4 16
#define L5 17
#define L6 26
#define L7 13
#define L8 22
const int number_leds = 8;

int get_cpu_usage(void);
void update(void);
int count = 0;
int i = 0;
char *cpuline;
long int user_jif, nice_jif, system_jif, idle_jif;
long int iowait_jif;
long int tjif;
long int delta_total;
long int delta_idle;
long int delta_usage;
unsigned int base;
int error;
int leds_lit;

MODULE_LICENSE("GPL");

static int __init pix_init(void){
  printk(KERN_INFO "Initialized");

  gpio_request(L1, "L1");
  gpio_direction_output(L1, true);
  gpio_set_value(L1, true);

  gpio_request(L2, "L2");
  gpio_direction_output(L2, true);
  gpio_set_value(L2, false);

  gpio_request(L3, "L3");
  gpio_direction_output(L3, true);
  gpio_set_value(L3, true);

  gpio_request(L4, "L4");
  gpio_direction_output(L4, true);
  gpio_set_value(L4, false);

  gpio_request(L5, "L5");
  gpio_direction_output(L5, true);
  gpio_set_value(L5, true);

  gpio_request(L6, "L6");
  gpio_direction_output(L6, true);
  gpio_set_value(L6, false);

  gpio_request(L7, "L7");
  gpio_direction_output(L7, true);
  gpio_set_value(L7, true);

  gpio_request(L8, "L8");
  gpio_direction_output(L8, true);
  gpio_set_value(L8, false);

  printk(KERN_INFO "Finished Initalization");

  while(1)
  {
       update();
       //sleep(1);
       //udelay(1000000);
       count += 1;
       if(count > 10) {
           break;
       }
  }
  return 0;
}

static void __exit pix_exit(void){
  printk(KERN_INFO "Removing module");

  gpio_free(L1);
  gpio_free(L2);
  gpio_free(L3);
  gpio_free(L4);
  gpio_free(L5);
  gpio_free(L6);
  gpio_free(L7);
  gpio_free(L8);

  printk(KERN_INFO "Finished removing");
}

int previous_tjif = 0;
int previous_ijif = 0;

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

    ret = vfs_read(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
} 

// Return value 0-100
int get_cpu_usage(void)
{
    char *buf = kmalloc(48, GFP_KERNEL);

    // Read the cpu status from /proc/stat
    struct file *fp = file_open("/proc/stat", O_WRONLY|O_CREAT, 0644);
    file_read(fp, 48, buf, sizeof(char));
    file_close(fp);

    // Strip the prefixed "cpu  "
    cpuline = kmalloc(48, GFP_KERNEL);
    for(i=5; i<strlen(buf); i++) {
        cpuline[i-5] = buf[i];
    }
    cpuline[40] = '\0';

    printk("%s", cpuline);

    // Parse the proc/stat information into seperate jiffie containers

    //char *tokbuf = strtok(cpuline, " ");
    //char *tokbuf = strsep(cpuline, " ");
    //user_jif = atoi(tokbuf);
    //tokbuf = strtok(NULL, " ");
    //nice_jif = atoi(tokbuf);
    //tokbuf = strtok(NULL, " ");
    /*system_jif = atoi(tokbuf);
    tokbuf = strtok(NULL, " ");
    idle_jif = atoi(tokbuf);
    tokbuf = strtok(NULL, " ");
    iowait_jif = atoi(tokbuf);*/
    base = 10;
    error = kstrtol(strsep(&cpuline, " "), base, &user_jif);
    error = kstrtol(strsep(&cpuline, " "), base, &nice_jif);
    error = kstrtol(strsep(&cpuline, " "), base, &system_jif);
    error = kstrtol(strsep(&cpuline, " "), base, &idle_jif);
    error = kstrtol(strsep(&cpuline, " "), base, &iowait_jif);
    //printf("\n%d", user_jif);
    //printf("\n%d", nice_jif);
    //printf("\n%d", system_jif);
    //printf("\n%d", idle_jif);
    //printf("\n%d", iowait_jif);

    // Get the workload from that
    tjif = user_jif + nice_jif + system_jif + idle_jif + iowait_jif;
    //printf("\n%d", tjif);

    delta_total = tjif - previous_tjif;
    //printf("\n%d", delta_total);
    
    delta_idle = idle_jif - previous_ijif;
    //printf("\n%d", delta_idle);
    
    delta_usage = (1000*(delta_total-delta_idle)/(delta_total+5))/10;
    //printf("\n%d\n", delta_usage);
    
    previous_tjif = tjif;
    previous_ijif = idle_jif;

    kfree(cpuline);
    kfree(buf);

    return delta_usage;
}

void update(void)
{
    // Disable all LED's

    //for(int i=0; i < number_leds; i++)
    //    digitalWrite(i, 0);

    float cpu_usage = get_cpu_usage();
    printk("cpu_usage: %f\n", cpu_usage);
    leds_lit = cpu_usage/(100/(number_leds));
    printk("leds_lit: %d\n", leds_lit);
    //for(int i=0; i < leds_lit; i++)
    //    digitalWrite(i, 1);
}

module_init(pix_init);
module_exit(pix_exit);
