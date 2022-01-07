var express = require('express');
var router = express.Router();
const {getAllTemperatures, addTemperature, deleteTemperature, updateTemperature, getCityTemperatures} = require('../Apis/TemperaturesApi');

router.get('/', function(req, res) {
    let lat = req.query.lat
    let lon = req.query.lon
    let from = req.query.from
    let until = req.query.until
    let filters = {}
    if (!isNaN(Number(lat))) {
        filters.lat = Number(lat)
    }
    if (!isNaN(lon)) {
        filters.lon = Number(lon)
    }
    if (!isNaN(new Date(from))) {
        filters.from = new Date(from).getTime();
    }
    if (!isNaN(new Date(until))) {
        filters.until = new Date(until).getTime();
    }
    getAllTemperatures(filters).then(result => {
        res.status(result.code);
        res.send(result.data);
    });
});

router.get('/cities/:id', function(req, res) {
    let from = req.query.from
    let until = req.query.until
    let filters = {}
    if (!isNaN(new Date(from))) {
        filters.from = new Date(from).getTime();
    }
    if (!isNaN(new Date(until))) {
        filters.until = new Date(until).getTime();
    }
    getCityTemperatures(req.params.id, filters).then(result => {
        res.status(result.code);
        res.send(result.data);
    })
})

router.post('/', function(req, res) {
    addTemperature(req.body).then(result => {
        res.status(result.code);
        res.send(result.data);
    });
});

router.delete('/:id', function(req, res) {
    deleteTemperature(req.params.id).then(result => {
        res.status(result.code);
        res.send(result.data);
    });
});

router.put('/:id', function(req, res) {
    updateTemperature(req.params.id, req.body).then(result => {
        res.status(result.code);
        res.send(result.data);
    });
});

module.exports = router;