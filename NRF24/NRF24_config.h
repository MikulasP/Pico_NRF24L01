#ifndef _NRF24_CONFIG
#define _NRF24_CONFIG
/*
    NRF24L01 Register map and allowed values
    For additional information read the official documentation:https://www.sparkfun.com/datasheets/Components/SMD/nRF24L01Pluss_Preliminary_Product_Specification_v1_0.pdf

    The register map can be fount in the documentation in between page 54 and 59

*/


/*******************************************************************************************************/

    /* Pins */

#define _NRF_SPI                spi0        //SPI bus 0
#define _NRF_SPI_BAUDRATE       5000000     // 5 MBit/s
#define _NRF_SPI_CHIPSELECT     0
#define _NRF_SPI_CHIPSELECTNOT  1
#define _NRF_SPI_CHIPENABLE     1
#define _NRF_SPI_CHIPENABLENOT  0

//For common SPI bus
#define _NRF_SPI_SCK            6
#define _NRF_SPI_MOSI           7
#define _NRF_SPI_MISO           4

/*

SPI 0 Pins:
	sck: GP6
	mosi: GP7
	miso: GP4

ce_tx = Pin(19, mode=Pin.OUT, value=0)
csn_tx = Pin(20, mode=Pin.OUT, value=1)
#irq_tx = Pin(21, mode=Pin.IN)
ce_rx = Pin(17, mode=Pin.OUT, value=0)
csn_rx = Pin(16, mode=Pin.OUT, value=1)
#irq_rx = Pin(18, mode=Pin.IN)

*/

/*******************************************************************************************************/

    /* Register map */

#define CONFIG      0x00    //Configuration register
#define EN_AA       0x01    //Enable Auto Acknowledgment
#define EN_RXADDR   0x02    //Enable RX addresses
#define SETUP_AW    0x03    //Setup of address widths (common for all data pipes)
#define SETUP_RETR  0x04    //Setup of Automatic Retransmission (delay and count)
#define RF_CH       0x05    //RF Channel
#define RF_SETUP    0x06    //RF Setup register
#define STATUS      0x07    //Status register (In paralell to the SPI command word applied on the MOSI pin, the STATUS register is shifted serially out on the MISO pin)
#define OBSERVE_TX  0x08    //Transmit observe register
#define RPD_REG     0x09    //Received Power Detector
#define RX_ADDR_P0  0x0A    //Receive address data pipe 0. 5 Bytes maximum. (LSByte is written first. The number of bytes is defined by SETUP_AW register)
#define RX_ADDR_P1  0x0B    //Receive address data pipe 1. 5 Bytes maximum. (LSByte is written first. The number of bytes is defined by SETUP_AW register)
#define RX_ADDR_P2  0x0C    //Receive address data pipe 2. Only LSB. MSBytes are equal to RX_ADDR_P1/
#define RX_ADDR_P3  0x0D    //Receive address data pipe 3. Only LSB. MSBytes are equal to RX_ADDR_P1
#define RX_ADDR_P4  0x0E    //Receive address data pipe 4. Only LSB. MSBytes are equal to RX_ADDR_P1
#define RX_ADDR_P5  0x0F    //Receive address data pipe 5. Only LSB. MSBytes are equal to RX_ADDR_P1
#define TX_ADDR     0x10    //Transmit address. (LSByte written first.)
#define RX_PW_P0    0x11    //Number of bytes in RX payload in data pipe 0 (1 to 32 bytes) 1 = 1byte ... 32 = 32 byte
//#define RX_PW_P1    0x12    //Number of bytes in RX payload in data pipe 1 (1 to 32 bytes) 1 = 1byte ... 32 = 32 byte
//#define RX_PW_P2    0x13    //Number of bytes in RX payload in data pipe 2 (1 to 32 bytes) 1 = 1byte ... 32 = 32 byte
//#define RX_PW_P3    0x14    //Number of bytes in RX payload in data pipe 3 (1 to 32 bytes) 1 = 1byte ... 32 = 32 byte
//#define RX_PW_P4    0x15    //Number of bytes in RX payload in data pipe 4 (1 to 32 bytes) 1 = 1byte ... 32 = 32 byte
//#define RX_PW_P5    0x16    //Number of bytes in RX payload in data pipe 5 (1 to 32 bytes) 1 = 1byte ... 32 = 32 byte
#define FIFO_STATUS 0x17    //FIFO status register

    //Note: Addresses 18 to 1B are reserved for test purpouses, altering them makes the chip malfunction!

#define DYNPD       0x1C    //Enable dynamic payload length
#define FEATURE     0x1D    //Feature register

/*******************************************************************************************************/

    /* Bit Mnemonics */


//CONFIG register
#define MASK_RX_DR  6       //Mask interrupt caused by RX_DR. 1: not reflected on IRQ pin | 0: Reflect RX_DR on IRQ pin as active low
#define MASK_TX_DS  5       //Mask interrupt caused by TX_DS. 1: not reflected on IRQ pin | 0: Reflect TX_DS on IRQ pin as active low
#define MASK_MAX_RT 4       //Mask interrupt caused by MAX_RT. 1: not reflected on IRQ pin | 0: Reflect RX_DR on MAX_RT pin as active low
#define EN_CRC      3       //Enable CRC. Forced Forced high if E_AA is high
#define CRC0        2       //CRC Length. 0: 1 bytes | 1: 2 bytes
#define PWR_UP      1       //1: POWER UP | 0: POWER DOWN
#define PRIM_RX     0       //RX/TX control. 1: PRX | 0: PTX


//EN_AA register
#define ENAA_P5     5       //Enable auto acknowledgement on data pipe 5
#define ENAA_P4     4       //Enable auto acknowledgement on data pipe 4
#define ENAA_P3     3       //Enable auto acknowledgement on data pipe 3
#define ENAA_P2     2       //Enable auto acknowledgement on data pipe 2
#define ENAA_P1     1       //Enable auto acknowledgement on data pipe 1
#define ENAA_P0     0       //Enable auto acknowledgement on data pipe 0


//EN_RXADDR register
#define ERX_P5      5       //Enable data pipe 5
#define ERX_P4      4       //Enable data pipe 4
#define ERX_P3      3       //Enable data pipe 3
#define ERX_P2      2       //Enable data pipe 2
#define ERX_P1      1       //Enable data pipe 1
#define ERX_P0      0       //Enable data pipe 0


//SETUP_AW register
/*
    Sets the address width of the Enhanced ShockBurst packet

    AW_1            3 Bytes
    AW_2            4 Bytes
    (AW_1 | AW_2)   5 Bytes
*/


//SETUP_RETR register
/*
    Auto Retransmit Delay

    Bits 7:4

    0000 - Wait 250µs
    0001 - Wait 500µs
    0010 - Wait 750µs
    ...
    1111 - Wait 4000µs

    Delay defined from end of transmission to start of next transmission.

*/
enum AUTO_RD {
    US250 = 0,              //0000 0000
    US500 = 16,             //0001 0000
    US750 = 32,             //0010 0000
    US1000 = 48,            //0011 0000
    US1250 = 64,            //0100 0000
    US1500 = 80,            //0101 0000
    US1750 = 96,            //0110 0000
    US2000 = 112,           //0111 0000
    US2250 = 128,           //1000 0000
    US2500 = 144,           //1001 0000
    US2750 = 160,           //1010 0000
    US3000 = 176,           //1011 0000
    US3250 = 192,           //1100 0000
    US3500 = 208,           //1101 0000
    US3750 = 224,           //1110 0000
    US4000 = 240            //1111 0000
};
/*
    Auto Retransmit Count

    Bits 3:0

    0000 (0) - Re-Transmit disabled
    0001 (1) - Up to 1 Re-Transmit on fail of AA (Auto Acknowledgment)
    ...
    1111 (15) - Up to 15 Re-Transmit on fail of AA (Auto Acknowledgment)

*/


//RF_CH
/*
    RF_CH                   //Sets the frequency channel the module operates on

    Bits 6:0    

*/

//RF_SETUP register
#define CONT_WAVE   7       //Enables continuous carrier transmit when high
                            //Bit 6 is reserved. Only 0 allowed!
#define RF_DR_LOW   5       //Set RF Data Rate to 250kbps
#define PLL_LOCK    4       //Force PLL lock signal. Only used in test
#define RF_DR_HIGH  3       //Select between the high speed data rates. This bit is don't care if RF_DR_LOW is set.
#define RF_PWR_LOW  2
#define RF_PWR_HIGH 1
/*
    RF_PWR - Transmit power

    00 - -18dBm     0
    01 - -12dBm     RF_PWR_LOW
    10 - -6dBm      RF_PWR_HIGH
    11 - 0dBm       (RF_PVR_LOW | RF_PWR_HIGH)

*/


//STATUS register
#define RX_DR       6       //Data Ready RX FIFO interrupt. Asserted when new data arrives RX FIFO.
#define TX_DS       5       //Data Sent TX FIFO interrupt. Asserted when packet transmitted on TX. (If AUTO_ACK is enabled this bit is set high only when ACK is received.)
#define MAX_RT      4       //Max number of TX retransmit interrupt. Write 1 to clear bit. (If MAX_RT is asserted it must be cleared to enable further communications.)
/*
    RX_P_NO                 //Data pipe number for the payload available for reading from RX FIFO

    Bits 3:1

    000 - 101 Data Pipe Number
    110 - Not Used
    111 - RX FIFO empty

*/
#define TX_FULL_STAT    0   //TX FIFO full flag. 1: TX FIFO full | 0: Locations available in TX FIFO


//OBSERVE_TX register
/*
    PLOS_CNT                //Count lost packets. Counter is overflow protected to 15

    Bits 7:4

    The counter is reset by writing to RF_CH

*/
/*
    ARC_CNT                 //Count retransmitted packets

    Bits 3:0

    The counter is reset when transmission of a new packet starts.
*/


//RPD register
#define RPD         0       //Received Power Detector


//FIFO_STATUS register
#define TX_REUSE    6       //Used for a PTX device
#define TX_FULL     5       //TX FIFO full flag. 1: TX FIFO full | 0: Available locations in TX FIFO
#define TX_EMPTY    4       //TX FIFO empty flag. 1: TX FIFO empty | 0: Data in TX FIFO
#define RX_FULL     1       //RX FIFO full flag. 1: RX FIFO full | 0: Available locations in RX FIFO
#define RX_EMPTY    0       //RX FIFO empty flag. 1: RX FIFO empty | 0: Data in RX FIFO


//DYNPD register
#define DPL_P5      5       //Enable dynamic payload length data pipe 5
#define DPL_P4      4       //Enable dynamic payload length data pipe 4
#define DPL_P3      3       //Enable dynamic payload length data pipe 3
#define DPL_P2      2       //Enable dynamic payload length data pipe 2
#define DPL_P1      1       //Enable dynamic payload length data pipe 1
#define DPL_P0      0       //Enable dynamic payload length data pipe 0


//FEATURE register
#define EN_DPL      2       //Enables dynamic payload length
#define EN_ACK_PAY  1       //Enables payload with ACK
#define EN_DYN_ACK  0       //Enables the W_TX_PAYLOAD_NOACK command


/*******************************************************************************************************/

    /* Command Mnemonics */

#define R_REGISTER          0x00     //Read register command
#define W_REGISTER          0x20     //Write register command
#define REG_MASK            0x1F     //5 bit register mask
#define R_RX_PAYLOAD        0x61     //Read from RX FIFO
#define W_TX_PAYLOAD        0xA0     //Write to TX FIFO
#define FLUSH_TX            0xE1     //Flush TX FIFO
#define FLUSH_RX            0xE2     //Flus RX FIFO
#define REUSE_TX_PL         0xE3     //Reuse last transmitted payload. (Used for PTX device)
#define R_RX_PL_WID         0x60     //Read RX payload width for the top R_RX_PAYLOAD in the RX FIFO
#define W_ACK_PAYLOAD       0xA8     //Write payload to be transmitted together with ACK packet on pipe PPP (Bits: 10101PPP | is valid from 000 to 101)
#define W_TX_PAYLOAD_NOACK  0xB0     //Disables AUTOACK on this specific packet (Used in TX mode)
#define NOP                 0xFF     //No operation. Can be used to read the STATUS register


/*******************************************************************************************************/

/*******************************************************************************************************/

#endif
