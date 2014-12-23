/**
 * snappy compress and uncompress class
 *
 * using in oojs:
 *      require('node-snappy');
 *      var snappy = oojs.using('oojs.utility.snappy');
 *
 * using in normal node:
 *      var snappy = require('node-snappy'); 
 * 
 */
define && define({
    name: 'snappy',
    namespace: 'oojs.utility',
    deps:{
        binding: require('bindings')('binding')
    },
    $snappy: function () {
    },

    /**
     * Compress.
     * If input isn't a string or buffer, automatically convert to buffer by using JSON.stringify.
     */
    compress: function (input) {
        if (!(typeof (input) === 'string' || Buffer.isBuffer(input))) {
            throw new Error('input must be a String or a Buffer');
        }
        return this.binding.compress(input);
    },

    /**
     * Decide if a buffer is compressed in a correct way.
     */
    isValidCompressed: function (input) {
        return this.binding.isValidCompressed(input);
    },

    /**
     * Uncompress previously compressed data.
     * A parser can be attached. If no parser is attached, return buffer.
     */
    uncompress : function (compressed, opts) {
        if (!Buffer.isBuffer(compressed)) {
            throw new Error('input must be a Buffer');
        }
        opts = opts || {};
        if (typeof (opts.asBuffer) !== 'boolean') opts.asBuffer = true
        return this.binding.uncompress(compressed, opts)
    }
});