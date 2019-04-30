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
int num_leds;
int i;
int leds[10];

static int __init pix_init(void){
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
  //gpio_set_value(leds[1], true);

  gpio_request(leds[2], "L2");
  gpio_direction_output(leds[2], true);
  //gpio_set_value(leds[2], true);

  gpio_request(leds[3], "L3");
  gpio_direction_output(leds[3], true);
  //gpio_set_value(leds[3], true);

  gpio_request(leds[4], "L4");
  gpio_direction_output(leds[4], true);
  //gpio_set_value(leds[4], true);

  gpio_request(leds[5], "L5");
  gpio_direction_output(leds[5], true);
  //gpio_set_value(leds[5], true);

  gpio_request(leds[6], "L6");
  gpio_direction_output(leds[6], true);
  //gpio_set_value(leds[6], true);

  gpio_request(leds[7], "L7");
  gpio_direction_output(leds[7], true);
  //gpio_set_value(leds[7], true);

  gpio_request(leds[8], "L8");
  gpio_direction_output(leds[8], true);
  //gpio_set_value(leds[8], true);
  
  //gpio_request(leds[9], "L9");
  //gpio_direction_output(leds[9], true);

  printk(KERN_INFO "Finished Initalization");
  num_leds = 8;
  for(i = 1; i < num_leds + 1; i++){
    gpio_set_value(leds[i], true);
  }
  for(i = num_leds + 1; i < 10; i ++){
    gpio_set_value(leds[i], false);
  }
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

  printk(KERN_INFO "Finished removing");
}

module_init(pix_init);
module_exit(pix_exit);
