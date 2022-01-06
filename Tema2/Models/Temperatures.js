const mongoose = require('mongoose');

const TemperatureSchema = new mongoose.Schema({
    id: {
        type: String,
        default: function () { return new mongoose.Types.ObjectId()},
        unique: true,
        auto: true
    },
    valoare: {
        type: Number,
        required: true
    },
    id_oras: {
        type: String,
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