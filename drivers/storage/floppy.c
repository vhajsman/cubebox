#include "floppy.h"

#include "kernel/libs/krnlout.h"
#include "kernel/libs/krnlstatus.h"
#include "kernel/libs/util/delay.h"
#include "kernel/irq/irq.h"

static char* _floppy0;
static char* _floppy1;

static volatile int _floppy_motor_ticks = 0;
static volatile int _floppy_motor_state = 0;

bool _floppy_wait = false;

static const char* floppy_driveTypes[8] = {
    "none",
    "360kB 5.25\"",
    "1.2MB 5.25\"",
    "720kB 3.5\"",

    "1.44MB 3.5\"",
    "2.88MB 3.5\"",
    "unknown type",
    "unknown type"
};



void floppy_printList() {
    print("------------------------------\n");
    print("Floppy drive listing:");

    print("\nDrive 0: ");
    print(floppy_driveTypes[_floppy0]);

    print("\nDrive 1: ");
    print(floppy_driveTypes[_floppy1]);
    
    print("\n------------------------------\n");
}

void floppy_detect() {
    println("Detecting floppy drives...");

    outb(0x70, 0x10);
    unsigned drives = inb(0x71);

    _floppy0 = (drives >> 4);
    _floppy1 = (drives & 0x0f);

    // floppy_printList();
}

void floppy_sendCmd(int base, uint8_t cmd) {
    for (int i = 0; i < 600; i++) {
        SLEEP_TICKS(10);

        if (0x80 & inb(base + FLOPPY_REG_MSR)) {
            outb(base + FLOPPY_REG_FIFO, cmd);
            return;
        }

        printStatusMsg(KRNL_STATUS_FATAL, "FLOPPY", "Response timeout while send command.", FLOPPY_FATAL_TIMEOUT);
    }
}

uint8_t floppy_readData(int base) {
    for (int i = 0; i < 600; i++) {
        SLEEP_TICKS(10);

        if (0x80 & inb(base + FLOPPY_REG_MSR))
            return inb(base + FLOPPY_REG_FIFO);

        printStatusMsg(KRNL_STATUS_FATAL, "FLOPPY", "Response timeout while data read.", FLOPPY_FATAL_TIMEOUT);
        return 0;
    }
}

void floppy_checkInterrupt(int base, int* st0, int* cylinder) {
    floppy_sendCmd(base, FLOPPY_CMD_SENSE_INTERRUPT);

    *st0 =      floppy_readData(base);
    *cylinder = floppy_readData(base);
}



void floppy_motor(int base, bool on) {
    if (on) {
        if (!_floppy_motor_state) {
            outb(base + FLOPPY_REG_DOR, 0x1C);
            DELAY_US(50);
        }

        _floppy_motor_state = FLOPPY_MOTOR_ON;
        return;
    }

    if (_floppy_motor_state == FLOPPY_MOTOR_WAIT)
        println("Floppy: floppy motor strange, already waiting...");

    _floppy_motor_ticks = 300;
    _floppy_motor_state = FLOPPY_MOTOR_WAIT;
}

void floppy_kill(int base) {
    outb(base + FLOPPY_REG_DOR, 0x0C);
    _floppy_motor_state = FLOPPY_MOTOR_OFF;
}

void floppy_timer() {
    while (1) {
        DELAY_US(50);

        if (_floppy_motor_state == FLOPPY_MOTOR_WAIT) {
            _floppy_motor_ticks -= 50;

            if (timer_ticks <= 0)
                floppy_kill(FLOPPY_BASE);
        }
    }
}




void floppy_irqWaitComplete(struct registers* regs) {
    _floppy_wait = false;
}

void floppy_irqWait() {
    debugln("Waiting for floppy IRQ (0x06)...");
    irq_installHandler(IRQ_FD, floppy_irqWaitComplete);

    _floppy_wait = true;
    while (_floppy_wait) {}
}

void floppy_calibrate(int base) {
    int i, st0, cylinder = -1;

    floppy_motor(base, true);

    for (i = 0; i < 10; i++) {
        floppy_sendCmd(base, FLOPPY_CMD_RECALIBRATE);
        floppy_sendCmd(base, 0x00);

        floppy_irqWait();
        floppy_checkInterrupt(base, &st0, &cylinder);

        if (st0 & 0xC0) {
            /*
            if ((st0 >> 6) == 1 || (st0 >> 6) == 2) {
                printStatusMsg(KRNL_STATUS_FATAL, "FLOPPY", "Calibration failure.", FLOPPY_FATAL_CALIBRATE);
            }
            */

            continue;
        }

        if (cylinder == 0) {
            floppy_motor(base, false);
            return 0;
        }
    }

    floppy_motor(base, false);

    printStatusMsg(KRNL_STATUS_FATAL, "FLOPPY", "Calibration failure.", FLOPPY_FATAL_CALIBRATE);
    return 1;
}

int floppy_reset(int base) {
    outb(base + FLOPPY_DOR, 0x00);
    outb(base + FLOPPY_DOR, 0x0C);

    floppy_irqWait();

    int st0, cylinder;
    floppy_checkInterrupt(base, &st0, &cylinder);

    // Transfer speed 500kb/s
    outb(base + FLOPPY_REG_CCR, 0x00);

    floppy_sendCmd(base, FLOPPY_CMD_SPECIFY);
    floppy_sendCmd(base, 0xDF);
    floppy_sendCmd(base, 0x02);

    return floppy_calibrate() != 0;
}




int floppy_seek(int base, unsigned cylinder, int head) {
    floppy_motor(base, FLOPPY_MOTOR_ON);

    unsigned i, st0, cyl;
    for (i = 0; i < 10; i++) {
        floppy_sendCmd(base, FLOPPY_CMD_SEEK);
        floppy_sendCmd(base, head << 2);
        floppy_sendCmd(base, cylinder);

        floppy_irqWait();
        floppy_checkInterrupt(base, &st0, &cyl);

        if (st0 & 0xC0)
            continue;

        if (cyl == cylinder) {
            floppy_motor(base, FLOPPY_MOTOR_OFF);
            return 0;
        }
    }

    printStatusMsg(KRNL_STATUS_ERROR, "FLOPPY", "Seek error.", FLOPPY_ERROR_SEEK);
    floppy_motor(base, FLOPPY_MOTOR_OFF);

    return 1;
}




typedef enum {
    floppy_r = 1,
    floppy_w = 2
} floppy_tdir;

static const char floppy_dmaBuffer[FLOPPY_DMA_LEN] __attribute__((aligned(0x8000)));

static void floppy_dma_init(floppy_tdir direction) {
    union {
        uint32_t b[4];
        uint32_t l;
    } addr, count;

    addr.l  = (unsigned) &floppy_dmaBuffer;
    count.l = (unsigned) &FLOPPY_DMA_LEN - 1;

    if ((addr.l >> 24) || (count.l << 16) || ((addr.l & 0xFFFF)) >> 16) {
        printStatusMsg(KRNL_STATUS_FATAL, "FLOPPY", "DMA: Buffer error.", FLOPPY_FATAL_DMA_BUFF);
        return;
    }

    unsigned char mode;
    switch (direction) {
        case floppy_r: mode = 0x46; break;
        case floppy_w: mode = 0x4A; break;

        default:
            printStatusMsg(KRNL_STATUS_ERROR, "FLOPPY", "Invalid data transfer direction.", FLOPPY_ERROR_DIR);
            return;
    }


    outb(0x0a, 0x06);

    outb(0x0c, 0xff);
    outb(0x04, addr.b[0]);
    outb(0x04, addr.b[1]);

    outb(0x81, addr.b[2]);

    outb(0x0c, 0xff);
    outb(0x05, count.b[0]);
    outb(0x05, count.b[1]);

    outb(0x0b, mode);

    outb(0x0a, 0x02);
}

int floppy_doTrack(int base, unsigned cylinder, floppy_tdir direction) {
    unsigned char cmd;
    static const uint8_t = 0xC0;

    switch (direction) {
        case floppy_r: cmd = FLOPPY_CMD_READ_DATA  | flags; break;
        case floppy_w: cmd = FLOPPY_CMD_WRITE_DATA | flags; break;

        default:
            printStatusMsg(KRNL_STATUS_ERROR, "FLOPPY", "Invalid data transfer direction.", FLOPPY_ERROR_DIR);
            return 1;
    }

    if (floppy_seek(base, cylinder, 0) + floppy_seek(base, cylinder, 1))
        return -1;



    string errorMessages[] = {
        "general error",
        "end of cylinder",
        "drive not ready",
        "general CRC error",
        "time out",
        "no data",
        "no address mark",
        "deleted address mark",
        "data CRC error",
        "wrong cylinder",
        "no uPD765 sector",
        "sector not valid",
        "sector read-only",
    };

    int i;
    for (i = 0; i < 20; i++) {
        floppy_motor(base, FLOPPY_MOTOR_ON);

        floppy_dma_init(direction);
        SLEEP_TICKS(10); // 100 ms

        floppy_sendCmd(base, cmd);  // set above for current direction
        floppy_sendCmd(base, 0);    // 0:0:0:0:0:HD:US1:US0 = head and drive
        floppy_sendCmd(base, cyl);  // cylinder
        floppy_sendCmd(base, 0);    // first head (should match with above)
        floppy_sendCmd(base, 1);    // first sector, strangely counts from 1
        floppy_sendCmd(base, 2);    // bytes/sector, 128*2^x (x=2 -> 512)
        floppy_sendCmd(base, 18);   // number of tracks to operate on
        floppy_sendCmd(base, 0x1b); // GAP3 length, 27 is default for 3.5"
        floppy_sendCmd(base, 0xff); // data length (0xff if B/S != 0)

        floppy_irqWait();


        unsigned char   st0, 
                        st1, 
                        st2, 
                        rcy, 
                        rhe, 
                        rse, 
                        bps;

        st0 = floppy_readData(base);
        st1 = floppy_readData(base);
        st2 = floppy_readData(base);
        rcy = floppy_readData(base);
        rhe = floppy_readData(base);
        rse = floppy_readData(base);
        bps = floppy_readData(base);


        int error = 0;

        if (st0 & 0xC0)      error = 1;
        if (st1 & 0x80)      error = 2;
        if (st0 & 0x08)      error = 3;
        if (st1 & 0x20)      error = 4;
        if (st1 & 0x10)      error = 5;
        if (st1 & 0x04)      error = 6;
        if ((st1 | st2) & 1) error = 7;
        if (st2 & 0x40)      error = 8;
        if (st2 & 0x20)      error = 9;
        if (st2 & 0x10)      error = 10;
        if (st2 & 0x04)      error = 11;
        if (st2 & 0x02)      error = 10;
        if (bps != 0x2)      error = 12;
        if (st1 & 0x02)      error = 13;

        floppy_motor(base, FLOPPY_MOTOR_OFF);

        if (error) {
            printStatusMsg(KRNL_STATUS_FAIL, "FLOPPY", errorMessages[error - 1], error + FLOPPY_ERROR_CYL_END - 1);
            return error + FLOPPY_ERROR_CYL_END - 1;
        }

        return 0;
    }

    printStatusMsg(KRNL_STATUS_FAIL, "FLOPPY", "error", 500);
    return 1;
}

int floppy_readTrack(int base, unsigned cylinder) {
    return floppy_doTrack(base, cylinder, floppy_r);
}

int floppy_writeTrack(int base, unsigned cylinder) {
    return floppy_doTrack(base, cylinder, floppy_w);
}



void floppy_init() {
    floppy_detect();
    floppy_printList();
}