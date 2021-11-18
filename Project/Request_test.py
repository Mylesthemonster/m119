import requests
import json

""" 
All Health Profiles

alcohol-free	        No alcohol used or contained
immuno-supportive	    Recipes which fit a science-based approach to eating to strengthen the immune system
celery-free	            Does not contain celery or derivatives
crustacean-free	        Does not contain crustaceans (shrimp, lobster etc.) or derivatives
dairy-free	            No dairy; no lactose
egg-free	            No eggs or products containing eggs
fish-free	            No fish or fish derivatives
fodmap-free	            Does not contain FODMAP foods: fermentable oligosaccharides, disaccharides, monosaccharides and polyols, which are short-chain carbohydrates
gluten-free	            No ingredients containing gluten
keto-friendly	        Maximum 7 grams of net carbs per serving
kidney-friendly	        Per serving – phosphorus less than 250 mg AND potassium less than 500 mg AND sodium less than 500 mg
kosher	                Contains only ingredients allowed by the kosher diet. However it does not guarantee kosher preparation of the ingredients themselves
low-potassium	        Less than 150mg per serving
lupine-free	            Does not contain lupine or derivatives
mustard-free	        Does not contain mustard or derivatives
low-fat-abs	            Less than 3g of fat per serving
No-oil-added	        No oil added except to what is contained in the basic ingredients
low-sugar	            No simple sugars – glucose, dextrose, galactose, fructose, sucrose, lactose, maltose
paleo	                Excludes what are perceived to be agricultural products; grains, legumes, dairy products, potatoes, refined salt, refined sugar, and processed oils
peanut-free	            No peanuts or products containing peanuts
pecatarian	            Does not contain meat or meat based products, can contain dairy and fish
pork-free	            Does not contain pork or derivatives
red-meat-free	        Does not contain beef, lamb, pork, duck, goose, game, horse, and other types of red meat or products containing red meat.
sesame-free	            Does not contain sesame seed or derivatives
shellfish-free	        No shellfish or shellfish derivatives
soy-free	            No soy or products containing soy
sugar-conscious	        Less than 4g of sugar per serving
tree-nut-free	        No tree nuts or products containing tree nuts
vegan	                No meat, poultry, fish, dairy, eggs or honey
vegetarian	            No meat, poultry, or fish
wheat-free 	            No wheat, can have gluten though 

Diet Profiles

balanced	        Protein/Fat/Carb values in 15/35/50 ratio
high-fiber	        More than 5g fiber per serving
high-protein	    More than 50% of total calories from proteins
low-carb	        Less than 20% of total calories from carbs
low-fat	            Less than 15% of total calories from fat
low-sodium	        Less than 140mg Na per serving
"""

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
    "upc": '041208010606',
    'nutrition-type': 'logging'
}

r1 = requests.get("https://api.edamam.com/api/food-database/v2/parser", params=parameters1)
json_string1 = r1.json()
# print(response.status_code)
# jprint(json_string1)

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
# jprint(json_string2)
print("ITEM: ", json_string2['ingredients'][0]['parsed'][0]['food'])
print("\nHEALTH Labels\n")
jprint(json_string2['healthLabels'])
print('\nHEALTH CAUTIONS\n')
jprint(json_string2['cautions'])
jprint(json_string2['totalNutrients'])
jprint(json_string2['totalDaily'])
print("ITEM CONTENTS:\n")
jprint(json_string2['ingredients'][0]['parsed'][0]['foodContentsLabel'])