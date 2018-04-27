'use strict'
const merge = require('webpack-merge')
const prodEnv = require('./prod.env')

module.exports = merge(prodEnv, {
  NODE_ENV: '"development"',
  WAMP_URL: '"ws://localhost:8080/ws"',
  WAMP_REALM: '"realm1"',
})
