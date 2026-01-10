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

#include "joystick.h"
#include "lf_telemetry.h"

#include <fcntl.h>
#include <fstab.h>
#include <stdio.h>
#include <termio.h>

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    printf("Hello World\n");
    int fd = open("/dev/ttyACM0", O_RDONLY);

    struct termios tio;
    tcgetattr(fd, &tio);
    cfmakeraw(&tio);
    cfsetspeed(&tio, B115200);
    tcsetattr(fd, TCSANOW, &tio);

    struct joystick joy;
    joystick_init(&joy);

    struct lf_telemetry tel;
    lf_telemetry_init(&tel);

    uint8_t abuf[5];
    int idx = 0;

    while (1) {
        uint8_t b;
        if (read(fd, &b, 1) != 1)
            continue;

        // wait for sync
        if (idx == 0 && b != 0xAA)
            continue;

        abuf[idx++] = b;

        if (idx < 5)
            continue;

        // full packet received
        uint8_t checksum = abuf[1] ^ abuf[2] ^ abuf[3];
        if (checksum != abuf[4]) {
            idx = 0; // bad packet, resync
            continue;
        }

        // ✅ VALID PACKET — unpack now
        uint16_t pot_left = (abuf[1] << 2) | (abuf[2] >> 6);
        uint16_t pot_right = ((abuf[2] & 0x3F) << 4) | (abuf[3] >> 4);
        uint8_t flags = abuf[3] & 0x0F;

        int pitch = pot_right * AXIS_MAX * 2 / 1024 + AXIS_MIN;
        int roll = pot_left * AXIS_MAX * 2 / 1024 + AXIS_MIN;

        joystick_write(&joy, AXIS_PITCH, pitch);
        joystick_write(&joy, AXIS_ROLL, roll);

        if (flags & 0x01)
            joystick_write(&joy, AXIS_THROTTLE, AXIS_MAX);
        else if (flags & 0x02)
            joystick_write(&joy, AXIS_THROTTLE, AXIS_MIN);
        else
            joystick_write(&joy, AXIS_THROTTLE, 0);

        if (flags & 0x04)
            joystick_write(&joy, AXIS_YAW, AXIS_MAX);
        else if (flags & 0x08)
            joystick_write(&joy, AXIS_YAW, AXIS_MIN);
        else
            joystick_write(&joy, AXIS_YAW, 0);

        idx = 0; // ready for next packet
        lf_telemetry_packet_t lf_packet;
        int status = lf_telemetry_read(&tel, &lf_packet);
        if (status == 0) {
            // Clear the terminal
            // Source - https://stackoverflow.com/q
            // Posted by Harikant Sharma
            // Retrieved 2026-01-09, License - CC BY-SA 4.0
            printf("\033[2J\033[1;1H");

            printf("Time: %.2f\n", lf_packet.timestamp);
            printf("Pos: (%4.2f %4.2f %4.2f)\n", lf_packet.posX, lf_packet.posY,
                   lf_packet.posZ);
            printf("Vel: (%4.2f %4.2f %4.2f)\n", lf_packet.velX, lf_packet.velY,
                   lf_packet.velZ);
        }
        printf("P: %d R: %d\n", pitch, roll);
    }
    return 0;
    // return lf_telemetry_test();
    // return joystick_test();
}
