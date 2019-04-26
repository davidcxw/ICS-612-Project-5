#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>

#define A1 18 // 0

MODULE_LICENSE("GPL");

static int __init pix_init(void){
  printk(KERN_INFO "PIX: starting...");
  // stuff to do
  gpio_request(A1, "A1");
  gpio_direction_output(A1, true);
  gpio_set_value(A1, true);
  printk(KERN_INFO "PIX: staring done.");
  return 0;
}

static void __exit pix_exit(void){
  printk(KERN_INFO "PIX: stopping...");
  // stuff to do
  gpio_free(A1);
  printk(KERN_INFO "PIX: stopping done.");
}

module_init(pix_init);
module_exit(pix_exit);
