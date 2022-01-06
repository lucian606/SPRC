const mongoose = require('mongoose');

const CountrySchema = new mongoose.Schema({
    id: {
        type: String,
        default: function () { return new mongoose.Types.ObjectId()},
        unique: true,
        auto: true
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
    orase: {
        type: Array,
        default: []
    }
},
    {
        versionKey: false
    }
);

module.exports = mongoose.model('Country' ,CountrySchema, 'Countries');