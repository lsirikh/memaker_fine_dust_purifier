/*****************************************************************
 *
 * Copyright (c) 2019 TEAM MEMAKER. All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef __RESOURCE_AI2C_H__
#define __RESOURCE_AI2C_H__

/**
 * @brief Reads the value of i2c bus connected to Arduino board.
 * @param[in] i2c_bus The i2c bus number that the slave device is connected
 * @param[out] out_value The value read by Arduino board
 * @return 0 on success, otherwise a negative error value
 * @see If the i2c bus is not open, creates i2c handle before reading data from the i2c slave device.
 */
extern int resource_read_arduino(int i2c_bus, uint16_t *out_value, int param_num);

extern int resource_write_arduino(int i2c_bus, uint8_t *input_value, int param_num);

#endif /* __RESOURCE_AI2C_H__ */

