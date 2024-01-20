#ifndef _FLOPPY_H
#define _FLOPPY_H

#include "cubebox.h"

#define FLOPPY_ERROR_SEEK           501
#define FLOPPY_ERROR_DIR            502
#define FLOPPY_ERROR_CYL_END        503
#define FLOPPY_ERROR_NOT_READY      504
#define FLOPPY_ERROR_CRC            505
#define FLOPPY_ERROR_TIMEOUT        506
#define FLOPPY_ERROR_NO_DATA        507
#define FLOPPY_ERROR_NO_MARK        508
#define FLOPPY_ERROR_DEL_MARK       509
#define FLOPPY_ERROR_DATA_CRC       510
#define FLOPPY_ERROR_CYL            511
#define FLOPPY_ERROR_NO_PD765       512
#define FLOPPY_ERROR_SECT           513
#define FLOPPY_ERROR_READ_ONLY      514
#define FLOPPY_FATAL_TIMEOUT        601
#define FLOPPY_FATAL_CALIBRATE      602
#define FLOPPY_FATAL_DMA_BUFF       603

//  --- FLOPPY DRIVE COMMANDS ---
#define FLOPPY_CMD_SPECIFY			3
#define FLOPPY_CMD_WRITE_DATA		5
#define FLOPPY_CMD_READ_DATA		6
#define FLOPPY_CMD_RECALIBRATE		7
#define FLOPPY_CMD_SENSE_INTERRUPT	8
#define FLOPPY_CMD_SEEK				15

/*
    --- FLOPPY REGISTERS ---

    The MSR byte: [read-only]
    -------------
        7   6   5    4    3    2    1    0
        MRQ DIO NDMA BUSY ACTD ACTC ACTB ACTA

        MRQ is 1 when FIFO is ready (test before read/write)
        DIO tells if controller expects write (1) or read (0)

        NDMA tells if controller is in DMA mode (1 = no-DMA, 0 = DMA)
        BUSY tells if controller is executing a command (1=busy)

        ACTA, ACTB, ACTC, ACTD tell which drives position/calibrate (1=yes)

    The DOR byte: [write-only]
    -------------
         7    6    5    4    3   2    1   0
        MOTD MOTC MOTB MOTA DMA NRST DR1 DR0

        DR1 and DR0 together select "current drive" = a/00, b/01, c/10, d/11
        MOTA, MOTB, MOTC, MOTD control motors for the four drives (1=on)

        DMA line enables (1 = enable) interrupts and DMA
        NRST is "not reset" so controller is enabled when it's 1
*/
#define FLOPPY_REG_DOR				2
#define FLOPPY_REG_MSR				4
#define FLOPPY_REG_FIFO				5
#define FLOPPY_REG_CCR				7

#define FLOPPY_MOTOR_ON             1
#define FLOPPY_MOTOR_OFF            0
#define FLOPPY_MOTOR_WAIT           2

#define FLOPPY_DMA_LEN              0x4800

extern static char* _floppy0;
extern static char* _floppy1;
extern static volatile int _floppy_motor_ticks = 0;
extern static volatile int _floppy_motor_state = 0;
extern static const char floppy_dmaBuffer[FLOPPY_DMA_LEN];

extern static const char* floppy_driveTypes[8];

extern typedef enum floppy_tdir;

void floppy_printList();
void floppy_detect();

void floppy_sendCmd(int base, uint8_t cmd);
uint8_t floppy_readData(int base);

void floppy_checkInterrupt(int base, int* st0, int* cylinder);

void floppy_motor(int base, bool on);
void floppy_kill(int base);
void floppy_timer();

void floppy_calibrate(int base);
int floppy_reset(int base);

int floppy_seek(int base, unsigned cylinder, int head);

void floppy_dma_init(floppy_tdir direction);

int floppy_doTrack(int base, unsigned cylinder, floppy_tdir direction);
int floppy_readTrack(int base, unsigned cylinder);
int floppy_writeTrack(int base, unsigned cylinder);

#endif
