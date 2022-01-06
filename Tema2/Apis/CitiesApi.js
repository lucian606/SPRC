const Cities = require('../Models/Cities');
const Countries = require('../Models/Countries');
const createMessage = require('./CommonApi')

async function getAllCities() {
    let result = {code: 200, data: {}};
    try {
        let cities = await Cities.find({}, {_id: 0});
        result.data = cities;
    } catch (error) {
        console.log(error);
        result.code = 500;
        result.data = createMessage("Error getting countries");
    }
    return result;
}

async function addCity(cityData) {
    let result = {code: 200, data: {}};
    let existingCity = await Cities.find({nume: cityData.nume, idTara: cityData.idTara});
    if (existingCity.length != 0) {
        result.code = 409;
        result.data = createMessage("City already exists");
    } else {
        try {
            let country = await Countries.find({id: cityData.idTara});
            if (country.length == 0) {
                result.code = 400;
                result.data = createMessage("Country not found");
            } else {
                country = country[0];
                let newCity = await Cities.create(cityData);
                let country_cities = country.orase;
                country_cities.push(newCity.id);
                await Countries.updateOne({id: cityData.idTara}, {orase: country_cities});
                result.code = 201;
                result.data = {id: newCity.id};
            }
        } catch (error) {
            console.log(error);
            result.code = 400;
            result.data = createMessage("Invalid country body for country post request");
        }
    }
    return result;
}

async function getCitiesOfCountry(idTara) {
    console.log(idTara);
    console.log("CEVA");
    let result = {code: 200, data: {}};
    try {
        let country = await Countries.find({id: idTara});
        let cities = await Cities.find({id: {$in: country[0].orase}}, {_id: 0});
        result.data  = cities;
    } catch (error) {
        console.log(error);
        result.code = 500;
        result.data = createMessage("Error getting cities of country");
    }
    return result;
}

async function updateCity(id, cityBody) {
    let result = {code: 200, data: {}};
    let existingCity = await Cities.find({id: id});
    if (existingCity.length == 0) {
        result.code = 404;
        result.data = createMessage("City not found");
    } else {
        try {
            cityBody.id = id;
            await Cities.findOneAndUpdate({id: id}, {$set: cityBody}, {upsert : false, useFindAndModify: false, runValidators : true})
            result.code = 200;
            result.data = createMessage("City updated");
        } catch (error) {
            console.log(error);
            result.code = 400;
            result.data = createMessage("Invalid body for city update");
        }
    }
    return result;
}

async function deleteCity(id) {
    let result = {code: 200, data: {}};
    let existingCity = await Cities.find({id: id});
    if (existingCity.length == 0) {
        result.code = 404;
        result.data = createMessage("City not found");
    } else {
        try {
            let countryId = existingCity[0].idTara;
            let country = await Countries.find({id: countryId});
            let country_cities = country[0].orase.filter(cityId => cityId != id);
            await Countries.findOneAndUpdate({id: countryId}, {orase: country_cities}, {upsert : false, useFindAndModify: false, runValidators : true});
            await Cities.findOneAndDelete({id: id});
            result.code = 200;
            result.data = createMessage("City deleted");
        } catch (error) {
            console.log(error);
            result.code = 400;
            result.data = createMessage("Invalid body for city delete");
        }
    }
    return result;
}

module.exports = {getAllCities, addCity, getCitiesOfCountry, updateCity, deleteCity}