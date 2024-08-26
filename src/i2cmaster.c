/**
 * @file    i2cmaster.c
 * @brief   I2C HAL for EFM32GG
 * @version 1.0
 *
 * @note    It uses polling
 *
 */

#include "../include/i2cmaster.h"
#include "../include/clock_efm32gg2.h"
#include "../include/em_device.h"
#include <stdint.h>

enum states {
    IDLE,
    SEND_DATA_1,
    SEND_DATA_2,
    SEND_DATA_3,
    RECEIVE_DATA,
    STOP
};

enum operation {
    WRITE = 0,
    READ = 1
};

enum operation_type {
    GENERAL_READ,
    GENERAL_WRITE
};

static enum states state = IDLE;
static uint8_t send_address = 0;
static uint8_t send_register = 0;
static uint8_t value_to_write = 0;
static uint8_t data = 0;
static const int BYTE_LIMIT = 6;
static enum operation_type general_operation = GENERAL_WRITE;

static void MyProcessInterrupt(I2C_TypeDef* i2c) {
    if (general_operation == GENERAL_WRITE) {
        switch (state)
        {
        case IDLE:
            /* Disable interrupt sources when done */
            i2c->IEN = 0;

            break;

        case SEND_DATA_1:
            if (i2c->IF & I2C_IF_NACK)
            {
                state = STOP;
                i2c->IFC = I2C_IFC_NACK;
                i2c->CMD = I2C_CMD_STOP;
            }
            else if (i2c->IF & I2C_IF_ACK) {
                i2c->IFC = I2C_IFC_ACK;

                state = SEND_DATA_2;

                i2c->TXDATA = send_register;
            }

            break;

        case SEND_DATA_2:
            if (i2c->IF & I2C_IF_NACK)
            {
                i2c->IFC = I2C_IFC_NACK;
                state = STOP;
                i2c->CMD = I2C_CMD_STOP;
            }
            else if (i2c->IF & I2C_IF_ACK) {
                i2c->IFC = I2C_IFC_ACK;

                state = SEND_DATA_3;

                i2c->TXDATA = value_to_write;
            }

            break;

        case SEND_DATA_3:
            if (i2c->IF & I2C_IF_NACK)
            {
                i2c->IFC = I2C_IFC_NACK;
                state = STOP;
                i2c->CMD = I2C_CMD_STOP;
            }
            else if (i2c->IF & I2C_IF_ACK) {
                i2c->IFC = I2C_IFC_ACK;

                state = IDLE;

                i2c->CMD = I2C_CMD_NACK;
                i2c->CMD = I2C_CMD_STOP;
            }

            break;

        case STOP:
            i2c->IFC = I2C_IFC_MSTOP;
            state = IDLE;

        default:
            break;
        }
    }
    else {
        switch (state) {
        case IDLE:
            /* Disable interrupt sources when done */
            i2c->IEN = 0;

            break;

        case SEND_DATA_1:
            if (i2c->IF & I2C_IF_NACK)
            {
                i2c->IFC = I2C_IFC_NACK;
                i2c->CMD = I2C_CMD_STOP;
                state = IDLE;
            }
            else if (i2c->IF & I2C_IF_ACK) {
                i2c->IFC = I2C_IFC_ACK;
                i2c->TXDATA = send_register;
                state = SEND_DATA_2;
            }

            break;

        case SEND_DATA_2:
            if (i2c->IF & I2C_IF_NACK)
            {
                i2c->IFC = I2C_IFC_NACK;
                i2c->CMD = I2C_CMD_STOP;
                state = STOP;
            }
            else if (i2c->IF & I2C_IF_ACK) {
                i2c->IFC = I2C_IFC_ACK;

                /* We have to write START cmd first since repeated start, otherwise */
                /* data would be sent first. */
                i2c->CMD = I2C_CMD_START;
                i2c->TXDATA = (send_address << 1) | READ;

                state = SEND_DATA_3;
            }

            break;

        case SEND_DATA_3:
            if (i2c->IF & I2C_IF_NACK)
            {
                state = IDLE;
                i2c->IFC = I2C_IFC_NACK;
                i2c->CMD = I2C_CMD_STOP;
            }
            else if (i2c->IF & I2C_IF_ACK)
            {
                i2c->IFC = I2C_IFC_ACK;
                state = RECEIVE_DATA;
            }

            break;

        case RECEIVE_DATA:
            if (i2c->IF & I2C_IF_RXDATAV) {
                data = i2c->RXDATA;

                /* If there is only one byte to receive we need to transmit the
                NACK now, before the stop. */
                i2c->CMD = I2C_CMD_NACK;
                i2c->CMD = I2C_CMD_STOP;

                state = IDLE;
            }

            break;

        case STOP:
            i2c->IFC = I2C_IFC_MSTOP;
            state = IDLE;

            break;

        default:
            break;
        }
    }
}

uint8_t I2C_MyRead(I2C_TypeDef* i2c, uint8_t address, uint8_t reg) {
    i2c->CMD = I2C_CMD_STOP;

    general_operation = GENERAL_READ;
    state = IDLE;

    send_address = address;
    send_register = reg;

    // Clears data variable
    data = 0;


    /* Check if in busy state. Since this SW assumes single master, we can */
    /* just issue an abort. The BUSY state is normal after a reset. */
    if (i2c->STATE & I2C_STATE_BUSY)
    {
        i2c->CMD = I2C_CMD_ABORT;
    }

    /* Ensure buffers are empty */
    i2c->CMD = I2C_CMD_CLEARPC | I2C_CMD_CLEARTX;
    if (i2c->IF & I2C_IF_RXDATAV)
    {
        (void)i2c->RXDATA;
    }

    /* Enable those interrupts we are interested in throughout transfer. */
    /* Notice that the I2C interrupt must also be enabled in the NVIC, but */
    /* that is left for an additional driver wrapper. */
    // i2c->IEN |= I2C_IEN_ACK | I2C_IEN_TXBL | I2C_IEN_NACK | I2C_IEN_RXDATAV;
    // Disables all interrupts
    i2c->IEN = 0;
    i2c->IEN |= I2C_IF_NACK | I2C_IF_ACK | I2C_IF_RXDATAV;

    state = SEND_DATA_1;

    /* Clear all pending interrupts prior to starting transfer. */
    i2c->IFC = _I2C_IFC_MASK;

    // Starts comunication
    /* Data not transmitted until START sent */
    i2c->CMD = I2C_CMD_START;
    i2c->TXDATA = (address << 1) | WRITE;
    
    while (state != IDLE) {
        if (i2c->STATE == 0) {
            return I2C_MyRead(i2c, address, reg);
        }
    }

    // Disable interrupts
    i2c->IEN = 0;

    /* Clear all pending interrupts prior to starting transfer. */
    i2c->IFC = _I2C_IFC_MASK;

    return data;
}

int I2C_MyWrite(I2C_TypeDef* i2c, uint8_t address, uint8_t reg, uint8_t value) {
    i2c->CMD = I2C_CMD_STOP;

    general_operation = GENERAL_WRITE;
    state = IDLE;

    send_address = address;
    send_register = reg;
    value_to_write = value;

    /* Check if in busy state. Since this SW assumes single master, we can */
    /* just issue an abort. The BUSY state is normal after a reset. */
    if (i2c->STATE & I2C_STATE_BUSY)
    {
        i2c->CMD = I2C_CMD_ABORT;
    }

    /* Ensure buffers are empty */
    i2c->CMD = I2C_CMD_CLEARPC | I2C_CMD_CLEARTX;
    if (i2c->IF & I2C_IF_RXDATAV)
    {
        (void)i2c->RXDATA;
    }


    // Enable interrupts
    i2c->IEN = 0;
    i2c->IEN |= I2C_IEN_ACK;

    state = SEND_DATA_1;

    i2c->CMD = I2C_CMD_START;
    i2c->TXDATA = (address << 1) | WRITE;

    while (state != IDLE) {
        if (i2c->STATE == 0) {
            return I2C_MyWrite(i2c, address, reg, value);
        }
    }

    // Disable interrupts
    i2c->IEN = 0;

    /* Clear all pending interrupts prior to starting transfer. */
    i2c->IFC = _I2C_IFC_MASK;

    return 0;
}

/** Interrupt Handlers */
///@{
void I2C0_IRQHandler(void) {
    MyProcessInterrupt(I2C0);
}

void I2C1_IRQHandler(void) {
    MyProcessInterrupt(I2C1);
}
///@}

/**
 * @brief  Enable clock for I2C module
 *
 * @param  i2c: Pointer to I2C module registers
 *
 * @return 0: OK. Negative: Error
 */
static int I2CMaster_EnableClock(I2C_TypeDef* i2c) {
    if (i2c == I2C0) {
        CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_I2C0;
    }
    else if (i2c == I2C1) {
        CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_I2C1;
    }
    else {
        return -1;
    }

    return 0;
}

/**
 * @brief   Initialize I2C unit as master
 *
 * @parms  i2c:     Pointer to I2C register area as defined in Silicon Labs CMSIS header files
 * @parms  speed:   Speed in KHz (e.g. 100, 400, but can be less)
 * @parms  loc:     Location code for both (SCL and SDA) pins
 *
 *
 * | Signal     | #0  | #1  | #2  | #3  | #4  | #5  | #6  |                                        |
 * |------------|-----|-----|-----|-----|-----|-----|-----|----------------------------------------|
 * | I2C0_SCL   | PA1 | PD7 | PC7 | PD15| PC1 | PF1 | PE13| I2C0 Serial Clock Line input / output  |
 * | I2C0_SDA   | PA0 | PD6 | PC6 | PD14| PC0 | PF0 | PE12| I2C0 Serial Data input / output        |
 * | I2C1_SCL   | PC5 | PB12| PE1 |     |     |     |     | I2C1 Serial Clock Line input / output  |
 * | I2C1_SDA   | PC4 | PB11| PE0 |     |     |     |     | I2C1 Serial Data input / output        |
 *
 */
int I2CMaster_Init(I2C_TypeDef* i2c, uint32_t speed, uint8_t loc) {

    // Enable clock for I2C peripheral
    I2CMaster_EnableClock(i2c);

    /* Enable I2C_TypeDef peripheral */
    i2c->CTRL |= I2C_CTRL_EN;

    /* Configure speed for present clock frequency */
    I2CMaster_ConfigureSpeed(i2c, speed);

    /* Configure pins to be used for I2C_TypeDef */
    i2c->ROUTE = (i2c->ROUTE & ~_I2C_ROUTE_LOCATION_MASK) | (loc << _I2C_ROUTE_LOCATION_SHIFT);
    /* Enable pins */
    i2c->ROUTE |= (I2C_ROUTE_SCLPEN | I2C_ROUTE_SDAPEN);

    /* Configure as master */
    i2c->CTRL &= ~I2C_CTRL_SLAVE;
    i2c->CTRL |= I2C_CTRL_ARBDIS;

    /* Additional configuration */
    i2c->CTRL |= I2C_CTRL_AUTOSN;  /* Automatic STOP on NACK (Master only) */
    i2c->CTRL |= I2C_CTRL_AUTOACK; /* Automatic STOP on NACK (Master only) */

    i2c->IEN = 0;
    i2c->IFC = _I2C_IFC_MASK;

    // Enable interrupts on NVIC
    NVIC_ClearPendingIRQ(I2C1_IRQn);
    NVIC_ClearPendingIRQ(I2C1_IRQn);
    NVIC_EnableIRQ(I2C1_IRQn);
    NVIC_EnableIRQ(I2C1_IRQn);

    return 0;
}

/**
 * @brief   Configure I2C unit according to the actual clock
 *
 * @note    f_SCL  = 1/(T_low+T_high)
 *
 * @note    The formulas below does not work for N less than 3
 *
 *          T_high = (N_high x (CLKDIV+1))/f_HFPERCLK
 *          T_low  = (N_low x (CLKDIV+1))/f_HFPERCLK
 *
 * @note
 *          Clock Low/High Ratio
 *
 *          STANDARD                      4:4
 *          ASYMMETRIC                    6:3
 *          FAST                         11:6
 *
 */
int I2CMaster_ConfigureSpeed(I2C_TypeDef* i2c, uint32_t speed) {
    uint32_t div;
    uint32_t perclk;
    unsigned t2, th, tl;
    uint32_t nh, nl;
    ClockConfiguration_t clockconf;

    ClockGetConfiguration(&clockconf);

    perclk = clockconf.hfperclkfreq;
    div = clockconf.hclkdiv;

    t2 = 1000 * perclk / speed;

    // tl = (t2 + 1) / 2; // Must be adjusted for non symmetrical clock signal
    // th = t2 - tl;

    // Calculate N_high and N_low for symmetric clock periods
    nh = (t2 / 2) - 1;
    nl = (t2 / 2) - 1;

    // // TODO: Calculate N_high and N_low
    // nh = 0;
    // nl = 0;

    i2c->CLKDIV = 500;

    return 0;
}