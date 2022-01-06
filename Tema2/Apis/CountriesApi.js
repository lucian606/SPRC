const Countries = require('../Models/Countries');
const Cities = require('../Models/Cities');
const createMessage = require('./CommonApi')    

async function getAllCountries() {
    let result = {code: 200, data: {}};
    try {
        let countries = await Countries.find({}, {_id: 0, orase: 0});
        result.data = countries;
    } catch (error) {
        console.log(error);
        result.code = 500;
        result.data = createMessage("Error getting countries");
    }
    return result;
}

async function addCountry(countryData) {
    let result = {code: 200, data: {}};
    let existingCountry = await Countries.find({nume: countryData.nume});
    if (existingCountry.length != 0) {
        result.code = 409;
        result.data = createMessage("Country already exists");
    } else {
        try {
            let newCountry = await Countries.create(countryData);
            result.code = 201;
            result.data = {id: newCountry.id};
        } catch (error) {
            console.log(error);
            result.code = 400;
            result.data = createMessage("Invalid country body for country post request");
        }
    }
    return result;
}

async function deleteCountry(countryId) {
    let result = {code: 200, data: {}};
    let existingCountry = await Countries.find({id: countryId});
    if (existingCountry.length == 0) {
        result.code = 404;
        result.data = createMessage("Country not found");
    } else {
        try {
            existingCountry = existingCountry[0];
            for (let i = 0; i < existingCountry.orase.length; i++) {
                await Cities.deleteOne({id: existingCountry.orase[i]});
            }
            await Countries.findOneAndDelete({id: countryId});
            result.code = 200;
            result.data = createMessage("Country deleted");
        } catch (error) {
            console.log(error);
            result.code = 400;
            result.data = createMessage("Invalid body for country deletion");
        }
    }
    return result;
}

async function updateCountry(countryId, countryBody) {
    let result = {code: 200, data: {}};
    let existingCountry = await Countries.find({id: countryId});
    if (existingCountry.length == 0) {
        result.code = 404;
        result.data = createMessage("Contry not found");
    } else {
        try {
            countryBody.id = countryId;
            await Countries.findOneAndUpdate({id: countryId}, {$set: countryBody}, {upsert : false, useFindAndModify: false, runValidators : true});
            result.code = 200;
            result.data = createMessage("Country updated");
        } catch (error) {
            console.log(error);
            result.code = 400;
            result.data = createMessage("Invalid body for country update")
        }
    }
    return result;
}

module.exports = { getAllCountries, addCountry, deleteCountry, updateCountry };