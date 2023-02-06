//#include "../globalInc.h"
#include "NRF24.h"
#include "NRF24_config.h"



//Defined in globalInc.h
//size_t _BV(uint8_t x) { return (1<<x); }

inline void NRF24::ChipSelect() { gpio_put(pinout.csn, _NRF_SPI_CHIPSELECT); }

inline void NRF24::ChipSelectNot() { gpio_put(pinout.csn, _NRF_SPI_CHIPSELECTNOT); }

bool NRF24::IsChipSelect() { return gpio_get(pinout.csn); }

inline void NRF24::ChipEnable() { gpio_put(pinout.ce, _NRF_SPI_CHIPENABLE); }

inline void NRF24::ChipEnableNot() { gpio_put(pinout.ce, _NRF_SPI_CHIPENABLENOT); }

bool NRF24::IsChipEnable() { return gpio_get(pinout.ce); }

void NRF24::FlushRX() { WriteCommand(FLUSH_RX); }

bool NRF24::RX_FIFO_Empty() { return ReadReg(FIFO_STATUS) & 0x01; }

bool NRF24::RX_FIFO_Full() { return (ReadReg(FIFO_STATUS) & 0x02) >> 1; }

void NRF24::FlushTX() { WriteCommand(FLUSH_TX); }

bool NRF24::TX_FIFO_Empty() { return (ReadReg(FIFO_STATUS) & 0x10) >> 4; }

bool NRF24::TX_FIFO_Full() { return (ReadReg(FIFO_STATUS) & 0x20) >> 5; }

bool NRF24::Available() { return !RX_FIFO_Empty(); }

bool NRF24::Available(uint8_t pipe) {
    if (pipe >= 0 && pipe <= 5)
        return ReadReg(RX_PW_P0 + pipe) > 0;
    return false;
}

bool NRF24::ChipAvailable() {
    tx_reg[0] = NOP;
    rx_reg[0] = 0x80;
    ChipSelect();
    spi_write_read_blocking(_NRF_SPI, tx_reg, rx_reg, 1);
    ChipSelectNot();
    return !((rx_reg[0] & 0x80) >> 7);
}

void NRF24::PowerDown() {
    power_state = false;
    WriteReg(CONFIG, (ReadReg(CONFIG)  & ~_BV(PWR_UP)));
}

void NRF24::PowerUp() {
    power_state = true;
    WriteReg(CONFIG, (ReadReg(CONFIG) | _BV(PWR_UP)));
}

bool NRF24::GetPowerState(bool update) {
    if (update)
        power_state = (ReadReg(CONFIG) & 0x02) == 2;
    return power_state;
}

uint8_t NRF24::GetStatusReg(bool update) {
    if (update)
        WriteCommand(NOP);
    return status_reg;
}

uint8_t NRF24::GetPayloadLength() {
    tx_reg[0] = R_RX_PL_WID;
    tx_reg[1] = NOP;
    rx_reg[0] = 0;
    rx_reg[1] = 0;
    ChipEnable();
    spi_write_read_blocking(_NRF_SPI, tx_reg, rx_reg, 2);
    ChipEnableNot();
    status_reg = rx_reg[0];
    return rx_reg[1];
}

uint8_t NRF24::GetPayloadLength(uint8_t pipe) {
    if (pipe >= 0 && pipe <= 5)
        return ReadReg(RX_PW_P0 + pipe);
    return false;
}

bool NRF24::GetHWMode(bool update) {
    if (update)
        prim_rx_mode = ReadReg(CONFIG) && _BV(PRIM_RX);
    return prim_rx_mode;
}

void NRF24::SetPayloadSize(uint8_t size) {
    if (size > 0 && size <= 32)
        payload_size = size;
}

uint8_t NRF24::GetPayloadSize() {
    return payload_size;
}

void NRF24::SetAddressWidth(ADDR_WIDTH width) {
    if (width >= _3B && width <= _5B)
        WriteReg(SETUP_AW, width);
}

ADDR_WIDTH NRF24::GetAddressWidth() {
    return (ADDR_WIDTH)ReadReg(SETUP_AW);
}

void NRF24::SetChannel(uint8_t channel) {
    if (channel >= 0 && channel <= 125)
        WriteReg(RF_CH, channel);
}

uint8_t NRF24::GetChannel() {
    return ReadReg(RF_CH);
}

void NRF24::SetPALevel(PA_LEVEL level) {
    switch (level)
    {
    case _MIN:
        WriteReg(RF_SETUP, ReadReg(RF_SETUP) & 0xF9);
        return;
    case _LOW:
        WriteReg(RF_SETUP, ReadReg(RF_SETUP) & 0xFB);
        return;
    case _HIGH:
        WriteReg(RF_SETUP, ReadReg(RF_SETUP) & 0xFD);
        return;
    case _MAX:
        WriteReg(RF_SETUP, ReadReg(RF_SETUP) | 0x06);
        return;
    default:
        return;
    }
}

PA_LEVEL NRF24::GetPA_Level() {
    return (PA_LEVEL)(ReadReg(RF_SETUP) & 0x06);
}

void NRF24::SetDataRate(DATA_RATE rate) {
    switch (rate)
    {
    case _1M :
        WriteReg(RF_SETUP, ReadReg(RF_SETUP) & 0xD7);
        return;
    case _2M:
        WriteReg(RF_SETUP, ReadReg(RF_SETUP) & 0xDF);
        return;
    case _250K:
        WriteReg(RF_SETUP, ReadReg(RF_SETUP) & 0xF7);
        return;
    default:
        return;
    }
}

DATA_RATE NRF24::GetDataRate() {
    return (DATA_RATE)(ReadReg(RF_SETUP) & 0x28);
}

void NRF24::SetCRC(bool mode) {
    WriteReg(CONFIG, ReadReg(CONFIG) & (mode ? 0xFF : 0xF7));
}

bool NRF24::GetCRC() {
    return (ReadReg(CONFIG) & 0x08) >> 3;
}

void NRF24::SetCRCLength(uint8_t len) {
    switch (len)
    {
    case 0:
        SetCRC(false);
        return;
    case 1:
        WriteReg(CONFIG, ReadReg(CONFIG) & 0xFB);
        return;
    case 2:
        WriteReg(CONFIG, ReadReg(CONFIG) | 0x04);
        return;
    default:
        return;
    }
}

uint8_t NRF24::GetCRCLength() {
    return ((ReadReg(CONFIG) & 0x04) >> 2) + 1;
}

void NRF24::SetAutoAck(bool ack) {
    WriteReg(EN_AA, (ack ? 0x3F : 0x00));
}

uint8_t NRF24::ReadReg(uint8_t reg) {
    tx_reg[0] = R_REGISTER | reg;
    tx_reg[1] = NOP;
    rx_reg[0] = 0;
    rx_reg[1] = 0;
    ChipSelect();
    spi_write_read_blocking(_NRF_SPI, tx_reg, rx_reg, 2);
    ChipSelectNot();
    status_reg = rx_reg[0];
    return rx_reg[1];
}

void NRF24::ReadReg(uint8_t reg, uint8_t* dst, uint8_t len) {
    if (len > 5) return;    
    tx_reg[0] = R_REGISTER | reg;
    for (uint8_t i = 1; i < len + 1; i++)
        tx_reg[i] = NOP;    
    ChipSelect();
    spi_write_read_blocking(_NRF_SPI, tx_reg, rx_reg, (len + 1));
    ChipSelectNot();
    for (uint8_t i = 0; i < len; i++)
        *(dst + i) = rx_reg[i + 1];    
    status_reg = rx_reg[0];
}

void NRF24::WriteReg(uint8_t reg, uint8_t value) {
    tx_reg[0] = W_REGISTER | reg;
    tx_reg[1] = value;
    rx_reg[0] = 0;
    ChipSelect();
    spi_write_read_blocking(_NRF_SPI, tx_reg, rx_reg, 2);
    ChipSelectNot();
    status_reg = rx_reg[0];
}

void NRF24::WriteReg(uint8_t reg, uint8_t* src, uint8_t len) {    
    if (len > 5) return;
    tx_reg[0] = W_REGISTER | reg;
    for (uint8_t i = 0; i < len; i++)
        tx_reg[i+1] = *(src + i);    
    ChipSelect();
    spi_write_read_blocking(_NRF_SPI, tx_reg, rx_reg, (len + 1));
    ChipSelectNot();
    status_reg = rx_reg[0];
}

void NRF24::WriteCommand(uint8_t command) {
    ChipSelect();
    spi_write_read_blocking(_NRF_SPI, &command, &status_reg, 1);
    ChipSelectNot();
}

void NRF24::Init(const uint8_t module_id, const _NRF_SPI_PINOUT pinout) 
{
    printf("NRF24: Initializing...\r\n");

    this->module_id = module_id;
    printf("NRF24: Module ID: %d\r\n", this->module_id);

    this->pinout = pinout;
    //Init and set SPI control pins
    gpio_init(pinout.ce);
    gpio_init(pinout.csn);
    gpio_set_dir(pinout.ce, GPIO_OUT);
    gpio_set_dir(pinout.csn, GPIO_OUT);
    //Set default state
    ChipSelectNot();
    ChipEnableNot();
    //Init SPI data bus
    spi_init(_NRF_SPI, _NRF_SPI_BAUDRATE);
    spi_set_format(_NRF_SPI, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    //Define SPI data pins
#ifdef _NRF_SPI_SCK
    gpio_set_function(_NRF_SPI_SCK, GPIO_FUNC_SPI);
#else
    gpio_set_function(pinout.sck, GPIO_FUNC_SPI);
#endif
#ifdef _NRF_SPI_MOSI
    gpio_set_function(_NRF_SPI_MOSI, GPIO_FUNC_SPI);
#else
    gpio_set_function(pinout.mosi, GPIO_FUNC_SPI);
#endif
#ifdef _NRF_SPI_MISO
    gpio_set_function(_NRF_SPI_MISO, GPIO_FUNC_SPI);
#else
    gpio_set_function(pinout.miso, GPIO_FUNC_SPI);
#endif
    WriteReg(STATUS, ReadReg(STATUS) | _BV(MAX_RT));   //Clear MAX_RT bit
    WriteCommand(NOP);  //Updating the stored status register value
    printf("NRF24: Module %d init OK!\r\n", this->module_id);
}

void NRF24::EraseTempBuffs() {
    for (uint8_t i = 0; i < 2; i++) {
        rx_buff[i] = 0;
        tx_buff[i] = 0;
        rx_reg[i] = 0;
        tx_reg[i] = 0;
    }
    for (uint8_t i = 2; i < 33; i++) {
        rx_buff[i] = 0;
        tx_buff[i] = 0;
    }
}

void NRF24::OpenWritingPipe(uint8_t *address) {
    WriteReg(TX_ADDR, address, 5);
}

void NRF24::CloseWritingPipe() {
    uint8_t reset_addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
    WriteReg(TX_ADDR, reset_addr, 5);
}

void NRF24::OpenReadingPipe(uint8_t pipe, uint8_t *address) {
    if (pipe < 0 || pipe > 5)
        return;
    if (pipe == 0 || pipe == 1)
        WriteReg(RX_ADDR_P0 + pipe, address, 5);
    else
        WriteReg(RX_ADDR_P0 + pipe, address, 1);
    WriteReg(RX_PW_P0 + pipe, payload_size);
    WriteReg(EN_RXADDR, ReadReg(EN_RXADDR) | _BV(pipe));
}

void NRF24::CloseReadingPipe(uint8_t pipe) {
    if (pipe < 0 || pipe > 5)
        return;
    WriteReg(EN_RXADDR, ReadReg(EN_RXADDR) & ~_BV(pipe));
    WriteReg(RX_PW_P0 + pipe, 0);
    switch (pipe)
    {
    case 0:
        {
        uint8_t reset_addr_0[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
        WriteReg(RX_ADDR_P0, reset_addr_0, 5);
        break;
        }
    case 1:
        {
        uint8_t reset_addr_1[] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
        WriteReg(RX_ADDR_P1, reset_addr_1, 5);
        break;
        }
    case 2:
        WriteReg(RX_ADDR_P2, 0xC3);
        break;
    case 3:
        WriteReg(RX_ADDR_P3, 0xC4);
        break;
    case 4:
        WriteReg(RX_ADDR_P4, 0xC5);
        break;
    case 5:
        WriteReg(RX_ADDR_P5, 0xC6);
        break;   
    default:
        break;
    }    
}

void NRF24::SetRXMode(bool power_up) {
    if (!GetPowerState(true) && power_up)
        PowerUp();
    WriteReg(CONFIG, ReadReg(CONFIG) | _BV(PRIM_RX));
    prim_rx_mode = true;
}

void NRF24::SetTXMode(bool power_up) {
    if (!GetPowerState(true) && power_up)
        PowerUp();
    WriteReg(CONFIG, ReadReg(CONFIG) & ~_BV(PRIM_RX));
    prim_rx_mode = false;
}

void NRF24::SetTXHigh() {
    if (!prim_rx_mode)
        ChipEnable();
}

void NRF24::SetTXLow() {
    if (!prim_rx_mode)
        ChipEnableNot();
}

bool NRF24::StartListening() {
    if (!IsListening()){
        ChipEnable();
        return true;
    }
    return false;
}

void NRF24::StopListening() {
    if (IsListening())
        ChipEnableNot();
}

bool NRF24::IsListening() {
    return GetHWMode(true) && IsChipEnable();
}

bool NRF24::Read(uint8_t *dst, uint8_t len) {    
    if (Available() && len > 0 && len <= payload_size) {
        tx_buff[0] = R_RX_PAYLOAD;
        for (uint8_t i = 1; i <= len; i++)
            tx_buff[i] = NOP;
        ChipSelect();
        spi_write_read_blocking(_NRF_SPI, tx_buff, rx_buff, (len + 1));
        ChipSelectNot();
        status_reg = rx_buff[0];
        memcpy(dst, (rx_buff + 1), len);
        WriteReg(STATUS, ReadReg(STATUS) | _BV(RX_DR));
        return true;
    }
    return false;
}

bool NRF24::WriteConst(uint8_t *src, uint8_t len) {
    if (!TX_FIFO_Full() && len > 0 && len <= payload_size) {
        WriteReg(STATUS, ReadReg(STATUS) | _BV(MAX_RT));   //Clear MAX_RT bit
        tx_buff[0] = W_TX_PAYLOAD;
        memcpy((tx_buff + 1), src, len);
        ChipSelect();
        spi_write_read_blocking(_NRF_SPI, tx_buff, rx_buff, (len + 1));
        ChipSelectNot();
        status_reg = rx_buff[0];
        return true;
    }
    return false;
}

bool NRF24::Write(uint8_t *src, uint8_t len) {
    if (!TX_FIFO_Full() && len > 0 && len <= payload_size) {
        WriteReg(STATUS, ReadReg(STATUS) | _BV(MAX_RT));   //Clear MAX_RT bit
        tx_buff[0] = W_TX_PAYLOAD;
        memcpy((tx_buff + 1), src, len);
        ChipSelect();
        spi_write_read_blocking(_NRF_SPI, tx_buff, rx_buff, (len + 1));
        ChipSelectNot();
        ChipEnable();
        sleep_us(20);       //Double the required 10us
        ChipEnableNot();
        sleep_us(130);      //Give time for TX Settling
        status_reg = rx_buff[0];
        return true;
    }
    return false;
}

//  --------------------------------------------------------------------------------
/*      Test Functions  */
//  --------------------------------------------------------------------------------

//
