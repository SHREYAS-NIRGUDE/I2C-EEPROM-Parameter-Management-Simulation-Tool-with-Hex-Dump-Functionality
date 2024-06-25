#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

// Define the size of the EEPROM (change this to match your EEPROM size)
#define EEPROM_SIZE 1024

// Define your data types here
typedef char RJS8T;
typedef unsigned char RJU8T;
typedef signed short RJS16T;
typedef unsigned short RJU16T;
typedef signed int RJS32T;
typedef unsigned int RJU32T;

// Define lengths and offsets for your parameters
#define SERIAL_NUMBER_LENGTH 18
#define PCB_SERIAL_NUMBER_LENGTH 18
#define PRODUCT_ID_LENGTH 12
#define MAC_ID_LENGTH 6
#define MAC_ID_COUNT 3

// Define fixed offsets for each parameter
#define SERIAL_NUMBER_OFFSET 0
#define PCB_SERIAL_NUMBER_OFFSET (SERIAL_NUMBER_OFFSET + SERIAL_NUMBER_LENGTH)
#define PRODUCT_ID_OFFSET (PCB_SERIAL_NUMBER_OFFSET + PCB_SERIAL_NUMBER_LENGTH)
#define MAC_ID_OFFSET (PRODUCT_ID_OFFSET + PRODUCT_ID_LENGTH)

// File path for simulating EEPROM data
#define EEPROM_FILE_PATH "eeprom_data.bin"

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

int main(int argc, char *argv[]) {
    int option;
    const struct option long_options[] = {
        {"updSRNUM", required_argument, 0, 's'},
        {"updBSRNUM", required_argument, 0, 'b'},
        {"updPID", required_argument, 0, 'p'},
        {"updMACID", required_argument, 0, 'm'},
        {"clearSRNUM", no_argument, 0, 'c'},
        {"clearBSRNUM", no_argument, 0, 'd'},
        {"clearPID", no_argument, 0, 'e'},
        {"clearMACID", no_argument, 0, 'n'},
        {"updRD", required_argument, 0, 'r'},
        {0, 0, 0, 0}
    };

    // Simulated EEPROM data
    char eepromData[EEPROM_SIZE];
    memset(eepromData, 0, EEPROM_SIZE);

    // Load EEPROM data from file or initialize it if the file doesn't exist
    FILE *eepromFile = fopen(EEPROM_FILE_PATH, "rb");
    if (eepromFile) {
        fread(eepromData, 1, sizeof(eepromData), eepromFile);
        fclose(eepromFile);
    } else {
        // Initialize EEPROM data with zeros
        memset(eepromData, 0, sizeof(eepromData));
    }

    while ((option = getopt_long_only(argc, argv, "s:b:p:m:cdenr:", long_options, NULL)) != -1) {
        switch (option) {
            case 's':
                // Update Serial Number
                if (strlen(optarg) == SERIAL_NUMBER_LENGTH) {
                    strncpy(eepromData + SERIAL_NUMBER_OFFSET, optarg, SERIAL_NUMBER_LENGTH);
                    printf("Serial Number updated successfully: %s\n", optarg);
                    printHexDump(eepromData, sizeof(eepromData));
    
                    // Save EEPROM data to the file
                    eepromFile = fopen(EEPROM_FILE_PATH, "wb");
                    if (eepromFile) {
                        fwrite(eepromData, 1, sizeof(eepromData), eepromFile);
                        fclose(eepromFile);
                    } else {
                        perror("Failed to save EEPROM data");
                    }
                } else {
                    printf("Serial Number has an incorrect length.\n");
                }
                break;
            case 'b':
                // Update Board Serial Number
                if (strlen(optarg) == PCB_SERIAL_NUMBER_LENGTH) {
                    strncpy(eepromData + PCB_SERIAL_NUMBER_OFFSET, optarg, PCB_SERIAL_NUMBER_LENGTH);
                    printf("Board Serial Number updated successfully: %s\n", optarg);
                    printHexDump(eepromData, sizeof(eepromData));
    
                    // Save EEPROM data to the file
                    eepromFile = fopen(EEPROM_FILE_PATH, "wb");
                    if (eepromFile) {
                        fwrite(eepromData, 1, sizeof(eepromData), eepromFile);
                        fclose(eepromFile);
                    } else {
                        perror("Failed to save EEPROM data");
                    }
                } else {
                    printf("Board Serial Number has an incorrect length.\n");
                }
                break;
            case 'p':
                // Update Product ID
                if (strlen(optarg) == PRODUCT_ID_LENGTH) {
                    strncpy(eepromData + PRODUCT_ID_OFFSET, optarg, PRODUCT_ID_LENGTH);
                    printf("Product ID updated successfully: %s\n", optarg);
                    printHexDump(eepromData, sizeof(eepromData));
    
                    // Save EEPROM data to the file
                    eepromFile = fopen(EEPROM_FILE_PATH, "wb");
                    if (eepromFile) {
                        fwrite(eepromData, 1, sizeof(eepromData), eepromFile);
                        fclose(eepromFile);
                    } else {
                        perror("Failed to save EEPROM data");
                    }
                } else {
                    printf("Product ID has an incorrect length.\n");
                }
                break;
            case 'm':
                // Update MAC IDs
                if (optarg[0] >= '0' && optarg[0] <= '9') {
                    int macIdCount = atoi(optarg);
                    if (macIdCount >= 1 && macIdCount <= MAC_ID_COUNT) {
                        char macId[MAC_ID_LENGTH + 1];
                        for (int i = 0; i < macIdCount; i++) {
                            snprintf(macId, sizeof(macId), "A0:FC:72:00:53:%02X", i + 1);
                            strncpy(eepromData + MAC_ID_OFFSET + i * MAC_ID_LENGTH, macId, MAC_ID_LENGTH);
                            printf("MAC ID %d updated successfully: %s\n", i + 1, macId);
                            printHexDump(eepromData, sizeof(eepromData));
                        }
    
                        // Save EEPROM data to the file
                        eepromFile = fopen(EEPROM_FILE_PATH, "wb");
                        if (eepromFile) {
                            fwrite(eepromData, 1, sizeof(eepromData), eepromFile);
                            fclose(eepromFile);
                        } else {
                            perror("Failed to save EEPROM data");
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
                // Save EEPROM data to the file
                eepromFile = fopen(EEPROM_FILE_PATH, "wb");
                if (eepromFile) {
                    fwrite(eepromData, 1, sizeof(eepromData), eepromFile);
                    fclose(eepromFile);
                } else {
                    perror("Failed to save EEPROM data");
                }
                printHexDump(eepromData, sizeof(eepromData));
                break;
            case 'd':
                // Clear Board Serial Number
                memset(eepromData + PCB_SERIAL_NUMBER_OFFSET, 0, PCB_SERIAL_NUMBER_LENGTH);
                printf("Board Serial Number cleared.\n");
                // Save EEPROM data to the file
                eepromFile = fopen(EEPROM_FILE_PATH, "wb");
                if (eepromFile) {
                    fwrite(eepromData, 1, sizeof(eepromData), eepromFile);
                    fclose(eepromFile);
                } else {
                    perror("Failed to save EEPROM data");
                }
                printHexDump(eepromData, sizeof(eepromData));
                break;
            case 'e':
                // Clear Product ID
                memset(eepromData + PRODUCT_ID_OFFSET, 0, PRODUCT_ID_LENGTH);
                printf("Product ID cleared.\n");
                // Save EEPROM data to the file
                eepromFile = fopen(EEPROM_FILE_PATH, "wb");
                if (eepromFile) {
                    fwrite(eepromData, 1, sizeof(eepromData), eepromFile);
                    fclose(eepromFile);
                } else {
                    perror("Failed to save EEPROM data");
                }
                printHexDump(eepromData, sizeof(eepromData));
                break;
            case 'n':
                // Clear MAC IDs
                memset(eepromData + MAC_ID_OFFSET, 0, MAC_ID_COUNT * MAC_ID_LENGTH);
                printf("MAC IDs cleared.\n");
                // Save EEPROM data to the file
                eepromFile = fopen(EEPROM_FILE_PATH, "wb");
                if (eepromFile) {
                    fwrite(eepromData, 1, sizeof(eepromData), eepromFile);
                    fclose(eepromFile);
                } else {
                    perror("Failed to save EEPROM data");
                }
                printHexDump(eepromData, sizeof(eepromData));
                break;
            case 'r':
                // Read parameter
                if (strcmp(optarg, "SRNUM") == 0) {
                    char serialNumber[SERIAL_NUMBER_LENGTH + 1];
                    strncpy(serialNumber, eepromData + SERIAL_NUMBER_OFFSET, SERIAL_NUMBER_LENGTH);
                    serialNumber[SERIAL_NUMBER_LENGTH] = '\0';
                    printf("Serial Number: %s\n", serialNumber);
                } else if (strcmp(optarg, "BSRNUM") == 0) {
                    char boardSerialNumber[PCB_SERIAL_NUMBER_LENGTH + 1];
                    strncpy(boardSerialNumber, eepromData + PCB_SERIAL_NUMBER_OFFSET, PCB_SERIAL_NUMBER_LENGTH);
                    boardSerialNumber[PCB_SERIAL_NUMBER_LENGTH] = '\0';
                    printf("Board Serial Number: %s\n", boardSerialNumber);
                } else if (strcmp(optarg, "PID") == 0) {
                    char productID[PRODUCT_ID_LENGTH + 1];
                    strncpy(productID, eepromData + PRODUCT_ID_OFFSET, PRODUCT_ID_LENGTH);
                    productID[PRODUCT_ID_LENGTH] = '\0';
                    printf("Product ID: %s\n", productID);
                } else if (strcmp(optarg, "MACID") == 0) {
                    for (int i = 0; i < MAC_ID_COUNT; i++) {
                        char macId[MAC_ID_LENGTH + 1];
                        strncpy(macId, eepromData + MAC_ID_OFFSET + i * MAC_ID_LENGTH, MAC_ID_LENGTH);
                        macId[MAC_ID_LENGTH] = '\0';
                        printf("MAC ID %d: %s\n", i + 1, macId);
                    }
                } else {
                    printf("Invalid argument for -updRD.\n");
                }
                break;
            default:
                // Print usage information for unknown options
                printf("Usage: %s --updSRNUM <new_serial_number> --updBSRNUM <new_board_serial_number> --updPID <new_product_id> --updMACID <num_of_macids> --clearSRNUM --clearBSRNUM --clearPID --clearMACID --updRD <parameter>\n", argv[0]);
                break;
        }
    }

    // If no options were specified, print a hex dump of the simulated EEPROM data
    if (argc == 1) {
        printHexDump(eepromData, sizeof(eepromData));
    }

    return 0;
}
