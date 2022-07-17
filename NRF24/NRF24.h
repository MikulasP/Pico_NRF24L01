#ifndef NRF24L01_OK
#define NRF24L01_OK



    /* Data structures */

//For non-common SPI

struct _NRF_SPI_PINOUT {
    uint8_t sck;
    uint8_t mosi;
    uint8_t miso;
    uint8_t ce;
    uint8_t csn;
    uint8_t irq;
};


//For common SPI
/*
struct _NRF_SPI_PINOUT {
    uint8_t ce;
    uint8_t csn;
    uint8_t irq;
};*/

//Radio power level
enum PA_LEVEL {_MIN = 0, _LOW = 2, _HIGH = 4, _MAX = 6};

//Radio data rate
enum DATA_RATE {_1M = 0, _2M = 8, _250K = 32};

//Pipe address width 
enum ADDR_WIDTH {_3B = 1, _4B = 2, _5B = 3};

//
size_t _BV(uint8_t x);

class NRF24 {

private:

    _NRF_SPI_PINOUT pinout;         //Device pinout
    uint8_t payload_size = 32;      //Fixed payload size
    uint8_t address_width = 0;      //Pipe address width
    uint8_t status_reg = 0;         //The CONFIG register's value
    
    bool debug_messages = false;    //Print debug messages to serial
    bool power_state = false;       //Radio hw power state. true: Standby-I mode | false: Power Down mode
    bool prim_rx_mode = false;      //PRIM_RX mode. true: RX mode | false: TX mode
    bool var_payload_size = false;  //Enable or disable variable payload size (payload_size will be ignored) //!Not implemented in Read function yet!

    uint8_t rx_buff[32 + 1];        //RX buffer for payload read
    uint8_t tx_buff[32 + 1];        //TX buffer for payload write

    uint8_t rx_reg[5 + 1];          //RX buffer for register read
    uint8_t tx_reg[5 + 1];          //TX buffer for register write

public:

    /**
    * @brief Set up the chip for normal operation
    * 
    * @param _pinout Control pins of the device
    */
    void Init(const _NRF_SPI_PINOUT _pinout);
    //*OK

    /**
    * @brief Checks if the device is connected and working
    * 
    * @return true: device available | false: cannot communicate with device
    */
    bool ChipAvailable(void);
    //*OK

    /**
    * @brief Send the device into PowerDown mode
    */
    void PowerDown(void);
    //*OK

    /**
    * @brief Wake the device from PowerDown mode
    */
    void PowerUp(void);
    //*OK

    /**
    * @brief 
    */
    void SetRXMode(bool power_up = true);
    //*OK


    /**
    * @brief 
    */
    void SetTXMode(bool power_up = true);
    //*OK

    /**
    * @brief Set ChipEnable high for immediate TX data transfer
    */
    void SetTXHigh();
    //*OK

    /**
    * @brief Set ChipEnable Low manually in TX mode
    */
    void SetTXLow();
    //*OK

    /**
    * @brief Put the device into RX mode and start listening on opened data pipes
    * 
    * @return true: Device started listening | false: Operation cannot be done (not in RX mode, already listening, ...)
    */
    bool StartListening(void);
    //*OK
    
    //Puts the device into RX mode
    /**
    * @brief Stop listening on opened data pipes. Only works if device is in RX mode
    */
    void StopListening(void);
    //*OK

    /**
    * @brief Check if the radio is listening for data
    * 
    * @return true: listening | false: not listening
    */
    bool IsListening(void);
    //*OK
    
    /**
    * @brief Check if there are any packets in the RX FIFO available to read, and can return the data pipe's number
    * 
    * @param pipeNumber RX pipe number
    * 
    * @return true: Data present in FIFO on given pipe | false: No data present in FIFO on given pipe
    */
    bool Available(uint8_t pipe);
    //*OK
    
    /**
    * @brief Open a data pipe for writing
    * 
    * @param address Pipe address
    */
    void OpenWritingPipe(uint8_t *address);
    //*OK

    /**
    * @brief Open a data pipe for writing
    * 
    * @param address Pipe address
    */
    //void OpenWritingPipe(uint64_t address);
    //!Not yet implemented function

    //Close a writing data pipe
    /**
    * @brief Reset writing pipe address to default value
    */
    void CloseWritingPipe(void);
    //*OK
    
    /**
    * @brief Open a data pipe for reading | Note:
    * 
    * On data pipe 0-1 address width is 5 bytes.
    * On data pipe 2-5 only LSByte is written, first 4 bytes equal to the pipe 1 address.
    * When setting data pipe 2-5 only provide the last byte of the address!
    * 
    * @param num Pipe number
    * @param address Pipe address
    */
    void OpenReadingPipe(uint8_t pipe, uint8_t *address);
    //*OK

    //
    /**
    * @brief Open a data pipe for reading
    * 
    * @param num Pipe number
    * @param address Pipe address
    */
    //void OpenReadingPipe(uint8_t num, uint64_t address);
    //!Not yet implemented function

    /**
    * @brief Close a reading data pipe
    * 
    * @param num Pipe number
    */
    void CloseReadingPipe(uint8_t pipe);
    //*OK
    
    /**
    * @brief Write <len> bytes of data to the TX FIFO. Returns true if the operation was successfull
    * 
    * @param *src Buffer of data to write
    * @param len Number of bytes to write
    */
    bool Write(uint8_t *src, uint8_t len);
    //*OK
    
    /**
    * @brief Read <len> bytes of data from the RX FIFO. Returns true if the operation was successfull
    * 
    * @param *dst Buffer for read data
    * @param len Number of bytes to read
    */
    bool Read( uint8_t *dst, uint8_t len);
    //*OK
    
    /**
    * @brief Read 1 byte of data from a register
    * 
    * @param reg Address of the register
    * 
    * @return The register's content 
    */
    uint8_t ReadReg(uint8_t reg);
    //*OK

    /**
    * @brief Read <len> bytes into <dst> from a register
    * 
    * @param reg  Address of the register
    * @param *dst Buffer for read data
    * @param len  Number of bytes to read
    */
    void ReadReg(uint8_t reg, uint8_t* dst, uint8_t len);
    //*OK
    
    /**
    * @brief Write 1 byte of data to a register
    * 
    * @param reg Address of the register
    * @param value Value to write into the register
    */
    void WriteReg(uint8_t reg, uint8_t value);
    //*OK

    /**
    * @brief Write <len> to a register from <src>
    * 
    * @param reg Address of the register
    * @param *src Buffer for write data
    * @param len Number of bytes to write
    */
    void WriteReg(uint8_t reg, uint8_t* src, uint8_t len);
    //*OK
    
    //Write a single ACK packet on the specified data pipe
    //bool WriteAckPayload(uint8_t pipe, const void* buff, uint8_t len, bool loadAndSend = 1);
    //!Not yet implemented function
    
    //This function should be called as soon as transmission is finished to drop the radio back to STANDBY-I mode. If not issued, the radio will remain in STANDBY-II mode which, per the data sheet, is not a recommended operating mode.
    //void TXStandby();
    //!Not yet implemented function
    
    //
    //void SetAutoACK(bool mode, uint8_t pipeNumber);
    //!Not yet implemented function
    
    /**
    * @brief Flush the TX buffer
    */
    void FlushTX(void);
    //*OK
    
    /**
    * @brief Flush the RX buffer
    */
    void FlushRX(void);
    //*OK

    /**
    * @brief Enable or disable variable payload size
    * 
    * @param enabled true: enable | false: disable
    */
    //void VariablePayload(bool enabled = false);
    //!Not yet implemented function

//  --------------------------------------------------------------------------------
/*      Test Functions  */
//  --------------------------------------------------------------------------------

    //
    //bool TestCarrier(void);
    //!Not yet implemented function
    
    //
    //bool TestRDP(void);
    //!Not yet implemented function
    
    //
    //bool IsHardwareValid(void);
    //!Not yet implemented function
    
    //
    //bool IsNRF24L01Plus(void);
    //!Not yet implemented function
    
    //
    //void StartConstantCarrier(uint8_t paLevel, uint8_t channel);
    //!Not yet implemented function
    
    //
    //void StopConstantCarrier(void);
    //!Not yet implemented function

//  --------------------------------------------------------------------------------
/*      Radio Parameters        */
//  --------------------------------------------------------------------------------

    /**
    * @brief Check if there are any packets in the RX FIFO available to read
    * 
    * @return true: Data present in FIFO | false: No data present in FIFO
    */
    bool Available(void);
    //*OK

    /**
    * @brief Checks whether the RX FIFO is full
    * 
    * @return true: FIFO is full | false: Free space in FIFO
    */
    bool RX_FIFO_Full(void);
    //*OK
    
    /**
    * @brief Checks whether the RX FIFO is empty
    * 
    * @return true: FIFO empty | false: Data in FIFO
    */
    bool RX_FIFO_Empty(void);
    //*OK
    
    /**
    * @brief Checks whether the TX FIFO is full
    * 
    * @return true: FIFO is full | false: Free space in FIFO
    */ 
    bool TX_FIFO_Full(void);
    //*OK
    
    /**
    * @brief Checks whether the TX FIFO is full
    * 
    * @return true: FIFO empty | false: Data in FIFO
    */
    bool TX_FIFO_Empty(void);
    //*OK

    /**
    * @brief Get the chip's power state
    * 
    * @param update update the stored value by reading the CONFIG register
    * 
    * @return true: Standby-I state | false: PowerDown state
    */
    bool GetPowerState(bool update = false);
    //*OK

    /**
    * @brief ChipSelect state
    * 
    * @return true: ChipSelect | false: ChipSelectNot
    */
    bool IsChipSelect(void);
    //*OK

    /**
    * @brief ChipEnable state
    * 
    * @return true: ChipEnable | false: ChipEnableNot
    */
    bool IsChipEnable(void);
    //*OK

    /**
    * @brief Get PRIM_RX state
    * 
    * @param update update the stored value by reading the CONFIG register
    * 
    * @return true: RX mode | false: TX mode
    */
    bool GetHWMode(bool update = false);
    //*OK

    //Enable dynamic payload on a specific data pipe
    //void Enable_Dynamic_Payload_Length_on_Pipe(uint8_t pipe_num, bool mode);
    //!Not yet implemented function

    //Get the stored status register value
    /**
    * @brief Get the Status Reg object
    * 
    * @param update Update the stored value by reading the register
    *  
    * @return The STATUS register's value
    */
    uint8_t GetStatusReg(bool update = false);
    //*OK
    
    //Enable Dynamic Payload Length
    //void DynamicPayloadLength(bool mode);
    //!Not yet implemented function
    
    //Enable or disable Payload ACK
    //void SetPayloadACK(bool mode);
    //!Not yet implemented function
    
    //Enable or disable dynamic Payload ACK
    //void SetDynamicPayloadACK(bool mode);
    //!Not yet implemented function
    
    /**
    * @brief Get the first payload's length in RX buffer
    * 
    * @return First payload's length
    */
    uint8_t GetPayloadLength(void);
    //*OK

    /**
    * @brief Get the payload's length in given data pipe FIFO
    * 
    * @param pipe Pipe number (0-5)
    * @return uint8_t Number of bytes in FIFO
    */
    uint8_t GetPayloadLength(uint8_t pipe);
    //*OK
    
    /**
    * @brief Set fixed payload size to send and receive
    * 
    * @param size Fix payload size (1 - 32)
    */
    void SetPayloadSize(uint8_t size);
    //*OK

    /**
    * @brief Get fixed payload size
    * 
    * @return uint8_t Fix payload size
    */
    uint8_t GetPayloadSize();
    //*OK
    
    /**
    * @brief Set pipe address width
    * 
    * @param width Address width
    */
    void SetAddressWidth(ADDR_WIDTH width);
    //*OK

    /**
     * @brief Get pipe address Width
     * 
     * @return ADDR_WIDTH Address width
     */
    ADDR_WIDTH GetAddressWidth(void);
    //*OK
    
    //
    //void SetTXRetries(uint8_t delay, uint8_t retries);
    //!Not yet implemented function
    
    /**
    * @brief Set RF channel
    * 
    * @param channel Channel number
    */
    void SetChannel(uint8_t channel);
    //*OK
    
    /**
    * @brief Get RF channel
    * 
    * @return uint8_t Channel number
    */
    uint8_t GetChannel(void);
    //*OK
    
    /**
    * @brief Set the radio's power level
    * 
    * @param level Power level
    */
    void SetPALevel(PA_LEVEL level);
    //*OK
    
    /**
    * @brief Get the radio's power level
    * 
    * @return PA_LEVEL Power level
    */
    PA_LEVEL GetPA_Level(void);
    //*OK
    
    /**
    * @brief Set data rate
    * 
    * @param rate Data rate
    */
    void SetDataRate(DATA_RATE rate);
    //*OK

    /**
    * @brief Get data rate
    * 
    * @return DATA_RATE Data rate
    */
    DATA_RATE GetDataRate(void);
    //*OK
    
    /**
    * @brief Set CRC status
    * 
    * @param mode true: Enable CRC | false: Disable CRC
    */
    void SetCRC(bool mode);
    //*OK

    /**
    * @brief Get CRC status
    * 
    * @return true: CRC enabled | false: CRC disabled
    */
    bool GetCRC(void);
    //*OK

    /**
    * @brief Set CRC length
    * 
    * @param len CRC length (1, 2)
    */
    void SetCRCLength(uint8_t len);
    //*OK

    /**
    * @brief Get CRC length
    * 
    * @return uint8_t CRC length
    */
    uint8_t GetCRCLength(void);
    //*OK

    /**
    * @brief Set Auto Acknowledgement status
    * 
    * @param ack true: Enable Auto Ack | false: Disable Auto Ack
    */
    void SetAutoAck(bool ack = true);
    //*OK

    /**
    * @brief Get Auto Acknowledgement status
    * 
    * @return true: Auto Ack enabled | false: Auto Ack disabled
    */
    bool GetAutoAck(void);
    //*OK

    //
    //uint8_t GetARC_Count(void);
    //!Not yet implemented function
    
    //
    //void MaskIRQ(bool tx_ok, bool tx_fail, bool rx_ready);
    //!Not yet implemented function

//  --------------------------------------------------------------------------------
/*      Misc                */
//  --------------------------------------------------------------------------------

    /**
    * @brief Erase the TX and RX temp buffer's content
    */
    void EraseTempBuffs(void);
    //*OK

//  --------------------------------------------------------------------------------
/*      Radio Operation     */
private:
//  --------------------------------------------------------------------------------

    /**
    * @brief  Enable chip
    */
    inline void ChipEnable(void);
    //*OK

    /**
    * @brief Disable chip
    */
    inline void ChipEnableNot(void);
    //*OK

    /**
    * @brief Select chip
    */
    inline void ChipSelect(void);
    //*OK

    /**
    * @brief Deselect chip
    */
    inline void ChipSelectNot(void);
    //*OK

    /**
    * @brief Write a one word command to the NRF24 chip
    * 
    * @param command Command for the chip
    */
    void WriteCommand(uint8_t command);
    //*OK

    //
    //void write_payload(const void* buf, uint8_t len, const uint8_t writeType);
    //!Not yet implemented function

    //
    //void read_payload(void* buf, uint8_t len);
    //!Not yet implemented function

    //
    //void toggle_features(void);
    //!Not yet implemented function

    //OK
    //void PrintDebug(char* message);
    //!Not yet implemented function

};
#endif