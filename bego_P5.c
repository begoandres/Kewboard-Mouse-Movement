#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/keyboard.h>
#include <asm/io.h>

struct input_dev *bego;
size_t tcount = 0;
struct notifier_block nb;
int keymap[5] = {103,108,106,105,28};



ssize_t move_mouse(int i, int kp)
{

	int n=10;
	switch(keymap[i])
	{
		case 108:
			input_report_rel(bego, REL_Y, n);
			break;
		case 103:
			input_report_rel(bego, REL_Y, -n);
			break;
		case 106:
			input_report_rel(bego, REL_X, n);
			break;
		case 105:
			input_report_rel(bego, REL_X, -n);
			break;
		case 28:
			input_report_key(bego, BTN_LEFT, kp);
			break;
	}
	input_sync(bego);
	return i;	
}



int kb_notifier_fn(struct notifier_block *nb, unsigned long action, void* data)
{
	struct keyboard_notifier_param *kp = (struct keyboard_notifier_param*)data;
	int i;

	for(i = 0; i < 5; i++)
	{
		if(keymap[i] == kp->value)
		{		
			move_mouse(i, kp ->down);
		}
	}

	return 0;
}

int init (void) 
{

	bego = input_allocate_device();

	bego->name = "bego";

	set_bit(EV_REL, bego->evbit);
	set_bit(REL_X, bego->relbit);
	set_bit(REL_Y, bego->relbit);
	set_bit(EV_KEY, bego->evbit);
	set_bit(BTN_LEFT, bego->keybit);
	nb.notifier_call = kb_notifier_fn;
	register_keyboard_notifier(&nb);
	
	input_register_device(bego);

	return 0;
}

void cleanup(void) 
{
	input_unregister_device(bego);
	unregister_keyboard_notifier(&nb);
}

MODULE_LICENSE("GPL"); 
module_init(init);
module_exit(cleanup);
