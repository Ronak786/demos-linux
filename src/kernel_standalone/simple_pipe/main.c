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
#include <linux/module.h> // for MODULE_*, module_*
#include <linux/moduleparam.h> // for module_param, MODULE_PARM_DESC
#include <linux/fs.h> // for fops
#include <linux/device.h> // for class_create
#include <linux/slab.h> // for kzalloc
#include <linux/uaccess.h> // for copy_to_user, access_ok
#include <linux/cdev.h> // for cdev_*
#include <linux/sched.h> // for TASK_INTERRUPTIBLE and more constants
#include <linux/spinlock.h> // for spinlock_t and ops on it
#include <linux/wait.h> // for wait_queue_head_t and ops on it

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("A named pipe exercise");

#define DO_COPY

/*
* See exercise.txt for the description of this exercise.
*
* Notes:
* - we protect the pipe here using a spinlock. Maybe a mutex would give better results
* on a single CPU. Test it out.
* - Even though the pipe size is PAGE_SIZE (4096 on a i386) we can only store PAGE_SIZE-1
* bytes in the pipe. This is because if we stored 4096 bytes in the pipe we would not
* be able to distinguish between a full pipe and an empty one. This is the reason for the
* weird -1 in the 'pipe_room' function.
* - the performace of this pipe as can be ascertained using the 'pipemeter' is much
* lower than the kernels own pipe as can be checked
* via 'cat /dev/zero | pipemeter > /dev/null' (I prepared a script for this).
*
* TODO:
* - explain the difference in performance mentioned above and improve this example to
* give similar performance.
* - hold the readers and writers as atomic variables.
* This will enable us to remove the spinlock protection that we currently have in the
* 'open' and 'release' fops.
*
* TOOLS: pipemeter
*/

static int pipes_count=8;
module_param(pipes_count, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(pipes_count, "How many pipes to create ?");
// this size of pipe performs very well! (3.0 G/s)
//static int pipe_size=PAGE_SIZE*10;
// this one (PAGE_SIZE) doesnt...
static int pipe_size=PAGE_SIZE;
module_param(pipe_size, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(pipe_size, "What is the pipe size ?");

// struct for each pipe
typedef struct _my_pipe_t {
	char* data;
	size_t size;
	size_t read_pos;
	size_t write_pos;
} my_pipe_t;

// these are all out pipes
static my_pipe_t* pipes=NULL;
// this variable will store the class
static struct class *my_class;
// this variable will hold our cdev struct
static struct cdev cdev;
// this is the first dev_t allocated to us...
static dev_t first_dev;

// pipe constructur
static inline int pipe_ctor(my_pipe_t* pipe) {
	pipe->data=kmalloc(pipe_size,GFP_KERNEL);
	if(pipe->data==NULL) {
		return -EFAULT;
	}
	pipe->size=pipe_size;
	pipe->read_pos=0;
	pipe->write_pos=0;
	return 0;
}

// pipe destructor
static inline void pipe_dtor(const my_pipe_t* pipe) {
	kfree(pipe->data);
}

// do we have data in the pipe ?
static inline bool pipe_have_data(const my_pipe_t* pipe) {
	return pipe->read_pos!=pipe->write_pos;
}

// return the empty room of a pipe
static inline size_t pipe_room(const my_pipe_t* pipe) {
	if(pipe->read_pos<=pipe->write_pos) {
		return pipe->size-pipe->write_pos+pipe->read_pos-1;
	} else {
		return pipe->read_pos-pipe->write_pos-1;
	}
}
// return the occupied room of a pipe
static inline size_t pipe_data(const my_pipe_t* pipe) {
	if(pipe->read_pos<=pipe->write_pos) {
		return pipe->write_pos-pipe->read_pos;
	} else {
		return pipe->size-pipe->read_pos+pipe->write_pos;
	}
}

// read into the pipe
static inline int pipe_copy_from_user(my_pipe_t* const pipe,int count,const char** __user ubuf) {
	int ret;
	pr_debug("count is %d, read_pos is %d, write_pos is %d, size is %d\n",count,pipe->read_pos,pipe->write_pos,pipe->size);
	#ifdef DO_COPY
	ret=copy_from_user(pipe->data+pipe->write_pos,*ubuf,count);
	#else // DO_COPY
	ret=0;
	#endif // DO_COPY
	if(ret==0) {
		*ubuf+=count;
		pipe->write_pos+=count;
		//BUG_ON(pipe->write_pos>pipe->size);
		if(pipe->write_pos==pipe->size) {
			pipe->write_pos=0;
		}
	}
	return ret;
}

// read from the pipe
static inline int pipe_copy_to_user(my_pipe_t* const pipe,int count,char** __user ubuf) {
	int ret;
	pr_debug("count is %d, read_pos is %d, write_pos is %d, size is %d\n",count,pipe->read_pos,pipe->write_pos,pipe->size);
	#ifdef DO_COPY
	ret=copy_to_user(*ubuf,pipe->data+pipe->read_pos,count);
	#else // DO_COPY
	ret=0;
	#endif // DO_COPY
	if(ret==0) {
		*ubuf+=count;
		pipe->read_pos+=count;
		//BUG_ON(pipe->read_pos>pipe->size);
		if(pipe->read_pos==pipe->size) {
			pipe->read_pos=0;
		}
	}
	return ret;
}

// these are the actual operations

static int pipe_open(struct inode* inode, struct file* filp) {
	// hide the pipe in the private_data of the file_struct
	filp->private_data=pipes+(iminor(inode)-MINOR(first_dev));
	return 0;
}

static ssize_t pipe_read(struct file * file, char __user * buf, size_t count, loff_t *ppos) {
	my_pipe_t* pipe;
	size_t data,work_size,first_chunk,second_chunk,ret;
	pr_debug("start\n");
	if (!access_ok(VERIFY_WRITE, buf, count))
		return -EFAULT;
	pipe=(my_pipe_t*)(file->private_data);
	// lets sleep while there is no data in the pipe
	// why do we not just use the waitqueue condition? because we want to get
	// the pipe LOCKED with data
	data=pipe_data(pipe);
	if(data==0) {
		return 0;
	}
	// now data > 0
	work_size=min(data,count);
	pr_debug("work_size is %d\n",work_size);
	// copy_to_user data from the pipe
	if(pipe->read_pos<=pipe->write_pos) {
		if((ret=pipe_copy_to_user(pipe,work_size,&buf))) {
			return ret;
		}
	} else {
		first_chunk=min(work_size,pipe->size-pipe->read_pos);
		if((ret=pipe_copy_to_user(pipe,first_chunk,&buf))) {
			return ret;
		}
		if(first_chunk<work_size) {
			second_chunk=work_size-first_chunk;
			if((ret=pipe_copy_to_user(pipe,second_chunk,&buf))) {
				return ret;
			}
		}
	}
	*ppos+=work_size;
	return work_size;
}

static ssize_t pipe_write(struct file * file, const char __user * buf, size_t count, loff_t *ppos) {
	my_pipe_t* pipe;
	size_t work_size,room,first_chunk,second_chunk,ret;
	pr_debug("start\n");
	if (!access_ok(VERIFY_READ, buf, count))
		return -EFAULT;
	pipe=(my_pipe_t*)(file->private_data);
	// lets check if we have room in the pipe
	// why do we not just use the waitqueue condition? because we want to get
	// the pipe LOCKED with data
	room=pipe_room(pipe);
	if(room==0) {
		return 0;
	}
	// now room > 0
	work_size=min(room,count);
	pr_debug("work_size is %d\n",work_size);
	// copy_from_user data from the pipe
	if(pipe->read_pos<=pipe->write_pos) {
		first_chunk=min(work_size,pipe->size-pipe->write_pos);
		if((ret=pipe_copy_from_user(pipe,first_chunk,&buf))) {
			return ret;
		}
		if(first_chunk<work_size) {
			second_chunk=work_size-first_chunk;
			if((ret=pipe_copy_from_user(pipe,second_chunk,&buf))) {
				return ret;
			}
		}
	} else {
		if((ret=pipe_copy_from_user(pipe,work_size,&buf))) {
			return ret;
		}
	}
	*ppos+=work_size;
	return work_size;
}

// this is the operations table
static const struct file_operations pipe_fops={
	.owner=THIS_MODULE,
	.open=pipe_open,
	.read=pipe_read,
	.write=pipe_write,
};

static int __init pipe_init(void) {
	int ret;
	int i;
	int j;
	struct device* pipe_device;
	// allocate all pipes
	pipes=(my_pipe_t*)kmalloc(sizeof(my_pipe_t)*pipes_count,GFP_KERNEL);
	if(IS_ERR(pipes)) {
		pr_err("kmalloc\n");
		ret=PTR_ERR(pipes);
		goto err_return;
	}
	// initialize all pipes
	for(i=0;i<pipes_count;i++) {
		ret=pipe_ctor(pipes+i);
		if(ret) {
			for(j=0;j<i;j++) {
				pipe_dtor(pipes+j);
			}
			goto err_pipes;
		}
	}
	// allocate our own range of devices
	ret=alloc_chrdev_region(&first_dev, 0, pipes_count, THIS_MODULE->name);
	if(ret) {
		pr_err("cannot alloc_chrdev_region\n");
		goto err_pipes_ctor;
	}
	pr_debug("allocated the region\n");
	// add the cdev structure
	cdev_init(&cdev, &pipe_fops);
	ret=cdev_add(&cdev, first_dev, pipes_count);
	if(ret) {
		pr_err("cannot cdev_add\n");
		goto err_dealloc;
	}
	pr_debug("added the cdev\n");
	// this is creating a new class (/proc/devices)
	my_class=class_create(THIS_MODULE,THIS_MODULE->name);
	if(IS_ERR(my_class)) {
		pr_err("class_create\n");
		ret=PTR_ERR(my_class);
		goto err_cdev_del;
	}
	pr_debug("created the class\n");
	for(i=0;i<pipes_count;i++) {
		// and now lets auto-create a /dev/ node
		pipe_device=device_create(my_class, NULL, MKDEV(MAJOR(first_dev),MINOR(first_dev)+i),NULL,"%s%d",THIS_MODULE->name,i);
		if(IS_ERR(pipe_device)) {
			pr_err("device_create\n");
			ret=PTR_ERR(pipe_device);
			goto err_class;
		}
	}
	pr_info("%s loaded successfully\n",THIS_MODULE->name);
	return 0;
	/*
err_device:
	for(i=0;i<pipes_count;i++) {
		device_destroy(my_class, MKDEV(MAJOR(first_dev),MINOR(first_dev)+i));
	}
	*/
err_class:
	class_destroy(my_class);
err_cdev_del:
	cdev_del(&cdev);
err_dealloc:
	unregister_chrdev_region(first_dev, pipes_count);
err_pipes_ctor:
	for(i=0;i<pipes_count;i++) {
		pipe_dtor(pipes+i);
	}
err_pipes:
	kfree(pipes);
err_return:
	return ret;
}

static void __exit pipe_exit(void) {
	int i;
	for(i=0;i<pipes_count;i++) {
		device_destroy(my_class,MKDEV(MAJOR(first_dev),MINOR(first_dev)+i));
	}
	class_destroy(my_class);
	cdev_del(&cdev);
	unregister_chrdev_region(first_dev, pipes_count);
	for(i=0;i<pipes_count;i++) {
		pipe_dtor(pipes+i);
	}
	kfree(pipes);
	pr_info("%s unloaded successfully\n",THIS_MODULE->name);
}

module_init(pipe_init);
module_exit(pipe_exit);