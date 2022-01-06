const mongoose = require('mongoose');

const CitySchema = new mongoose.Schema({
    id: {
        type: Number,
        default: Date.now,
        unique: true
    },
    idTara: {
        type: Number
    },
    nume: {
        type: String,
        required: true,
        unique: true
    },
    lat: {
        type: Number,
        required: true
    },
    lon: {
        type: Number,
        required: true
    },
    temperaturi: {
        type: Array,
        default: []
    }
},
    {
        versionKey: false
    }
);

CitySchema.index({idTara: 1, nume: 1}, {unique: true});
module.exports = mongoose.model('Cities' ,CitySchema, 'Cities');