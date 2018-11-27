
python esptool.py -p /dev/ttyS0 --baud 115200 write_flash --flash_size=2MB -fm dio 0x00000 "/home/supla/Desktop/ESP8266-Firmware/coal_module_wroom_2048_eagle.flash.bin" 0x40000 "/home/supla/Desktop/ESP8266-Firmware/coal_module_wroom_2048_eagle.irom0text.bin" 
