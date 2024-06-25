### README for EEPROM Parameter Management Tool

This tool is designed to manage EEPROM parameters such as serial numbers, board serial numbers, product IDs, and MAC addresses. The EEPROM data is simulated in a binary file for demonstration purposes. The tool allows updating, clearing, and reading specific parameters and provides a hex dump feature to display the EEPROM data.

#### Command Usage

The following commands can be used to update, clear, and read parameters in the simulated EEPROM data.

#### Updating Parameters

1. **Update Serial Number**

   ```sh
   ./eeprom_tool --updSRNUM <new_serial_number>
   ```

   - `<new_serial_number>`: Must be exactly 18 characters long.
   - Example: `./eeprom_tool --updSRNUM 123456789012345678`

2. **Update Board Serial Number**

   ```sh
   ./eeprom_tool --updBSRNUM <new_board_serial_number>
   ```

   - `<new_board_serial_number>`: Must be exactly 18 characters long.
   - Example: `./eeprom_tool --updBSRNUM ABCDEFGHIJKLMNOPQR`

3. **Update Product ID**

   ```sh
   ./eeprom_tool --updPID <new_product_id>
   ```

   - `<new_product_id>`: Must be exactly 12 characters long.
   - Example: `./eeprom_tool --updPID PROD12345678`

4. **Update MAC IDs**

   ```sh
   ./eeprom_tool --updMACID <num_of_macids>
   ```

   - `<num_of_macids>`: Must be a number between 1 and 3.
   - Example: `./eeprom_tool --updMACID 2`

#### Clearing Parameters

1. **Clear Serial Number**

   ```sh
   ./eeprom_tool --clearSRNUM
   ```

2. **Clear Board Serial Number**

   ```sh
   ./eeprom_tool --clearBSRNUM
   ```

3. **Clear Product ID**

   ```sh
   ./eeprom_tool --clearPID
   ```

4. **Clear MAC IDs**

   ```sh
   ./eeprom_tool --clearMACID
   ```

#### Reading Parameters

1. **Read Serial Number**

   ```sh
   ./eeprom_tool --updRD SRNUM
   ```

2. **Read Board Serial Number**

   ```sh
   ./eeprom_tool --updRD BSRNUM
   ```

3. **Read Product ID**

   ```sh
   ./eeprom_tool --updRD PID
   ```

4. **Read MAC IDs**

   ```sh
   ./eeprom_tool --updRD MACID
   ```

#### Hex Dump of EEPROM Data

If no options are specified, the tool will print a hex dump of the entire EEPROM data.

```sh
./eeprom_tool
```

#### Example Commands

- Update the serial number:
  
  ```sh
  ./eeprom_tool --updSRNUM 123456789012345678
  ```

- Clear the board serial number:
  
  ```sh
  ./eeprom_tool --clearBSRNUM
  ```

- Read the product ID:
  
  ```sh
  ./eeprom_tool --updRD PID
  ```

- Print a hex dump of the EEPROM data:
  
  ```sh
  ./eeprom_tool
  ```

#### Notes

- Ensure that the EEPROM data file `eeprom_data.bin` exists in the same directory as the tool. If the file does not exist, it will be created and initialized with zeros.
- The length of the serial number, board serial number, and product ID must match the specified lengths (18 characters for serial numbers, 12 characters for product ID).
- The number of MAC IDs to be updated must be between 1 and 3. The MAC IDs will be generated and stored sequentially.
