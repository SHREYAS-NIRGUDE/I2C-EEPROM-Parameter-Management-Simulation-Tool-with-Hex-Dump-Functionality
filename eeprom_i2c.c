#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

// Define your data types here
typedef char RJS8T;
typedef unsigned char RJU8T;
typedef signed short RJS16T;
typedef unsigned short RJU16T;
typedef signed int RJS32T;
typedef unsigned int RJU32T;

// Define lengths for your parameters
#define SERIAL_NUMBER_LENGTH 18
#define PCB_SERIAL_NUMBER_LEN 18
#define PRODUCT_ID_LEN 12
#define MAC_ID_LEN 6
#define MAC_ID_COUNT 3

// Define fixed offsets for each parameter
#define SERIAL_NUMBER_OFFSET 100
#define PCB_SERIAL_NUMBER_OFFSET (SERIAL_NUMBER_OFFSET + SERIAL_NUMBER_LENGTH)
#define PRODUCT_ID_OFFSET (PCB_SERIAL_NUMBER_OFFSET + PCB_SERIAL_NUMBER_LEN)
#define MAC_ID_OFFSET (PRODUCT_ID_OFFSET + PRODUCT_ID_LEN)

// I2C Configuration
#define EEPROM_I2C_ADDRESS 0x50
#define I2C_BUS "/dev/i2c-1"

// Function to print a hex dump of EEPROM data with addresses and characters
void printHexDump(const char *data, int length) {
    printf("EEPROM Hex Dump:\n");

    for (int i = 0; i < length; i++) {
        if (i % 16 == 0) {
            if (i > 0) {
                printf("  ");
                for (int j = i - 16; j < i; j++) {
                    if (j >= 0) {
                        char c = data[j];
                        if (c >= 32 && c <= 126) {
                            putchar(c);
                        } else {
                            putchar('.');
                        }
                    }
                }
                printf("\n");
            }
            printf("0x%04X: ", i);
        }
        printf("%02X ", (unsigned char)data[i]);
    }

    printf("\n");
}

// Function to write data to EEPROM
int writeDataToEEPROM(int file, unsigned int address, const char *data, int dataSize) {
    unsigned char buffer[dataSize + 2];

    // Set the EEPROM memory address
    buffer[0] = (address >> 8) & 0xFF; // High byte
    buffer[1] = address & 0xFF;        // Low byte

    // Copy data to the buffer
    memcpy(&buffer[2], data, dataSize);

    // Write data to EEPROM
    if (write(file, buffer, dataSize + 2) != dataSize + 2) {
        perror("Write failed");
        return -1;
    }
    return 0;
}

// Function to read data from EEPROM
int readDataFromEEPROM(int file, unsigned int address, char *data, int dataSize) {
    unsigned char buffer[2];

    // Set the EEPROM memory address
    buffer[0] = (address >> 8) & 0xFF; // High byte
    buffer[1] = address & 0xFF;        // Low byte

    // Write the address to the EEPROM
    if (write(file, buffer, 2) != 2) {
        perror("Write failed");
        return -1;
    }

    // Read data from EEPROM
    if (read(file, data, dataSize) != dataSize) {
        perror("Read failed");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int option;
    const struct option long_options[] = {
        {"updSRNUM", required_argument, 0, 's'},
        {"updBSRNUM", required_argument, 0, 'b'},
        {"updMACID", required_argument, 0, 'm'},
        {"clearSRNUM", no_argument, 0, 'c'},
        {"clearBSRNUM", no_argument, 0, 'd'},
        {"clearMACID", no_argument, 0, 'n'},
        {"updRD", required_argument, 0, 'r'},
        {0, 0, 0, 0}
    };

    // Open the I2C bus
    int i2cFile = open(I2C_BUS, O_RDWR);
    if (i2cFile < 0) {
        perror("Failed to open the I2C bus");
        return 1;
    }

    // Set the I2C address of the EEPROM
    if (ioctl(i2cFile, I2C_SLAVE, EEPROM_I2C_ADDRESS) < 0) {
        perror("Failed to acquire bus access and/or talk to slave");
        return 1;
    }

    char eepromData[EEPROM_SIZE];

    while ((option = getopt_long_only(argc, argv, "s:b:m:cdnr:", long_options, NULL)) != -1) {
        switch (option) {
            case 's':
                // Update Serial Number
                if (strlen(optarg) == SERIAL_NUMBER_LENGTH) {
                    strncpy(eepromData + SERIAL_NUMBER_OFFSET, optarg, SERIAL_NUMBER_LENGTH);
                    printf("Serial Number updated successfully: %s\n", optarg);
                    printHexDump(eepromData, sizeof(eepromData));

                    // Write updated data to EEPROM
                    if (writeDataToEEPROM(i2cFile, SERIAL_NUMBER_OFFSET, eepromData + SERIAL_NUMBER_OFFSET, SERIAL_NUMBER_LENGTH) == 0) {
                        printf("Data written to EEPROM.\n");
                    }
                } else {
                    printf("Serial Number has an incorrect length.\n");
                }
                break;
            case 'b':
                // Update Board Serial Number
                if (strlen(optarg) == PCB_SERIAL_NUMBER_LEN) {
                    strncpy(eepromData + PCB_SERIAL_NUMBER_OFFSET, optarg, PCB_SERIAL_NUMBER_LEN);
                    printf("Board Serial Number updated successfully: %s\n", optarg);
                    printHexDump(eepromData, sizeof(eepromData));

                    // Write updated data to EEPROM
                    if (writeDataToEEPROM(i2cFile, PCB_SERIAL_NUMBER_OFFSET, eepromData + PCB_SERIAL_NUMBER_OFFSET, PCB_SERIAL_NUMBER_LEN) == 0) {
                        printf("Data written to EEPROM.\n");
                    }
                } else {
                    printf("Board Serial Number has an incorrect length.\n");
                }
                break;
            case 'm':
                // Update MAC IDs
                if (optarg[0] >= '0' && optarg[0] <= '9') {
                    int macIdCount = atoi(optarg);
                    if (macIdCount >= 1 && macIdCount <= MAC_ID_COUNT) {
                        char macId[MAC_ID_LEN + 1];
                        for (int i = 0; i < macIdCount; i++) {
                            snprintf(macId, sizeof(macId), "A0:FC:72:00:53:%02X", i + 1);
                            strncpy(eepromData + MAC_ID_OFFSET + i * MAC_ID_LEN, macId, MAC_ID_LEN);
                            printf("MAC ID %d updated successfully: %s\n", i + 1, macId);
                            printHexDump(eepromData, sizeof(eepromData));
                        }

                        // Write updated data to EEPROM
                        if (writeDataToEEPROM(i2cFile, MAC_ID_OFFSET, eepromData + MAC_ID_OFFSET, MAC_ID_COUNT * MAC_ID_LEN) == 0) {
                            printf("Data written to EEPROM.\n");
                        }
                    } else {
                        printf("Invalid number of MAC IDs. Must be between 1 and %d.\n", MAC_ID_COUNT);
                    }
                } else {
                    printf("Invalid argument for -updMACID.\n");
                }
                break;
            case 'c':
                // Clear Serial Number
                memset(eepromData + SERIAL_NUMBER_OFFSET, 0, SERIAL_NUMBER_LENGTH);
                printf("Serial Number cleared.\n");

                // Write cleared data to EEPROM
                if (writeDataToEEPROM(i2cFile, SERIAL_NUMBER_OFFSET, eepromData + SERIAL_NUMBER_OFFSET, SERIAL_NUMBER_LENGTH) == 0) {
                    printf("Data written to EEPROM.\n");
                }
                printHexDump(eepromData, sizeof(eepromData));
                break;
            case 'd':
                // Clear Board Serial Number
                memset(eepromData + PCB_SERIAL_NUMBER_OFFSET, 0, PCB_SERIAL_NUMBER_LEN);
                printf("Board Serial Number cleared.\n");

                // Write cleared data to EEPROM
                if (writeDataToEEPROM(i2cFile, PCB_SERIAL_NUMBER_OFFSET, eepromData + PCB_SERIAL_NUMBER_OFFSET, PCB_SERIAL_NUMBER_LEN) == 0) {
                    printf("Data written to EEPROM.\n");
                }
                break;
            case 'n':
                // Clear MAC IDs
                memset(eepromData + MAC_ID_OFFSET, 0, MAC_ID_COUNT * MAC_ID_LEN);
                printf("MAC IDs cleared.\n");

                // Write cleared data to EEPROM
                if (writeDataToEEPROM(i2cFile, MAC_ID_OFFSET, eepromData + MAC_ID_OFFSET, MAC_ID_COUNT * MAC_ID_LEN) == 0) {
                    printf("Data written to EEPROM.\n");
                }
                break;
            case 'r':
                // Read parameter
                if (strcmp(optarg, "SRNUM") == 0) {
                    // Read data from EEPROM
                    if (readDataFromEEPROM(i2cFile, SERIAL_NUMBER_OFFSET, eepromData + SERIAL_NUMBER_OFFSET, SERIAL_NUMBER_LENGTH) == 0) {
                        printf("Serial Number: %s\n", eepromData + SERIAL_NUMBER_OFFSET);
                    }
                } else if (strcmp(optarg, "BSRNUM") == 0) {
                    // Read data from EEPROM
                    if (readDataFromEEPROM(i2cFile, PCB_SERIAL_NUMBER_OFFSET, eepromData + PCB_SERIAL_NUMBER_OFFSET, PCB_SERIAL_NUMBER_LEN) == 0) {
                        printf("Board Serial Number: %s\n", eepromData + PCB_SERIAL_NUMBER_OFFSET);
                    }
                } else if (strcmp(optarg, "MACID") == 0) {
                    // Read data from EEPROM
                    if (readDataFromEEPROM(i2cFile, MAC_ID_OFFSET, eepromData + MAC_ID_OFFSET, MAC_ID_COUNT * MAC_ID_LEN) == 0) {
                        for (int i = 0; i < MAC_ID_COUNT; i++) {
                            char macId[MAC_ID_LEN + 1];
                            strncpy(macId, eepromData + MAC_ID_OFFSET + i * MAC_ID_LEN, MAC_ID_LEN);
                            macId[MAC_ID_LEN] = '\0';
                            printf("MAC ID %d: %s\n", i + 1, macId);
                        }
                    }
                } else {
                    printf("Invalid argument for -updRD.\n");
                }
                break;
            default:
                // Print usage information for unknown options
                printf("Usage: %s --updSRNUM <new_serial_number> --updBSRNUM <new_board_serial_number> --updMACID <num_of_macids> --clearSRNUM --clearBSRNUM --clearMACID --updRD <parameter>\n", argv[0]);
                break;
        }
    }

    // Close the I2C bus
    close(i2cFile);

    return 0;
}
