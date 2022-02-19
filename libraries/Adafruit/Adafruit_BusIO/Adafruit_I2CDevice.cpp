#include "Adafruit_I2CDevice.h"

/*!
 *    @brief  Create an I2C device at a given address
 *    @param  addr The 7-bit I2C address for the device
 *    @param  theWire The I2C bus to use, defaults to &Wire
 */
Adafruit_I2CDevice::Adafruit_I2CDevice(uint8_t addr)
{
    _addr = addr;
    _begun = false;
    _i2c_bus_dev = RT_NULL;
}

// Adafruit_I2CDevice::Adafruit_I2CDevice(uint8_t addr, TwoWire *theWire)
// {
//   Adafruit_I2CDevice(addr);
// }

/*!
 *    @brief  Initializes and does basic address detection
 *    @param  addr_detect Whether we should attempt to detect the I2C address
 * with a scan. 99% of sensors/devices don't mind but once in a while, they spaz
 * on a scan!
 *    @return True if I2C initialized and a device with the addr found
 */
bool Adafruit_I2CDevice::begin(bool addr_detect)
{
    bool rst = true;

    _i2c_bus_dev = (struct rt_i2c_bus_device *)rt_device_find("i2c4"); // TODO
    if(_i2c_bus_dev == RT_NULL)
    {
        return false;
    }

    if(addr_detect)
    {
        rst = detected();
    }

    if(rst == true)
    {
        _begun = true;
        return true;
    }
    else
    {
        return false;
    }
}

/*!
 *    @brief  De-initialize device, turn off the Wire interface
 */
void Adafruit_I2CDevice::end(void)
{
    _i2c_bus_dev = RT_NULL;
    _addr = 0x00;
    _begun = false;
}

/*!
 *    @brief  Scans I2C for the address - note will give a false-positive
 *    if there's no pullups on I2C
 *    @return True if I2C initialized and a device with the addr found
 */
bool Adafruit_I2CDevice::detected(void)
{

    return true;
}

/*!
 *    @brief  Write a buffer or two to the I2C device. Cannot be more than
 * maxBufferSize() bytes.
 *    @param  buffer Pointer to buffer of data to write. This is const to
 *            ensure the content of this buffer doesn't change.
 *    @param  len Number of bytes from buffer to write
 *    @param  prefix_buffer Pointer to optional array of data to write before
 * buffer. Cannot be more than maxBufferSize() bytes. This is const to
 *            ensure the content of this buffer doesn't change.
 *    @param  prefix_len Number of bytes from prefix buffer to write
 *    @param  stop Whether to send an I2C STOP signal on write
 *    @return True if write was successful, otherwise false.
 */
bool Adafruit_I2CDevice::write(const uint8_t *buffer, size_t len, bool stop,
                               const uint8_t *prefix_buffer,
                               size_t prefix_len)
{
    rt_uint16_t flag = RT_NULL;
//    rt_uint8_t *temp_buffer;
//    rt_uint8_t i;

    if(stop == false)
    {
        flag |= RT_I2C_NO_STOP;
    }

//    temp_buffer = (rt_uint8_t*)rt_malloc(prefix_len+len);
//    if(temp_buffer == RT_NULL)
//    {
//        return false;
//    }

    // prefix_buffer: register address 16bit
    // buffer: actual data
//    for(i = 0; i<prefix_len; i++)
//    {
//        temp_buffer[i] = prefix_buffer[i];
//    }
//    rt_memcpy(&temp_buffer[i], buffer, len);
//    rt_i2c_master_send(_i2c_bus_dev, _addr, flag, temp_buffer, prefix_len+len);
//
//    rt_free(temp_buffer);

    // this method is OK
    // 通过RT_I2C_NO_STOP和RT_I2C_NO_START的组合可以将数据一口气传过去
    // RT_I2C_NO_START 不会发送起始位以及芯片地址
    rt_i2c_master_send(_i2c_bus_dev, _addr, flag | RT_I2C_NO_STOP, prefix_buffer, prefix_len);
    rt_i2c_master_send(_i2c_bus_dev, _addr, flag | RT_I2C_NO_START, buffer, len);

    // cannot send like this
//    rt_i2c_master_send(_i2c_bus_dev, _addr, flag, prefix_buffer, prefix_len);
//    rt_i2c_master_send(_i2c_bus_dev, _addr, flag, buffer, len);

    return true;
}

/*!
 *    @brief  Read from I2C into a buffer from the I2C device.
 *    Cannot be more than maxBufferSize() bytes.
 *    @param  buffer Pointer to buffer of data to read into
 *    @param  len Number of bytes from buffer to read.
 *    @param  stop Whether to send an I2C STOP signal on read
 *    @return True if read was successful, otherwise false.
 */
bool Adafruit_I2CDevice::read(uint8_t *buffer, size_t len, bool stop)
{
    rt_uint16_t flag = RT_NULL;

    if(stop == false)
    {
        flag |= RT_I2C_NO_STOP;
    }
    rt_i2c_master_recv(_i2c_bus_dev, _addr, flag, buffer, len);
    return true;
}

/*!
 *    @brief  Write some data, then read some data from I2C into another buffer.
 *    Cannot be more than maxBufferSize() bytes. The buffers can point to
 *    same/overlapping locations.
 *    @param  write_buffer Pointer to buffer of data to write from
 *    @param  write_len Number of bytes from buffer to write.
 *    @param  read_buffer Pointer to buffer of data to read into.
 *    @param  read_len Number of bytes from buffer to read.
 *    @param  stop Whether to send an I2C STOP signal between the write and read
 *    @return True if write & read was successful, otherwise false.
 */
bool Adafruit_I2CDevice::write_then_read(const uint8_t *write_buffer,
                                         size_t write_len, uint8_t *read_buffer,
                                         size_t read_len, bool stop) {
  if (!write(write_buffer, write_len, stop)) {
    return false;
  }

  return read(read_buffer, read_len);
}

/*!
 *    @brief  Returns the 7-bit address of this device
 *    @return The 7-bit address of this device
 */
uint8_t Adafruit_I2CDevice::address(void) { return _addr; }

/*!
 *    @brief  Change the I2C clock speed to desired (relies on
 *    underlying Wire support!
 *    @param desiredclk The desired I2C SCL frequency
 *    @return True if this platform supports changing I2C speed.
 *    Not necessarily that the speed was achieved!
 */
bool Adafruit_I2CDevice::setSpeed(uint32_t desiredclk) {
  (void)desiredclk;
  return false;
}