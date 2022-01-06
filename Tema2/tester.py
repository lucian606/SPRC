import requests
from requests.api import get

port = 3000
host = f'http://localhost:{port}/api'

countries = [
{
    'nume': 'Poland',
    'lat': 52.22977,
    'lon': 21.01178
},
{
    'nume': 'Romania',
    'lat': 45.94315,
    'lon': 24.96676
},
{
    'nume': 'France',
    'lat': 46.2276,
    'lon': 2.2137
}]

cities = [
    {
        'nume': 'Bucuresti',
        'lat': 44.4267,
        'lon': 26.1025
    },
    {
        'nume': 'Cluj-Napoca',
        'lat': 46.7712,
        'lon': 23.6236
    },
    {
        'nume': 'Warsaw',
        'lat': 52.2297,
        'lon': 21.0117
    },
    {
        'nume': 'Krakow',
        'lat': 50.0647,
        'lon': 19.945
    },
    {
        'nume': 'Lyon',
        'lat': 45.7640,
        'lon': 4.83567
    },
    {
        'nume': 'Paris',
        'lat': 48.8566,
        'lon': 2.3522
    },
]

temperatures = [
    {
        'valoare' : 39.2
    },
    {
        'valoare' : 40.2
    },
    {
        'valoare' : 41.2
    },
    {
        'valoare' : 42.2
    },
    {
        'valoare' : 43.2
    },
    {
        'valoare' : 44.2
    }
]

country_ids = []
city_ids = []
temperature_ids = []

def getCountries():
    url = f'{host}/countries'
    response = requests.get(url)
    return response.json()

def addCountry(country):
    url = f'{host}/countries'
    response = requests.post(url, json=country)
    return response.json()

def getCountry(id):
    url = f'{host}/countries/{id}'
    response = requests.get(url)
    return response.json()

def deleteCountry(id):
    print(f'Deleting country with id: {id}')
    url = f'{host}/countries/{id}'
    response = requests.delete(url)
    return response.json()

def deleteAllCountries():
    for country in getCountries():
        deleteCountry(country['id'])

def getCities():
    url = f'{host}/cities'
    response = requests.get(url)
    return response.json()

def addCity(city):
    url = f'{host}/cities'
    response = requests.post(url, json=city)
    return response.json()

def getCity(id):
    url = f'{host}/cities/{id}'
    response = requests.get(url)
    return response.json()

def deleteCity(id):
    url = f'{host}/cities/{id}'
    response = requests.delete(url)
    return response.json()

def deleteAllCities():
    for city in getCities():
        deleteCity(city['id'])

def getTemperatures():
    url = f'{host}/temperatures'
    response = requests.get(url)
    return response.json()

def addTemperature(temperature):
    url = f'{host}/temperatures'
    response = requests.post(url, json=temperature)
    return response.json()

print('Clearing the DB')
deleteAllCountries()
print(getCountries())
print(getCities())
#print(getTemperatures())
print('Testing:')
for country in countries:
    print(f'Adding {country["nume"]}')
    res = addCountry(country)
    print(res)
    country_ids.append(res['id'])
print(getCountries())
for i in range(len(cities)):
    cities[i]['idTara'] = country_ids[i // 2]
    city = cities[i]
    print(f'Adding {city["nume"]}')
    res = addCity(city)
    print(res)
    city_ids.append(res['id'])
print(getCities())
# for i in range(len(temperatures)):
#     temperatures[i]['id_oras'] = city_ids[i]
#     temperature = temperatures[i]
#     print(f'Adding {temperature["valoare"]}')
#     res = addTemperature(temperature)
#     print(res)
#     temperature_ids.append(res['id'])
# print(getCountries())
# print(getCities())
# print(getTemperatures())