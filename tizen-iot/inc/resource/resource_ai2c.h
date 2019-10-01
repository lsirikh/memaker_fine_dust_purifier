

#ifndef __RESOURCE_AI2C_H__
#define __RESOURCE_AI2C_H__

/**
 * @brief Reads the value of i2c bus connected to Arduino board.
 * @param[in] i2c_bus The i2c bus number that the slave device is connected
 * @param[out] out_value The value read by Arduino board
 * @return 0 on success, otherwise a negative error value
 * @see If the i2c bus is not open, creates i2c handle before reading data from the i2c slave device.
 */
extern int resource_read_arduino(int i2c_bus, int *out_value);

extern int resource_write_arduino(int i2c_bus, int *input_value);

#endif /* __RESOURCE_AI2C_H__ */

