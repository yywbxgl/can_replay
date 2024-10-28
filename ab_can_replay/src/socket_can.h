#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

class SocketCan {
public:

    SocketCan() {
        socket_fd = -1;
        memset(&ifr, 0, sizeof(ifr));
        memset(&addr, 0, sizeof(addr));
    }

    ~SocketCan() {
        if (socket_fd > 0) close(socket_fd);
    }

    bool init(const std::string& can_interface) {
        if ((socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
            perror("Error while opening socket");
            return false;
        }

        strncpy(ifr.ifr_name, can_interface.c_str(), IFNAMSIZ - 1);
        ifr.ifr_name[IFNAMSIZ - 1] = '\0';

        if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0) {
            perror("Failed to get CAN interface index. Error in ioctl");
            return false;
        }

        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;

        if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("Failed to bind CAN socket. Error in socket bind");
            return false;
        }

        return true;
    }



    bool writeFrame(const can_frame& frame) {
        if (write(socket_fd, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
            perror("Error in writing to CAN socket");
            return false;
        }
        return true;
    }

    bool readFrame(can_frame& frame) {
        ssize_t nbytes = read(socket_fd, &frame, sizeof(struct can_frame));
        if (nbytes < 0) {
            perror("Error in reading from CAN socket");
            return false;
        }
        if ((unsigned long)nbytes < sizeof(struct can_frame)) {
            std::cerr << "Incomplete CAN frame read" << std::endl;
            return false;
        }
        return true;
    }

private:
    int socket_fd;
    struct ifreq ifr;
    struct sockaddr_can addr;
};

