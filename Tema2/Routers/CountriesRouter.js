var express = require('express');
var router = express.Router();
const {getAllCountries, addCountry, deleteCountry, updateCountry} = require('../Apis/CountriesApi');

router.get('/', function(req, res) {
    getAllCountries().then(result => {
        res.status(result.code);
        res.send(result.data);
    });
});

router.post('/', function(req, res) {
    addCountry(req.body).then(result => {
        res.status(result.code);
        res.send(result.data);
    });
});

router.delete('/:id', function(req, res) {
    deleteCountry(req.params.id).then(result => {
        res.status(result.code);
        res.send(result.data);
    });
});

router.put('/:id', function(req, res) {
    updateCountry(req.params.id, req.body).then(result => {
        res.status(result.code);
        res.send(result.data);
    });
});

module.exports = router;