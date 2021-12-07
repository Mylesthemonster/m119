"""
Connect by BLEDevice
"""
# I've tried using Bleak and BLE-GATT with no luck

import asyncio
import platform
import sys

from bleak import BleakClient, BleakScanner
from bleak.exc import BleakError

import requests
import json
import numpy as np

serv_uuid = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
char_uuid = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

ADDRESS = "c8:c9:a3:d2:fd:3a"

def jprint(obj):
    # create a formatted string of the Python JSON object
    text = json.dumps(obj, indent=4)
    if (text == '[]'):
        print("None\n")
    else:
        print(text)

async def main(ble_address: str):
    str_val = ""
    prev_str_val = ""
    device = await BleakScanner.find_device_by_address(ble_address, timeout=20.0)
    if not device:
        raise BleakError(f"A device with address {ble_address} could not be found.")
    print("Client found!")
    async with BleakClient(device) as client:
        while True:
            val = await client.read_gatt_char(char_specifier=char_uuid)
            str_val = val.decode()
            if not prev_str_val == str_val:
                prev_str_val = str_val
                print("\n\n\n\nBarcode scanned: ", str_val)
                parameters1 = {
                    "app_id": '3856950d',
                    "app_key": '1cc4df43a69b6aa78deea50283406569',
                    "upc": str_val,
                    'nutrition-type': 'logging'
                }

                r1 = requests.get("https://api.edamam.com/api/food-database/v2/parser", params=parameters1)
                json_string1 = r1.json()
                try:
                    parameters2 = {
                        "ingredients": [
                            {
                                "quantity": 1,
                                "measureURI": json_string1['hints'][0]['measures'][0]['uri'],
                                "foodId": json_string1['hints'][0]['food']['foodId']
                            }
                        ]
                    }
                except:
                    print("Product not found in database!")
                    continue

                r2 = requests.post(
                    "https://api.edamam.com/api/food-database/v2/nutrients?app_id=3856950d&app_key=1cc4df43a69b6aa78deea50283406569",
                    json=parameters2)
                json_string2 = r2.json()
                item = json_string2['ingredients'][0]['parsed'][0]['food']
                ingredients = json_string2['ingredients'][0]['parsed'][0]['foodContentsLabel']
                total_nutri = json_string2['totalNutrients']
                daily_nutri = json_string2['totalDaily']
                health_label = json_string2['healthLabels']
                cautions = json_string2['cautions']

                print("ITEM: ", item)

                print('\nHEALTH CAUTIONS:')
                for x in np.array(list(cautions)):
                    print("\t", x)

                print("\nHEALTH LABELS:")
                for x in np.array(list(health_label)):
                    print("\t", x)

                '''
                print("\nINGREDIENTS:\n")
                print(ingredients)
    
                print("\nNUTRITION FACTS\n")
                for nf_type, nf_info in total_nutri.items():
                    print("\nNutritional Abbrivation:", nf_type)
    
                    for key in nf_info:
                        if type(nf_info[key]) is float:
                            print(key + ':', round(nf_info[key]))
                        else:
                            print(key + ':', nf_info[key])
    
                print("\nDAILY NUTRITION FACTS\n")
                for nf_type, nf_info in daily_nutri.items():
                    print("\nNutritional Abbrivation:", nf_type)
    
                    for key in nf_info:
                        if type(nf_info[key]) is float:
                            print(key + ':', round(nf_info[key]))
                        else:
                            print(key + ':', nf_info[key])
                '''


if __name__ == "__main__":
    print("Running")
    asyncio.run(main(sys.argv[1] if len(sys.argv) == 2 else ADDRESS))
