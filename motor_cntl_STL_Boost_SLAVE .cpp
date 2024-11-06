#include <iostream>
#include <cstring>
#include <array>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>

constexpr uint32_t CAN_ID = 28;               // Expected CAN ID for incoming commands
constexpr const char* CAN_INTERFACE = "can0"; // CAN interface name

// Function to initialize the CAN socket for receiving commands
int initialize_can_socket() {
    int socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_fd < 0) {
        perror("Error creating CAN socket");
        return -1;
    }

    struct ifreq ifr;
    strcpy(ifr.ifr_name, CAN_INTERFACE);
    ioctl(socket_fd, SIOCGIFINDEX, &ifr);

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error binding CAN socket");
        return -1;
    }

    return socket_fd;
}

// Function to process the received CAN command and control the motor
void process_command(const std::array<uint8_t, 8>& command) {
    if (command[0] == 'S') {
        if (command[7] == '1') {
            std::cout << "Motor Start Command Received\n";
            // Insert motor start code here
        } else if (command[7] == '0') {
            std::cout << "Motor Stop Command Received\n";
            // Insert motor stop code here
        }
    } else if (command[0] == 'F') {
        std::cout << "Forward Command Received\n";
        // Insert motor forward code here
    } else if (command[0] == 'R') {
        std::cout << "Reverse Command Received\n";
        // Insert motor reverse code here
    } else {
        std::cout << "Unknown Command\n";
    }
}

int main() {
    int socket_fd = initialize_can_socket();
    if (socket_fd < 0) return -1;

    struct can_frame frame;

    while (true) {
        int nbytes = read(socket_fd, &frame, sizeof(frame));
        if (nbytes < 0) {
            perror("Error reading CAN frame");
            break;
        }

        if (frame.can_id == CAN_ID) {
            std::array<uint8_t, 8> command_data;
            std::copy(std::begin(frame.data), std::begin(frame.data) + frame.can_dlc, command_data.begin());

            // Process the received command
            process_command(command_data);
        }
    }

    close(socket_fd);
    return 0;
}

