require('dotenv').config();
const express = require('express');
const mongoose  = require('mongoose');
const app = express();
const port = process.env.PORT || 3001;
const CountriesRouter = require('./Routers/CountriesRouter');
const CitiesRouter = require('./Routers/CitiesRouter');
const TemperaturesRouter = require('./Routers/TemperaturesRouter');

app.use(express.static('public'));
app.use(express.json());
app.use('/api/countries', CountriesRouter);
app.use('/api/cities', CitiesRouter);
app.use('/api/temperatures', TemperaturesRouter);
mongoose.connect(process.env.MONGO_URL, { useNewUrlParser: true, useUnifiedTopology: true })


app.listen(port, () => console.log(`Example app listening on port ${port}!`));
