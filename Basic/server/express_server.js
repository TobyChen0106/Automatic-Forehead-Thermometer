const express = require('express')
const bodyParser = require('body-parser')
const app = express()

const PORT = 8080

app.use(bodyParser.json({limit: '1mb'}))  //integrate json
app.use(bodyParser.urlencoded({            
    extended: true
}))

let last_data = {}
app.get('/', function (req, res) {

  res.send(JSON.stringify(last_data)) // show last recorded temperature

});

app.listen(PORT, function () {
  console.log(`Server listening on port ${PORT}!`)
});

app.post('/upload',function (req,res) {
    // API endpoint for esp8266
    console.log(req.body)
    last_data = req.body
    res.send('Server recieved message')
})
