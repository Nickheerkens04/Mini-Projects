#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>


unsigned int hello1(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
	printk(KERN_INFO "***Hello1***\n");
	return NF_ACCEPT;

}

unsigned int hello2(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
	printk(KERN_INFO "***Hell02 ***\n");
	return NF_ACCEPT;
}


static struct nf_hook_ops hook1, hook2;

int registerFilter(void) {
	printk(KERN_INFO "Registering filters...\n");

	hook1.hook = hello1;
	hook1.hooknum = NF_INET_LOCAL_OUT;
	hook1.pf = PF_INET;
	hook1.priority = -50;
	nf_register_net_hook(&init_net, &hook1);

	hook2.hook = hello2;
	hook2.hooknum = NF_INET_LOCAL_OUT;
	hook2.pf = PF_INET;
	hook2.priority = -200;
	nf_register_net_hook(&init_net, &hook2);

	return 0;
}
 void removeFilter(void) {
	printk(KERN_INFO "Removing Filters...\n");
	nf_unregister_net_hook(&init_net, &hook1);
	nf_unregister_net_hook(&init_net, &hook2);
}

module_init(registerFilter);
module_exit(removeFilter);
MODULE_LICENSE("GPL");
