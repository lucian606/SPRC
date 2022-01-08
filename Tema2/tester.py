import requests
from requests.api import get

port = 3000
host = f'http://localhost:{port}/api'

countries = [
{
    'nume': 'Poland',
    'lat': 52.23,
    'lon': 21.01
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

new_temperatures = [
    {
        'valoare' : 69.2
    },
    {
        'valoare' : 69.2
    },
    {
        'valoare' : 69.2
    },
    {
        'valoare' : 69.2
    },
    {
        'valoare' : 69.2
    },
    {
        'valoare' : 69.2
    },
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
    print(f'Adding {city["nume"]}')
    url = f'{host}/cities'
    response = requests.post(url, json=city)
    return response.json()

def getCity(id):
    url = f'{host}/cities/{id}'
    response = requests.get(url)
    return response.json()

def getCountryCities(id):
    url = f'{host}/cities/country/{id}'
    response = requests.get(url)
    return response.json()

def deleteCity(id):
    print(f'Deleting city with id: {id}')
    url = f'{host}/cities/{id}'
    response = requests.delete(url)
    return response.json()

def updateCity(id, city):
    print(f'Updating city with id: {id}')
    url = f'{host}/cities/{id}'
    response = requests.put(url, json=city)
    return response.json()

def deleteAllCities():
    for city in getCities():
        deleteCity(city['id'])

def getTemperatures():
    url = f'{host}/temperatures?from=2012-01-01&until=2022-12-31&lat=44.4267'
    response = requests.get(url)
    return response.json()

def getCityTemperatures(id):
    url = f'{host}/temperatures/cities/{id}'
    response = requests.get(url)
    return response.json()

def getCountryTemperatures(id):
    url = f'{host}/temperatures/countries/{id}'
    response = requests.get(url)
    return response.json()

def addTemperature(temperature):
    print(f'Adding {temperature["valoare"]}')
    url = f'{host}/temperatures'
    response = requests.post(url, json=temperature)
    return response.json()

def deleteTemperature(id):
    print(f'Deleting temperature with id: {id}')
    url = f'{host}/temperatures/{id}'
    response = requests.delete(url)
    return response.json()

def deleteAllTemperatures():
    for temperature in getTemperatures():
        deleteTemperature(temperature['id'])

def updateTemperature(id, temperature):
    print(f'Updating temperature with id: {id}')
    url = f'{host}/temperatures/{id}'
    response = requests.put(url, json=temperature)
    return response.json()

def runFirstTest():
    print('Clearing the DB')
    deleteAllCountries()
    deleteAllCities()
    deleteAllTemperatures()
    print(getCountries())
    for country in countries:
        print(f'Adding {country["nume"]}')
        res = addCountry(country)
        print(res)
        country_ids.append(res['id'])
    print(getCountries())
    for i in range(len(cities)):
        cities[i]['idTara'] = country_ids[i // 2]
        city = cities[i]
        res = addCity(city)
        print(res)
        city_ids.append(res['id'])
    print(getCities())
    for i in range(len(temperatures)):
        temperatures[i]['id_oras'] = city_ids[i]
        if (i % 2):
            temperatures[i]['timestamp'] = i
        temperature = temperatures[i]
        res = addTemperature(temperature)
        print(res)
        temperature_ids.append(res['id'])
    #print(getCities())
    #print(getCityTemperatures(city_ids[0]))
    #print(getCountryCities(country_ids[0]))
    #print(getCountryTemperatures(country_ids[0]))
    #print(getTemperatures())
    # deleteAllCities()
    deleteAllCountries()
    # print(getCountries())
    # print(getCities())
    # print(getTemperatures())

def runSecondTest():
    print('Clearing the DB')
    deleteAllCountries()
    deleteAllCities()
    deleteAllTemperatures()
    print(getCountries())
    print(getCities())
    print(getTemperatures())
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
        res = addCity(city)
        print(res)
        city_ids.append(res['id'])
    print(getCities())
    print("Update")
    for i in range(len(city_ids)):
        res = updateCity(city_ids[i], {'idTara' : country_ids[0]})
        print(res)
    for country in country_ids:
        res = getCountryCities(country)
        print(res)
    print(getCities())
    deleteAllCountries()

def runThirdTest():
    print('Clearing the DB')
    deleteAllCountries()
    deleteAllCities()
    deleteAllTemperatures()
    print(getCountries())
    print(getCities())
    print(getTemperatures())
    print('Testing:')
    for country in countries:
        print(f'Adding {country["nume"]}')
        res = addCountry(country)
        print(res)
        country_ids.append(res['id'])
    for i in range(len(cities)):
        cities[i]['idTara'] = country_ids[i // 2]
        city = cities[i]
        res = addCity(city)
        print(res)
        city_ids.append(res['id'])
    for i in range(len(temperatures)):
        temperatures[i]['id_oras'] = city_ids[i]
        temperature = temperatures[i]
        res = addTemperature(temperature)
        print(res)
        temperature_ids.append(res['id'])
    print(getCities())
    print("Update")
    for i in range(len(temperature_ids)):
        res = updateTemperature(temperature_ids[i], {'id_oras' : city_ids[0]})
        print(res)
    print(getCities())
    deleteAllCountries()
    #deleteAllCities()
    print(getCountries())
    print(getCities())
    print(getTemperatures())

runFirstTest()