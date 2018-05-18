
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int ID =0;
static int class = 0;

int __init init_my_module(void){
	printk("Hello module! My classId is %d, studentId is %d\n",class,ID);
	return 0;
}

void __exit exit_my_module(void){
	printk("Bye!\n");
}
module_param(ID,int,0);
module_param(class,int,0);
module_init(init_my_module);
module_exit(exit_my_module);
MODULE_LICENSE("GPL");

