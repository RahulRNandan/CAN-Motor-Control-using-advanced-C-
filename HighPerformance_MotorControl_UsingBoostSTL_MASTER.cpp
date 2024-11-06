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

constexpr uint32_t CAN_ID = 28;               // CAN ID to use for sending commands
constexpr const char* CAN_INTERFACE = "can0"; // CAN interface name

// Function to initialize the CAN socket for sending commands
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

// Function to send a CAN command
void send_command(int socket_fd, const std::array<uint8_t, 8>& command) {
    struct can_frame frame;
    frame.can_id = CAN_ID;
    frame.can_dlc = command.size();
    std::copy(command.begin(), command.end(), frame.data);

    if (write(socket_fd, &frame, sizeof(frame)) != sizeof(frame)) {
        perror("Error sending CAN frame");
    }
}

int main() {
    int socket_fd = initialize_can_socket();
    if (socket_fd < 0) return -1;

    std::array<uint8_t, 8> command;
    
    // Example: Sending Start Command
    command = {'S', 0, 0, 0, 0, 0, 0, '1'};
    send_command(socket_fd, command);
    
    sleep(1);

    // Example: Sending Stop Command
    command = {'S', 0, 0, 0, 0, 0, 0, '0'};
    send_command(socket_fd, command);

    sleep(1);

    // Example: Sending Forward Command with speed
    command = {'F', 0, 0, 0, '0', '5', '0', '0'};
    send_command(socket_fd, command);

    sleep(1);

    // Example: Sending Reverse Command with speed
    command = {'R', 0, 0, 0, '0', '5', '0', '0'};
    send_command(socket_fd, command);

    close(socket_fd);
    return 0;
}

