const mongoose = require('mongoose');

const TemperatureSchema = new mongoose.Schema({
    id: {
        type: Number,
        default: Date.now,
        unique: true
    },
    valoare: {
        type: Number,
        required: true
    },
    id_oras: {
        type: Number,
        required: true
    },
    timestamp: {
        type: Number,
        default: Date.now
    }
},
    {
        versionKey: false
    }
);

TemperatureSchema.index({id_oras: 1, timestamp: 1}, {unique: true});
module.exports = mongoose.model('Temperatures' , TemperatureSchema, 'Temperatures');