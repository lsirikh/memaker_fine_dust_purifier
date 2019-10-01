

#include <stdlib.h>
#include <unistd.h>
#include <peripheral_io.h>
#include <sys/time.h>

#include "log.h"
#include "resource_internal.h"

#define I2C_PIN_MAX 28
/* I2C */
//#define GY30_ADDR (0xA0>>1) /* MBED Addr */
#define ARDUINO_ADDR (0x04) /* Arduino Addr */


static struct {
	int opened;
	peripheral_i2c_h pin_h;
} resource_ai2c_s;

void resource_close_arduino(void)
{

	if (!resource_ai2c_s.opened)
		return;
	_D("1");
	_I("Arduino I2C Communication is finishing...");
	peripheral_i2c_close(resource_ai2c_s.pin_h);
	resource_ai2c_s.opened = 0;
}

//변경자:이기호
int resource_read_arduino(int i2c_bus, int *out_value)
{
	int ret = PERIPHERAL_ERROR_NONE;
	static int write = 0;

	//아두이노로 부터 수신한 데이터를 저장하는 버퍼
	uint8_t buf[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};//read data buffer
	uint8_t cbuf[] = { 0x00, 0x00, 0x00, 0x00 };//command buffer
	uint32_t length = 10; //buffer byte length


	static int count = 0;

	_D("1");

	if (!resource_ai2c_s.opened) {
		ret = peripheral_i2c_open(i2c_bus, ARDUINO_ADDR, &resource_ai2c_s.pin_h);
		_D("2, ret(peripheral_i2c_open)=%d", ret);
		if (ret != PERIPHERAL_ERROR_NONE) {
			_E("i2c open error : %s", get_error_message(ret));
			return -1;
		}
		resource_ai2c_s.opened = 1;
		write = 0;
	}

	ret = peripheral_i2c_read(resource_ai2c_s.pin_h, buf, length);
	_D("5, ret(peripheral_i2c_read)=%d", ret);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("i2c read error : %s", get_error_message(ret));
		return -1;
	}
	_D("READ : 0x%2X, 0x%2X, 0x%2X, 0x%2X, 0x%2X, 0x%2X, 0x%2X, 0x%2X, 0x%2X, 0x%2X, count : %d", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], count++);


	//아두이노로 수신한 데이터를 처리하여 out_value배열에 저장함 추후 value에 값이 됨
	out_value[0] = buf[1];
	out_value[0] = out_value[0]<<8|buf[0];
	out_value[1] = buf[3];
	out_value[1] = out_value[1]<<8|buf[2];
	out_value[2] = buf[5];
	out_value[2] = out_value[2]<<8|buf[4];
	out_value[3] = buf[7];
	out_value[3] = out_value[3]<<8|buf[6];
	out_value[4] = buf[9];
	out_value[4] = out_value[4]<<8|buf[8];
	_D("out_value : %d, %d, %d, %d, %d", out_value[0], out_value[1], out_value[2], out_value[3], out_value[4]);


	return 0;
}

//제작자:이기호
//아두이노로 명령을 보내는 함수
int resource_write_arduino(int i2c_bus, int *input_value)
{
	int ret = PERIPHERAL_ERROR_NONE;
	static int write = 0;

	uint8_t cbuf[] = { 0x00, 0x00, 0x00, 0x00 };//command buffer
	uint32_t length = 10; //buffer byte length


	static int count = 0;

	_D("1");

	if (!resource_ai2c_s.opened) {
		ret = peripheral_i2c_open(i2c_bus, ARDUINO_ADDR, &resource_ai2c_s.pin_h);
		_D("2, ret(peripheral_i2c_open)=%d", ret);
		if (ret != PERIPHERAL_ERROR_NONE) {
			_E("i2c open error : %s", get_error_message(ret));
			return -1;
		}
		resource_ai2c_s.opened = 1;
		write = 0;
	}
	_D("3");

	//2개의 cbuf만 활용하여 데이터 저장
	cbuf[0] = (uint8_t)input_value[0];//set commnand
	cbuf[1] = (uint8_t)input_value[1];//set commnand
	_D("input_value[0], input_value[1] : %d, %d", input_value[0], input_value[1]);
	if (!write) {
		ret = peripheral_i2c_write(resource_ai2c_s.pin_h, cbuf, 2);
		_D("4, ret(peripheral_i2c_write)=%d", ret);
		if (ret != PERIPHERAL_ERROR_NONE) {
			_E("i2c write error : %s", get_error_message(ret));
			return -1;
		}
		//write = 1;
	}



	return 0;
}
