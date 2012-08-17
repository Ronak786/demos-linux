/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

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

//#define DEBUG
#include<linux/module.h> // for MODULE_*
#include<linux/fs.h> // for fops
#include<linux/device.h> // for struct device
#include<linux/spinlock.h> // for the spin lock API
#include<linux/slab.h> // for the kmalloc API

#include"shared.h" // for the ioctl numbers

//#define DO_DEBUG
#include"kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Showing how to use spin locks in the kernel");

/*
* For a full description of which types of spin locks to use where please refere
* to Rusty Russel's "Unreliable Guide to Kernel Locking" which comes with the kernel
* documentation within the kernel sources or here:
* http://www.kernel.org/pub/linux/kernel/people/rusty/kernel-locking/index.html
*/

// static data
static struct device* my_device;
DEFINE_SPINLOCK(mr_lock); // unlocked by default...
static spinlock_t *lock_t;

// fops

/*
* This is the ioctl implementation.
*/
static long kern_unlocked_ioctll(struct file *filp, unsigned int cmd, unsigned long arg) {
	unsigned long flags;
	PR_DEBUG("start");
	switch (cmd) {
		case IOCTL_SPINLOCK_IRQSAVE:
			// lock - this will disable interrupts on the local CPU Only!!!
			spin_lock_irqsave(&mr_lock, flags);
			/* critical section ... */
			spin_unlock_irqrestore(&mr_lock, flags);
			return 0;
		case IOCTL_SPINLOCK_REGULAR:
			// lock - this will not disable interrupts and may cause a dead lock if you
			// try to acquire the same lock from an interrupt handler or higher level task
			spin_lock(&mr_lock);
			spin_unlock(&mr_lock);
			return 0;
		case IOCTL_SPINLOCK_ALLOCATED:
			lock_t=kmalloc(sizeof(spinlock_t), GFP_KERNEL);
			spin_lock_init(lock_t);
			spin_lock(lock_t);
			spin_unlock(lock_t);
			kfree(lock_t);
			return 0;
	}
	return(-EINVAL);
}


/*
* The file operations structure.
*/
static struct file_operations my_fops={
	.owner=THIS_MODULE,
	.unlocked_ioctl=kern_unlocked_ioctll,
};

#include"device.inc"
