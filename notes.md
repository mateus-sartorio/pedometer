# How to configure I2C

I2Cn_CTRL EN register: if cleared, resets internal state of I2C (terminates ongoing transfers)

I2Cn_CTRL EN register: can be cleared to reset the state of the slave

I2Cn_CTRL CLHR register: controls ration between highs and lows in signals

TXC flag in I2Cn_STATUS and TXC flag in I2Cn_IF registers: are set when transmit shift register is out of data, and are cleared when new data becomes available. TXC interrupt flag must be cleared by software

I2Cn_CMD CLEARTX flag: will abbort any data being transfered

## Algorithm

set START in I2Cn_CMD

transmit ADDR with R/W

send arbitrary ammount of data

send START or STOP