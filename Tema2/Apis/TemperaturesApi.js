const Countries = require('../Models/Countries');
const Cities = require('../Models/Cities');
const Temperatures = require('../Models/Temperatures');
const createMessage = require('./CommonApi');

async function getAllTemperatures(filters) {
    let result = {code: 200, data: {}};
    try {
        let temperature_filters = {timestamp : {}};
        let city_filters = {};
        if (filters.from != undefined) {
            temperature_filters.timestamp.$gt = filters.from;
        }
        if (filters.until != undefined) {
            temperature_filters.timestamp.$lt = filters.until;
        }
        if (filters.lat != undefined) {
            city_filters.lat = filters.lat;
        }
        if (filters.lon != undefined) {
            city_filters.lon = filters.lon;
        }
        console.log(city_filters)
        console.log(temperature_filters);
        let temperatures = await Temperatures.find(temperature_filters, {_id: 0});
        let data = [];
        let citiesMap = {};
        let cityIds = [];
        for (let i = 0; i < temperatures.length; i++) {
            cityIds.push(temperatures[i].id_oras);
        }
        city_filters.id = {$in: cityIds};
        let cities = await Cities.find(city_filters, {_id: 0});
        for (let i = 0; i < cities.length; i++) {
            citiesMap[cities[i].id] = cities[i];
        }
        for (let i = 0; i < temperatures.length; i++) {
            city = citiesMap[temperatures[i].id_oras];
            if (city != undefined) {
                data.push({
                    id: temperatures[i].id,
                    idTara: city.idTara,
                    nume: city.nume,
                    lat: city.lat,
                    lon: city.lon
                });
            }
        }
        result.data = data;
    } catch (error) {
        console.log(error);
        result.code = 500;
        result.data = createMessage("Error getting temperatures");
    }
    return result;
}

async function getCityTemperatures(id, filters) {
    let result = {code: 200, data: {}};
    try {
        let city = await Cities.find({id: id});
        if (city.length == 0) {
            result.code = 404;
            result.data = createMessage("City not found");
        } else {
            city = city[0];
            let data = [];
            let temperatures = city.temperaturi;
            let temperature_filters = {id: {$in: temperatures}, timestamp: {}};
            if (filters.from != undefined) {
                temperature_filters.timestamp.$gt = filters.from;
            }
            if (filters.until != undefined) {
                temperature_filters.timestamp.$lt = filters.until;
            }
            temperatures = await Temperatures.find(temperature_filters, {_id: 0});
            for (let i = 0; i < temperatures.length; i++) {
                data.push({
                    id: temperatures[i].id,
                    idTara: city.idTara,
                    nume: city.nume,
                    lat: city.lat,
                    lon: city.lon
                });
            }
            result.data = data;
        }
    } catch (error) {
        console.log(error);
        result.code = 500;
        result.data = createMessage("Error getting temperatures");
    }
    return result;
}

async function addTemperature(temperatureData) {
    let result = {code: 200, data: {}};
    let existingTemperature = await Temperatures.find({id_oras: temperatureData.id_oras, timestamp: temperatureData.timestamp});
    if (existingTemperature.length != 0) {
        result.code = 409;
        result.data = createMessage("Temperature already exists");
    } else {
        try {
            let city = await Cities.find({id: temperatureData.id_oras});
            if (city.length == 0) {
                result.code = 400;
                result.data = createMessage("City not found");
            } else {
                city = city[0];
                let newTemperature = await Temperatures.create(temperatureData);
                let city_temperatures = city.temperaturi;
                city_temperatures.push(newTemperature.id);
                await Cities.updateOne({id: temperatureData.id_oras}, {temperaturi: city_temperatures});
                result.code = 201;
                result.data = {id: newTemperature.id};
            }
        } catch (error) {
            console.log(error);
            result.code = 400;
            result.data = createMessage("Invalid temperature body for temperature post request");
        }
    }
    return result;
}

async function deleteTemperature(id) {
    let result = {code: 200, data: {}};
    try {
        let temperature = await Temperatures.find({id: id});
        if (temperature.length == 0) {
            result.code = 404;
            result.data = createMessage("Temperature not found");
        } else {
            let city = await Cities.find({id: temperature[0].id_oras});
            city = city[0];
            let city_temperatures = city.temperaturi;
            city_temperatures = city_temperatures.filter(tempId => tempId != id);
            await Cities.updateOne({id: temperature[0].id_oras}, {temperaturi: city_temperatures});
            await Temperatures.deleteOne({id: id});
            result.code = 200;
            result.data = createMessage("Temperature deleted");
        }
    } catch (error) {
        console.log(error);
        result.code = 400;
        result.data = createMessage("Error deleting temperature");
    }
    return result;
}

async function updateTemperature(id, temperatureData) {
    let result = {code: 200, data: {}};
    let existingTemperature = await Temperatures.find({id: id});
    if (existingTemperature.length == 0) {
        result.code = 404;
        result.data = createMessage("Temperature not found");
    } else if (temperatureData.id != undefined && temperatureData.id != id) {
        result.code = 400;
        result.data = createMessage("Invalid body for temperature update");
    } else {
        try {
            temperatureData.id = id;
            if (temperatureData.id_oras != undefined && temperatureData.id_oras != existingTemperature[0].id_oras) {
                let city = await Cities.find({id: temperatureData.id_oras});
                let city_temperatures = city[0].temperaturi;
                city_temperatures.push(id);
                await Cities.findOneAndUpdate({id: temperatureData.id_oras}, {temperaturi: city_temperatures});
                let oldCity = await Cities.find({id: existingTemperature[0].id_oras});
                city_temperatures = oldCity[0].temperaturi;
                city_temperatures = city_temperatures.filter(tempId => tempId != id);
                await Cities.findOneAndUpdate({id: existingTemperature[0].id_oras}, {temperaturi: city_temperatures});
            }
            await Temperatures.findOneAndUpdate({id: id},  {$set: temperatureData}, {upsert : false, useFindAndModify: false, runValidators : true});
            result.code = 200;
            result.data = createMessage("Temperature updated");
        } catch (error) {
            console.log(error);
            result.code = 400;
            result.data = createMessage("Invalid temperature body for temperature update request");
        }
    }
    return result;
}

module.exports = {getAllTemperatures, getCityTemperatures, addTemperature, deleteTemperature, updateTemperature};