

#ifndef __RESOURCE_INTERNAL_H__
#define __RESOURCE_INTERNAL_H__

#include <peripheral_io.h>

#include <resource/resource_ai2c_internal.h>

#define PIN_MAX 40

struct _resource_ai2c {
	int opened;
	peripheral_gpio_h pin_h;
	void (*close) (int);
};
typedef struct _resource_ai2c resource_ai2c;

typedef void (*resource_read_cb)(double value, void *data);

struct _resource_read_cb_s {
	resource_read_cb cb;
	void *data;
	int pin_num;
};
typedef struct _resource_read_cb_s resource_read_s;

extern resource_ai2c *resource_get_info(int pin_num);
extern void resource_close_all(void);

#endif /* __RESOURCE_INTERNAL_H__ */
