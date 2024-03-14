# rpi_pico_projects
## Build
Move to the project directory and execute the following command:
west build -b rpi_pico
## Flash the board with UF2
1. Press the BOOTSEL button while connecting the board to the PC
2. Drag-and-drop the file <project>/build/zephyr/zephyr.uf2 in the board directory (where <project> is the name of the main directory, for example "rpi_pico_led_blink")
