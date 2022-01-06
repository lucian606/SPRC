const Countries = require('../Models/Countries');
const Cities = require('../Models/Cities');
const Temperatures = require('../Models/Temperatures');
const createMessage = require('./CommonApi');

async function getAllTemperatures() {
    let result = {code: 200, data: {}};
    try {
        let temperatures = await Temperatures.find({}, {_id: 0});
        result.data = temperatures;
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
        result.code = 500;
        result.data = createMessage("Error deleting temperature");
    }
    return result;
}

module.exports = {getAllTemperatures, addTemperature, deleteTemperature}