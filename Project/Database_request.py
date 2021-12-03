import requests
import json
import numpy as np

def jprint(obj):
    # create a formatted string of the Python JSON object
    text = json.dumps(obj, indent=4)
    if( text == '[]'):
        print("None\n")
    else:
        print(text)

parameters1 = {
    "app_id": '3856950d',
    "app_key": '1cc4df43a69b6aa78deea50283406569',
    "upc": '028400337298',
    'nutrition-type': 'logging'
}

r1 = requests.get("https://api.edamam.com/api/food-database/v2/parser", params=parameters1)
json_string1 = r1.json()

parameters2 = {
    "ingredients":[
    {
      "quantity": 1,
      "measureURI": json_string1['hints'][0]['measures'][0]['uri'],
      "foodId": json_string1['hints'][0]['food']['foodId']
    }
  ]
}

r2 = requests.post("https://api.edamam.com/api/food-database/v2/nutrients?app_id=3856950d&app_key=1cc4df43a69b6aa78deea50283406569", json=parameters2)
json_string2 = r2.json()
item = json_string2['ingredients'][0]['parsed'][0]['food']
ingredients = json_string2['ingredients'][0]['parsed'][0]['foodContentsLabel']
total_nutri = json_string2['totalNutrients']
daily_nutri = json_string2['totalDaily']
health_label = json_string2['healthLabels']
cautions = json_string2['cautions']

print("ITEM: ", item)

print("\nHEALTH Labels\n")
for x in np.array(list(health_label)):
  print(x)

print('\nHEALTH CAUTIONS\n')
for x in np.array(list(cautions)):
  print(x)

print("\nINGREDIENTS:\n")
print(ingredients)

print ("\nNUTRITION FACTS\n")
for nf_type, nf_info in total_nutri.items():
    print("\nNutritional Abbrivation:", nf_type)
    
    for key in nf_info:
        if type(nf_info[key]) is float:
          print(key + ':', round(nf_info[key]))
        else:
          print(key + ':', nf_info[key])

print ("\nDAILY NUTRITION FACTS\n")
for nf_type, nf_info in daily_nutri.items():
    print("\nNutritional Abbrivation:", nf_type)
    
    for key in nf_info:
      if type(nf_info[key]) is float:
          print(key + ':', round(nf_info[key]))
      else:
        print(key + ':', nf_info[key])




