/*
	This file is part of the linuxapi project.
	Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include <linux/module.h> /* for MODULE_LICENSE(), MODULE_AUTHOR(),
			MODULE_DESCRIPTION(), module_init(),
			module_exit() */
#include <linux/slab.h> /* for kmalloc(), kfree() */
#include "connect.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Module written in C++");

static int __init link_init(void)
{
	return cpp_init();
}

static void __exit link_exit(void)
{
	cpp_exit();
}

void myprintk(const char *msg)
{
	printk(msg);
}

void *mymalloc(unsigned int size)
{
	return kmalloc(size, GFP_KERNEL);
}

void myfree(void *pointer)
{
	kfree(pointer);
}

module_init(link_init);
module_exit(link_exit);
