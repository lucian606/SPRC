var express = require('express');
var router = express.Router();
const {getAllTemperatures, addTemperature, deleteTemperature, updateTemperature} = require('../Apis/TemperaturesApi');

router.get('/', function(req, res) {
    getAllTemperatures().then(result => {
        res.status(result.code);
        res.send(result.data);
    });
});

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

module.exports = router;