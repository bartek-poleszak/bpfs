BPFS
=========

A simple filesystem written as a part of my BSc thesis.
It is written in C++ and uses FUSE (Filesystem in Userspace) to write directly to /dev/sdX block devices.

Its most interesting feature is ability to create a partition distributed over many devices (something like RAID 0, Its called it _matrix_ in the code). In this mode data on the device _n_ is encrypted with key stored on the device _n-1_. So if you've got a secret document which can be opened only if a group of people gathers in one place, you have to create BPFS matrix on few flashdrives and give each person one.

It lacks features like access control or directories, but hey, simplicity was the goal here! ;)

You can find the complete description (unfortunately only in Polish) in _thesis.pdf_.

Warning
-------

Applications in this project usualy need root privilages and it's quite easy to purge your hard drive with them - use at your own risk!
Using virtual environment is highly recommended.

Modules
-------

Project is divided into four modules:

* __core__ - main filesystem logic
* __file-access__ - a small library providing access to block devices
* __manager__ - an application used to format a partition into BPFS
* __fuse-wrapper__ - an application which uses FUSE to mount a partition

### manager

To run manager you have to call
```
./manager /dev/<partition>
```

Than you will be able to use `mkfs` command to format the chosen patition. You will have to provide:

* block size
* inode size
* number of inodes

To create partition matrix use command `mkmatrix`. You will have to enter:

* number of device to use
* paths to those devices
* block size
* inode size
* number of inodes