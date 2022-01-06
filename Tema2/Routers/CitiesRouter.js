var express = require('express');
var router = express.Router();
const {getAllCities, addCity, getCitiesOfCountry, deleteCity, updateCity} = require('../Apis/CitiesApi');

router.get('/', function(req, res) {
    getAllCities().then(result => {
        res.status(result.code);
        res.send(result.data);
    });
});

router.post('/', function(req, res) {
    addCity(req.body).then(result => {
        res.status(result.code);
        res.send(result.data);
    });
});

router.get('/country/', function(req, res) {
    res.status(200);
    res.send([]);
});

router.get('/country/:id', function(req, res) {
    getCitiesOfCountry(req.params.id).then(result => {
        res.status(result.code);
        res.send(result.data);
    });
});

router.delete('/:id', function(req, res) {
    deleteCity(req.params.id).then(result => {
        res.status(result.code);
        res.send(result.data);
    });
});

router.put('/:id', function(req, res) {
    updateCity(req.params.id, req.body).then(result => {
        res.status(result.code);
        res.send(result.data);
    });
});

module.exports = router;