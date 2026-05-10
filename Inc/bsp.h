#ifndef _BSP_H_
#define _BSP_H_

#include "stm32f4xx.h"
#include <stdbool.h>

void bsp_init(void);
bool read_btn(void);

#endif /* BSP_H_ */
