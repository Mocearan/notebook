# timer

---



## sleep

### sleep

```c
NAME
       sleep - sleep for the specified number of seconds

SYNOPSIS
       #include <unistd.h>

       unsigned int sleep(unsigned int seconds);
// sleep() makes the calling thread sleep until seconds seconds have elapsed or a signal arrives which is not ignored.

//  Zero if the requested time has elapsed, or the number of seconds left to sleep, if the call was interrupted by a signal handler.
```

> ```c
> do {
>     n = sleep(n);
> } while(n > 0);
> 
> while(n=sleep(n));
> ```
>
> > sleep 指定秒数，即便被中断。

### usleep

```c
NAME
       usleep - suspend execution for microsecond intervals

SYNOPSIS
       #include <unistd.h>

       int usleep(useconds_t usec);

// The  usleep()  function  suspends  execution of the calling thread for (at least) usec microseconds.  The sleep may be lengthened slightly by any system activity or by the time spent processing the call or by the granularity of system timers.

 // The usleep() function returns 0 on success.  On error, -1 is returned, with errno set to indicate the cause of the error.
```

#### ERRORS


           	EINTR  Interrupted by a signal; see signal(7).
    		EINVAL usec is not smaller than 1000000.  (On systems where that is considered an error.)

### nanosleep

```c
PROLOG
       This  manual  page is part of the POSIX Programmer's Manual.  The Linux implementation of this interface may differ (consult the corresponding Linux manual page for details of Linux behavior), or the interface may not be imple‐
       mented on Linux.

NAME
       nanosleep - high resolution sleep (REALTIME)

SYNOPSIS
       #include <time.h>

       int nanosleep(const struct timespec *rqtp, struct timespec *rmtp);

DESCRIPTION
       The nanosleep() function shall cause the current thread to be suspended from execution until either the time interval specified by the rqtp argument has elapsed or a signal is delivered to the calling thread, and its action  is to invoke a signal-catching function or to terminate the process. The suspension time may be longer than requested because the argument value is rounded up to an integer multiple of the sleep resolution or because of the scheduling of other activity by the system. But, except for the case of being interrupted by a signal, the suspension time shall not be less than the time specified by rqtp, as measured by the system clock CLOCK_REALTIME.

       The use of the nanosleep() function has no effect on the action or blockage of any signal.

RETURN VALUE
       If the nanosleep() function returns because the requested time has elapsed, its return value shall be zero.

       If the nanosleep() function returns because it has been interrupted by a signal, it shall return a value of -1 and set errno to indicate the interruption. If the rmtp argument is non-NULL, the timespec structure  referenced  by
       it is updated to contain the amount of time remaining in the interval (the requested time minus the time actually slept). If the rmtp argument is NULL, the remaining time is not returned.

       If nanosleep() fails, it shall return a value of -1 and set errno to indicate the error.


ERRORS
    The nanosleep() function shall fail if:

EINTR  The nanosleep() function was interrupted by a signal.

    EINVAL The rqtp argument specified a nanosecond value less than zero or greater than or equal to 1000 million.

    The following sections are informative.

```



## 时间结构

### time_t

### struct timeval

```c
struct timeval {
    long tv_sec; /* seconds */
    long tv_usec; /* microseconds */
};
```



### struct timespec

```c
struct timespec{
    time_t tv_sec; /* seconds */
    long tv_nsec; /* nanoseconds */
};
```



## timer

### setitimer

```c
NAME
       getitimer, setitimer - get or set value of an interval timer

SYNOPSIS
       #include <sys/time.h>

       int getitimer(int which, struct itimerval *curr_value);
       int setitimer(int which, const struct itimerval *new_value,
                     struct itimerval *old_value);
DESCRIPTION
       The system provides each process with three interval timers, each decrementing in a distinct time domain.  When any timer expires, a signal is sent to the process, and the timer (potentially) restarts.
// which:
//  ITIMER_REAL    decrements in real time, and delivers SIGALRM upon expiration. 
    // 经过指定时间，内核发送SIGALRM信号给本进程
// ITIMER_VIRTUAL  decrements only when the process is executing, and delivers SIGVTALRM upon expiration. 
    // 程序在用户态运行够指定时间后，内核发送SIGVTALRM给本进程
// ITIMER_PROF    decrements  both  when the process executes and when the system is executing on behalf of the process.  SIGPROF is delivered upon expiration.
    // 程序在内核态运行够指定时间后，内核发送SIGPROF给本进程
    Coupled with ITIMER_VIRTUAL, this timer is usually used to profile the time spent by the application in userand kernel space.  
    // 共用ITIMER_VIRTUAL 和 ITIMER_PROF,可以统计在用户和内核空间花费的时间。
    

       Timer values are defined by the following structures:

           struct itimerval {
               struct timeval it_interval; /* next value */
               struct timeval it_value;    /* current value */
           };

           struct timeval {
               time_t      tv_sec;         /* seconds */
               suseconds_t tv_usec;        /* microseconds */
               
       The function getitimer() fills the structure pointed to by curr_value with the current setting for the timer specified by which (one of ITIMER_REAL, ITIMER_VIRTUAL, or ITIMER_PROF).  The element it_value is set to the amount of
       time remaining on the timer, or zero if the timer is disabled.  Similarly, it_interval is set to the reset value.

       The function setitimer() sets the specified timer to the value in new_value.  If old_value is non-NULL, the old value of the timer is stored there.

       Timers decrement from it_value to zero, generate a signal, and reset to it_interval.  A timer which is set to zero (it_value is zero or the timer expires and it_interval is zero) stops.

       Both tv_sec and tv_usec are significant in determining the duration of a timer.

       Timers will never expire before the requested time, but may expire some (short) time afterward, which depends on the system timer resolution and on the system load; see time(7).  (But see BUGS below.)  Upon expiration, a signal
       will be generated and the timer reset.  If the timer expires while the process is active (always true for ITIMER_VIRTUAL) the signal will be delivered immediately when generated.  Otherwise the delivery  will  be  offset  by  a
       small time dependent on the system loading.

RETURN VALUE
       On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.
           
ERRORS
       EFAULT new_value, old_value, or curr_value is not valid a pointer.

       EINVAL which is not one of ITIMER_REAL, ITIMER_VIRTUAL, or ITIMER_PROF; or (since Linux 2.6.22) one of the tv_usec fields in the structure pointed to by new_value contains a value outside the range 0 to 999999.
```



### alarm

​		通过产生一次`SIGALRM`信号来进行定时通知。多次通知需要多次调用`alarm`

