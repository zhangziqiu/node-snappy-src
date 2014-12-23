require('node-oojs');
oojs.setPath({
    'oojs.utility.snappy': __dirname
});
module.exports = oojs.using('oojs.utility.snappy');