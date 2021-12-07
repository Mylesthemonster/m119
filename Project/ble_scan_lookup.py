"""
Connect by BLEDevice
"""
# I've tried using Bleak and BLE-GATT with no luck

import asyncio
import platform
import sys

from bleak import BleakClient, BleakScanner
from bleak.exc import BleakError

serv_uuid = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
char_uuid = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

ADDRESS = "c8:c9:a3:d2:fd:3a"

async def main(ble_address: str):
    str_val = ""
    prev_str_val = ""
    device = await BleakScanner.find_device_by_address(ble_address, timeout=20.0)
    if not device:
        raise BleakError(f"A device with address {ble_address} could not be found.")
    print("Client found!")
    async with BleakClient(device) as client:
        svcs = await client.get_services()
        print("Services:")
        for service in svcs:
            print(service)
            if service.uuid == serv_uuid:
                print("Desired service UUID found")
                while True:
                    val = await client.read_gatt_char(char_specifier=char_uuid)
                    str_val = val.decode()
                    if not prev_str_val == str_val:
                        prev_str_val = str_val
                        print(str_val)


if __name__ == "__main__":
    print("Running")
    asyncio.run(main(sys.argv[1] if len(sys.argv) == 2 else ADDRESS))