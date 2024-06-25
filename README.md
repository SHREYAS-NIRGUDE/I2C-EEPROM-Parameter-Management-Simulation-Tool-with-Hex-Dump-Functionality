# I2C-EEPROM-Parameter-Management-Simulation-Tool-with-Hex-Dump-Functionality

This tool is designed to simulate the management of EEPROM parameters, which are typically accessed and modified using the I2C protocol. In a real-world scenario, the tool would interact directly with an EEPROM device over an I2C bus to update and read various parameters. Specifically, for the ODSC 5G board, these parameters include serial numbers, board serial numbers, product IDs, and MAC addresses.

However, for the purpose of this demonstration, the tool operates on a simulated EEPROM data stored in a binary file. This approach allows users to visualize and understand the parameter update process without needing actual EEPROM hardware. The tool supports updating, clearing, and reading specific EEPROM parameters, and provides a hex dump feature to display the contents of the simulated EEPROM memory in a human-readable format.

By using this simulation, developers can ensure that their parameter management logic works correctly before deploying it to a physical EEPROM device. The hex dump functionality helps in verifying the updates and understanding the memory layout, making this tool a valuable resource for development and debugging on the ODSC 5G board.

