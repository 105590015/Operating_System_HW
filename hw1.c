#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

typedef struct _birthday
{
  int day;
  int month;
  int year;
  struct list_head list;
}birthday;

LIST_HEAD(birthday_list);

void CreateBirthday(int day, int month, int year)
{
  birthday *person;
  person = kmalloc(sizeof(*person), GFP_KERNEL);
  person->day = day;
  person->month = month;
  person->year = year;
  INIT_LIST_HEAD(&person->list);
  list_add_tail(&person->list, &birthday_list);
}

int hw1_init(void)
{
  int time = 1;
  printk(KERN_INFO "Loading hw1 Module\n");
  CreateBirthday(9, 11, 1997);
  CreateBirthday(6, 7, 1998);
  CreateBirthday(29, 2, 1996);
  CreateBirthday(10, 5, 1995);
  CreateBirthday(25, 12, 1994);
  birthday *ptr;
  list_for_each_entry(ptr, &birthday_list, list)
  {
    printk(KERN_INFO "Person%d's birthday is %d/%d/%d\n", time++, ptr->day, ptr->month, ptr->year);
  }
  return 0;
}

void hw1_exit(void)
{
  int time = 1;
  birthday *ptr, *next;
  list_for_each_entry_safe(ptr, next, &birthday_list, list)
  {
    list_del(&ptr->list);
    printk(KERN_INFO "Person%d's birthday has been removed from birthday_list\n", time);
    kfree(ptr);
    printk(KERN_INFO "Memory of person%d's birthday have been freed", time++);
  }
  printk(KERN_INFO "Removing hw1 Module\n");
}

module_init(hw1_init);
module_exit(hw1_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
