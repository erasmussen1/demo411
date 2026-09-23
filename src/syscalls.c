
#include "syscalls.h"

#include <errno.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * Linker symbols.
 *
 * These names depend on your linker script.
 * Common STM32 linker scripts define _end or end as the start of heap,
 * and _estack as the top of RAM.
 */
extern char _end;
extern char _estack;

/*
 * Optional: provide this function somewhere in your UART driver.
 *
 * Example:
 *
 * void uart_putc(char c)
 * {
 *     while (!(USART2->SR & USART_SR_TXE)) {
 *     }
 *     USART2->DR = (uint8_t)c;
 * }
 */
extern void uart_putc(uint8_t c);
extern uint8_t uart_getc(void);

/* ------------------------------------------------------------------------- */
/* stdout / stderr                                                           */
/* ------------------------------------------------------------------------- */

int _write(int file, char* ptr, int len) {
    if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
        errno = EBADF;
        return -1;
    }

    for (int i = 0; i < len; ++i) {
        /*
         * Optional CRLF conversion for terminals.
         *
        if (ptr[i] == '\n') {
            uart_putc('\r');
        }
        */

        uart_putc(ptr[i]);
    }

    return len;
}


/* ------------------------------------------------------------------------- */
/* stdin                                                                     */
/* ------------------------------------------------------------------------- */

int _read(int file, char* ptr, int len) {
    if (file != STDIN_FILENO) {
        errno = EBADF;
        return -1;
    }

    if (len < 0) {
        errno = EINVAL;
        return -1;
    }

    if (len == 0) {
        return 0;
    }

    if (ptr == NULL) {
        errno = EFAULT;
        return -1;
    }

    /*
     * This is an interactive character device, so return as soon as one byte
     * is available.  Filling len bytes here could make getchar() wait for a
     * complete Newlib input buffer instead of a single key press.
     */
    ptr[0] = (char)uart_getc();
    return 1;
}


/* ------------------------------------------------------------------------- */
/* File-like stubs used by Newlib                                            */
/* ------------------------------------------------------------------------- */

int _close(int file) {
    (void)file;

    errno = EBADF;
    return -1;
}


int _fstat(int file, struct stat* st) {
    if (st == NULL) {
        errno = EFAULT;
        return -1;
    }

    /*
     * Treat stdin/stdout/stderr as character devices.
     */
    if ((file == STDIN_FILENO) || (file == STDOUT_FILENO) || (file == STDERR_FILENO)) {

        st->st_mode = S_IFCHR;
        return 0;
    }

    errno = EBADF;
    return -1;
}

int _isatty(int file) {
    if ((file == STDIN_FILENO) || (file == STDOUT_FILENO) || (file == STDERR_FILENO)) {
        return 1;
    }

    errno = EBADF;
    return 0;
}

off_t _lseek(int file, off_t offset, int whence) {
    (void)file;
    (void)offset;
    (void)whence;

    errno = ESPIPE;
    return (off_t)-1;
}


/* ------------------------------------------------------------------------- */
/* Process-related stubs                                                     */
/* ------------------------------------------------------------------------- */

int _getpid(void) {
    return 1;
}

int _kill(int pid, int sig) {
    (void)pid;
    (void)sig;

    errno = EINVAL;
    return -1;
}

void _exit(int status) {
    (void)status;

    for (;;) {
        /*
         * Bare-metal program has nowhere to exit to.
         */
    }
}


/* ------------------------------------------------------------------------- */
/* Heap support for malloc/new                                               */
/* ------------------------------------------------------------------------- */

void* _sbrk(ptrdiff_t increment) {
    static char* heap_end = NULL;

    if (heap_end == NULL) {
        heap_end = &_end;
    }

    char* previous_heap_end = heap_end;
    char* new_heap_end = heap_end + increment;

    /*
     * This is deliberately conservative.
     *
     * _estack is normally the top of SRAM, but the active stack grows
     * downward from there. Using _estack directly does NOT reserve space
     * for the stack.
     *
     * Prefer using a dedicated heap limit symbol from your linker script
     * if one exists.
     */
    if (new_heap_end >= &_estack) {
        errno = ENOMEM;
        return (void*)-1;
    }

    heap_end = new_heap_end;

    return previous_heap_end;
}
