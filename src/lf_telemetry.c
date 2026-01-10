/**
 * @file lf_telemetry.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief
 * @version 0.1
 * @date Created: 2026-01-09
 * @modified Last Modified: 2026-01-09
 *
 * @copyright Copyright (c) 2026
 */

#include "lf_telemetry.h"

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 9001

int lf_telemetry_init(struct lf_telemetry *tel) {
    if (!tel) {
        return -1;
    }

    struct sockaddr_in servaddr;
    if ((tel->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation Failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(tel->sockfd, (const struct sockaddr *) &servaddr,
             sizeof(servaddr)) < 0) {
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }
    int buffer_size =
        sizeof(lf_telemetry_packet_t); // Only hold one packet's worth of data
    setsockopt(tel->sockfd, SOL_SOCKET, SO_RCVBUF, &buffer_size,
               sizeof(buffer_size));
    return 0;
}

int lf_telemetry_read(struct lf_telemetry *tel, lf_telemetry_packet_t *pkt) {
    if (!tel || !pkt) {
        return -1;
    }
    // int packets_read = 0;
    // lf_telemetry_packet_t temp_packet;
    // // Keep reading until the buffer is empty
    // while (recvfrom(tel->sockfd, &temp_packet, sizeof(lf_telemetry_packet_t),
    //                 MSG_DONTWAIT, NULL, NULL) > 0) {
    //     // Copy the data to our main packet pointer
    //     memcpy(pkt, &temp_packet, sizeof(lf_telemetry_packet_t));
    //     packets_read++;
    // }
    //
    // if (packets_read > 0) {
    //     return 1; // We found at least one new packet and 'packet' now holds the
    //               // newest
    // }
    //
    // return 0; // No new data
    ssize_t n = recvfrom(tel->sockfd, pkt, sizeof(lf_telemetry_packet_t),
                         MSG_DONTWAIT, NULL, NULL);

    if (n < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return 1;
        }
        return -1;
    }
    if ((size_t) n < sizeof(lf_telemetry_packet_t)) {
        return -2;
    }
    return 0;
}

int lf_telemetry_test(void) {
    struct lf_telemetry lft;
    lf_telemetry_init(&lft);

    while (1) {
        lf_telemetry_packet_t lf_packet;
        int status = lf_telemetry_read(&lft, &lf_packet);
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
        } else {
            // printf("No Packet\n");
            usleep(1000);
        }
    }

    return 0;
}
