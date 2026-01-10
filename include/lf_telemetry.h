/**
 * @file lf_telemetry.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Liftoff Telemetry
 * @version 0.1
 * @date Created: 2026-01-09
 * @modified Last Modified: 2026-01-09
 *
 * @copyright Copyright (c) 2026
 */

#ifndef _LF_TELEMETRY_H_
#define _LF_TELEMETRY_H_
#include <stdint.h>
#include <unistd.h>

#pragma pack(push, 1) // Critical for network packet alignment
typedef struct {
    float timestamp;              // 4 bytes
    float posX, posY, posZ;       // 12 bytes
    float rotW, rotX, rotY, rotZ; // 16 bytes (Quaternions)
    float velX, velY, velZ;       // 12 bytes
    float gyroP, gyroR, gyroY;    // 12 bytes
    float inT, inY, inP, inR;     // 16 bytes
    float batPct, batVolt;        // 8 bytes
    uint8_t motorCount;           // 1 byte
    float motors[4];              // 16 bytes (for a quad)
} lf_telemetry_packet_t;
#pragma pack(pop)

struct lf_telemetry {
    int sockfd;
};

extern int lf_telemetry_init(struct lf_telemetry *tel);

extern int lf_telemetry_read(struct lf_telemetry *tel,
                             lf_telemetry_packet_t *pkt);

extern int lf_telemetry_test(void);

#endif
