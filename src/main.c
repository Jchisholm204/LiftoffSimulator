/**
 * @file main.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief
 * @version 0.1
 * @date Created: 2026-01-09
 * @modified Last Modified: 2026-01-09
 *
 * @copyright Copyright (c) 2026
 */

#include <stdio.h>
#include "joystick.h"
#include "lf_telemetry.h"

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  printf("Hello World\n");
  // return lf_telemetry_test();
  return joystick_test();
}
