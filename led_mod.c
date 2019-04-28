// Michael Omori, David Wang
// ICS 612 Project 5
// LED LKM for Raspberry Pi 3 B+

#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>

#define L1 18
#define L2 23
#define L3 27
#define L4 16
#define L5 17
#define L6 26
#define L7 13
#define L8 22

MODULE_LICENSE("GPL");

static int __init pix_init(void){
  printk(KERN_INFO "Initialized");

  gpio_request(L1, "L1");
  gpio_direction_output(L1, true);
  gpio_set_value(L1, true);

  gpio_request(L2, "L2");
  gpio_direction_output(L2, true);
  gpio_set_value(L2, true);

  gpio_request(L3, "L3");
  gpio_direction_output(L3, true);
  gpio_set_value(L3, true);

  gpio_request(L4, "L4");
  gpio_direction_output(L4, true);
  gpio_set_value(L4, true);

  gpio_request(L5, "L5");
  gpio_direction_output(L5, true);
  gpio_set_value(L5, true);

  gpio_request(L6, "L6");
  gpio_direction_output(L6, true);
  gpio_set_value(L6, true);

  gpio_request(L7, "L7");
  gpio_direction_output(L7, true);
  gpio_set_value(L7, true);

  gpio_request(L8, "L8");
  gpio_direction_output(L8, true);
  gpio_set_value(L8, true);

  printk(KERN_INFO "Finished Initalization");
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

module_init(pix_init);
module_exit(pix_exit);
