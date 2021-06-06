#include "lib.h"
#include <mmu.h>
#include <env.h>
#define debug 0

static int pipeclose(struct Fd*);
static int piperead(struct Fd *fd, void *buf, u_int n, u_int offset);
static int pipestat(struct Fd*, struct Stat*);
static int pipewrite(struct Fd *fd, const void *buf, u_int n, u_int offset);

struct Dev devpipe =
{
.dev_id=	'p',
.dev_name=	"pipe",
.dev_read=	piperead,
.dev_write=	pipewrite,
.dev_close=	pipeclose,
.dev_stat=	pipestat,
};

#define BY2PIPE 32		// small to provoke races

struct Pipe {
	u_int p_rpos;		// read position
	u_int p_wpos;		// write position
	u_char p_buf[BY2PIPE];	// data buffer
};

int
pipe(int pfd[2])
{
	int r, va;
	struct Fd *fd0, *fd1;

	// allocate the file descriptor table entries
	if ((r = fd_alloc(&fd0)) < 0
	||  (r = syscall_mem_alloc(0, (u_int)fd0, PTE_V|PTE_R|PTE_LIBRARY)) < 0)
		goto err;

	if ((r = fd_alloc(&fd1)) < 0
	||  (r = syscall_mem_alloc(0, (u_int)fd1, PTE_V|PTE_R|PTE_LIBRARY)) < 0)
		goto err1;

	// allocate the pipe structure as first data page in both
	va = fd2data(fd0);
	if ((r = syscall_mem_alloc(0, va, PTE_V|PTE_R|PTE_LIBRARY)) < 0)
		goto err2;
	if ((r = syscall_mem_map(0, va, 0, fd2data(fd1), PTE_V|PTE_R|PTE_LIBRARY)) < 0)
		goto err3;

	// set up fd structures
	fd0->fd_dev_id = devpipe.dev_id;
	fd0->fd_omode = O_RDONLY;

	fd1->fd_dev_id = devpipe.dev_id;
	fd1->fd_omode = O_WRONLY;

	writef("[%08x] pipecreate \n", env->env_id, (* vpt)[VPN(va)]);

	pfd[0] = fd2num(fd0);
	pfd[1] = fd2num(fd1);
	return 0;

err3:	syscall_mem_unmap(0, va);
err2:	syscall_mem_unmap(0, (u_int)fd1);
err1:	syscall_mem_unmap(0, (u_int)fd0);
err:	return r;
}

static int
_pipeisclosed(struct Fd *fd, struct Pipe *p)
{
	// Your code here.
	// 
	// Check pageref(fd) and pageref(p),
	// returning 1 if they're the same, 0 otherwise.
	// 
	// The logic here is that pageref(p) is the total
	// number of readers *and* writers, whereas pageref(fd)
	// is the number of file descriptors like fd (readers if fd is
	// a reader, writers if fd is a writer).
	// 
	// If the number of file descriptors like fd is equal
	// to the total number of readers and writers, then
	// everybody left is what fd is.  So the other end of
	// the pipe is closed.
	int pfd,pfp,runs;

	do {
		runs = env->env_runs;
		pfd = pageref(fd);
		pfp = pageref(p);
	} while (runs != env->env_runs);

	if (pfd == pfp) {
		// writef("pfd is %d, pfp is %d\n", pfd, pfp);
		return 1;
	} else {
		return 0;
	}
	user_panic("_pipeisclosed reaches end");
}

int
pipeisclosed(int fdnum)
{
	struct Fd *fd;
	struct Pipe *p;
	int r;

	if ((r = fd_lookup(fdnum, &fd)) < 0)
		return r;
	p = (struct Pipe*)fd2data(fd);
	return _pipeisclosed(fd, p);
}

static int
piperead(struct Fd *fd, void *vbuf, u_int n, u_int offset)
{
	// Your code here.  See the lab text for a description of
	// what piperead needs to do.  Write a loop that 
	// transfers one byte at a time.  If you decide you need
	// to yield (because the pipe is empty), only yield if
	// you have not yet copied any bytes.  (If you have copied
	// some bytes, return what you have instead of yielding.)
	// If the pipe is empty and closed and you didn't copy any data out, return 0.
	// Use _pipeisclosed to check whether the pipe is closed.
	int i = 0;
	struct Pipe *p;
	char *rbuf = vbuf;

	p = (struct Pipe *)fd2data(fd);
	while (1) {
		if (_pipeisclosed(fd, p)) {
			return i;
		}
		if (p->p_rpos == p->p_wpos) {
			if (i == 0) {
				syscall_yield();
			} else {
				return i;
			}
		}
		while (p->p_rpos < p->p_wpos) {
			if (i >= n) {
				return i;
			}
			rbuf[i] = p->p_buf[p->p_rpos % BY2PIPE];
			i++;
			p->p_rpos++;
		}
	}

	user_panic("piperead reaches end");
	return -E_INVAL;
}

static int
pipewrite(struct Fd *fd, const void *vbuf, u_int n, u_int offset)
{
	// Your code here.  See the lab text for a description of what 
	// pipewrite needs to do.  Write a loop that transfers one byte
	// at a time.  Unlike in read, it is not okay to write only some
	// of the data.  If the pipe fills and you've only copied some of
	// the data, wait for the pipe to empty and then keep copying.
	// If the pipe is full and closed, return 0.
	// Use _pipeisclosed to check whether the pipe is closed.
	int i;
	struct Pipe *p;
	char *wbuf = vbuf;

	p = (struct Pipe *)fd2data(fd);
	for (i = 0; i < n; i++) {
		while (p->p_wpos == p->p_rpos + BY2PIPE) {
			if (_pipeisclosed(fd, p)) {
				writef("writing on a closed pipe\n");
				return 0;
			}
			syscall_yield();
		}
		p->p_buf[p->p_wpos % BY2PIPE] = wbuf[i];
		p->p_wpos++;
	}
	
	return n;
	// user_panic("pipewrite not implemented");
}

static int
pipestat(struct Fd *fd, struct Stat *stat)
{
	struct Pipe *p;

	

}

static int
pipeclose(struct Fd *fd)
{
	syscall_mem_unmap(0, fd);
	syscall_mem_unmap(0, fd2data(fd));
	return 0;
}

