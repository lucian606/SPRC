const express = require('express');
const mongoose  = require('mongoose');
const app = express();
const port = 3000;
const CountriesRouter = require('./Routers/CountriesRouter');
const CitiesRouter = require('./Routers/CitiesRouter');
const TemperaturesRouter = require('./Routers/TemperaturesRouter');

app.use(express.static('public'));
app.use(express.json());
app.use('/api/countries', CountriesRouter);
app.use('/api/cities', CitiesRouter);
app.use('/api/temperatures', TemperaturesRouter);
mongoose.connect('mongodb://localhost:27017/test', { useNewUrlParser: true, useUnifiedTopology: true })


app.listen(port, () => console.log(`Example app listening on port ${port}!`));
