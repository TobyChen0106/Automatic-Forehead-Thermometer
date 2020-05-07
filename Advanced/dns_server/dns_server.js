var named = require('node-named');
const winston = require('winston');
const logger = winston.createLogger({
  level: 'silly'
});

logger.add(new winston.transports.Console({
    format: winston.format.combine(
        winston.format.splat(),
        winston.format.simple()
    ),
    transports: [new winston.transports.Console()]
  }));

var server = named.createServer();
var ttl = 5;

var okAns = new named.ARecord('1.1.1.1');
var rejAns = new named.ARecord('2.2.2.2');

server.listen(53, '::ffff:0.0.0.0', () => {
    console.log('DNS server started');
});

server.on('query', (query) => {
    try {
        var domain = query.name();
        var arr = domain.split('.');
        var target = new named.SOARecord(domain, { serial: 123 });
        query.addAnswer(domain, target, ttl);
        if(arr.length == 5) {
            // Fill in your own domain name
            if(arr[2] == 'YOUR' && arr[3] == 'DOMAIN' && arr[4] == 'NAME') {
                // Parse your data in the domain name string
                console.log(arr[0]);
                query.addAnswer(domain, okAns, ttl);
                server.send(query);
            }
            else {
                throw 'rejected';
            }
        }
        else {
            throw 'rejected';
        }
    }
    catch(e) {
        query.addAnswer(domain, rejAns, ttl);
        server.send(query);
    }
});