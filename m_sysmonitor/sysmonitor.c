/*
 * Name:
 *      System Monitor Module
 * Description:
 *      A system call module that dumps system status periodically.
 * Usage:
 *      TODO: Add usage here
 * Author:
 *      Hai Lang
 * Date Created:
 *      2013-02-15
 * Last Update:
 *      2013-02-15
 *
*/
#include <sys/types.h>
#include <sys/param.h>
#include <sys/proc.h>
#include <sys/module.h>
#include <sys/sysent.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/sysproto.h>
//#include <sys/time.h> //fetch

/* The system call's arguments */
struct sysmonitor_args {
    int target;
    //char *target;
};

/* The system call function */
static int sysmonitor(struct thread *td, void *syscall_args)
{
    struct sysmonitor_args *args; /* local struct to receive syscall_args */
    args = (struct sysmonitor_args *)syscall_args; /* receive syscall_args with casting */

    /* Copy args to kernel space */
    int target = args->target;
    printf("target: %p\n", &target);
    printf("args target: %p\n", &args->target);

    switch(target) {
        case 0:
            printf("Target 0 Received!\n");
            break;
        case 1:
            printf("Target 1 Received!\n");
            break;
        default:
            printf("Unsupported Target!\n");
            break;
    }

    return (0);
}

/* Prepare sysent to register the new system call */
static struct sysent sysmonitor_sysent = {
    1,  /* Number of arguments */
    sysmonitor /* implementing function */
};

/* Define the offset in sysent[] where the new system call is to be allocated */
static int offset = NO_SYSCALL; /* Default, using the next available slots offset in sysent table */

/* Event handler function for the new system call */
static int load(struct module *module, int cmd, void *arg)
{
    int error = 0;

    switch(cmd) {
        case MOD_LOAD:
            uprintf("System call loaded at offset %d.\n", offset);
            break;
        case MOD_UNLOAD:
            uprintf("System call unloaded from offset %d.\n", offset);
            break;
        default:
            error = EOPNOTSUPP; /* Operation not supported */
            break;
    }

    return(error);
}

/* Declare and register the system call module */
SYSCALL_MODULE(sysmonitor, &offset, &sysmonitor_sysent, load, NULL);
