/************************************************
 * @file main.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-12-01
 *
 * @copyright Copyright (c) 2020
 *
 ************************************************/
#include "main.h"
#include "usb_device.h"

int main()
{
#if USER_USE_OS == 0
  HAL_Init();
  uart1_conf(115200);
#endif

  return 0;
}
