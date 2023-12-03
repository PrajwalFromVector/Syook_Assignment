#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

// Function to parse accelerometer data from BLE packet
void parse_accelerometer_data(char *raw_packet) {
    // Assuming accelerometer data format: X (2 bytes) + Y (2 bytes) + Z (2 bytes)
    int16_t accel_x, accel_y, accel_z;

    // Extract accelerometer data from the raw packet
    memcpy(&accel_x, raw_packet + 26, 2);
    memcpy(&accel_y, raw_packet + 28, 2);
    memcpy(&accel_z, raw_packet + 30, 2);

    // Use the accelerometer data as needed
    printf("Accelerometer Data: X=%d, Y=%d, Z=%d\n", accel_x, accel_y, accel_z);
}

// Function to determine whether the tag is moving or stationary
int is_moving(double acceleration_threshold) {
    // Implement your logic to decide the movement status
    // Use acceleration_threshold to make the determination
    // Placeholder logic - assuming movement if any acceleration is above the threshold
    return (acceleration_threshold > 0.0);
}

int main() {
    // Set the MAC address of the BLE beacon
    char target_device_address[] = "00:11:22:33:44:55";

    struct sockaddr_rc addr = {0};
    int s, status;
    char buf[1024] = {0};

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba(target_device_address, &addr.rc_bdaddr);

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    if (status == 0) {
        // Connection established
        while (1) {
            // Receive raw BLE packet from the BLE beacon
            status = read(s, buf, sizeof(buf));

            if (status > 0) {
                // Parse accelerometer data from the raw packet
                parse_accelerometer_data(buf);

                // Set an appropriate acceleration threshold based on your observations
                double acceleration_threshold = 0.5;

                // Determine whether the tag is moving or stationary
                if (is_moving(acceleration_threshold)) {
                    printf("Tag is moving\n");
                } else {
                    printf("Tag is stationary\n");
                }
            }
        }
    } else {
        fprintf(stderr, "Error: Unable to connect to the device.\n");
    }

    close(s);
    return 0;
}
