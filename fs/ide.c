/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurred during read the IDE disk, panic. 
// 	
// Hint: use syscalls to access device registers and buffers
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;

	int tmp;

	while (offset_begin + offset < offset_end) {
		// Your code here
		// error occurred, then panic.
		tmp = 0;
		if (syscall_write_dev(&tmp, 0x13000010, 4)) user_panic("error in ide_read()\n");
		tmp = offset_begin + offset;
		if (syscall_write_dev(&tmp, 0x13000000, 4)) user_panic("error in ide_read()\n");
		tmp = 0;
		if (syscall_write_dev(&tmp, 0x13000020, 4)) user_panic("error in ide_read()\n");
		if (syscall_read_dev(&tmp, 0x13000030, 4)) user_panic("error in ide_read()\n");
		if (tmp == 0) user_panic("error in ide_read()");
		if (syscall_read_dev(dst + offset, 0x13004000, 512)) user_panic("error in ide_read()\n");
		offset += 0x200;
	}
}


// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurred during read the IDE disk, panic.
//	
// Hint: use syscalls to access device registers and buffers
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
	// Your code here
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;

	// DO NOT DELETE WRITEF !!!
	writef("diskno: %d\n", diskno);

	int tmp;

	while (offset_begin + offset < offset_end) {
		// Your code here
	    // copy data from source array to disk buffer.
		// error occurred, then panic.
		if (syscall_write_dev(src + offset, 0x13004000, 512)) user_panic("error in ide_write()");
		tmp = 0;
		if (syscall_write_dev(&tmp, 0x13000010, 4)) user_panic("error in ide_write()");
		tmp = offset_begin + offset;
		if (syscall_write_dev(&tmp, 0x13000000, 4)) user_panic("error in ide_write()");
		tmp = 1;
		if (syscall_write_dev(&tmp, 0x13000020, 4)) user_panic("error in ide_write()");
		if (syscall_read_dev(&tmp, 0x13000030, 4)) user_panic("error in ide_write()");
		if (tmp == 0) user_panic("error in ide_write()");
		offset += 0x200;
	}
}

