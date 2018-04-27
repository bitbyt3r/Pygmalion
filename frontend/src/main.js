// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import 'babel-polyfill';

import Vue from 'vue';
import VueWamp from 'vue-wamp';
import VueMaterial from 'vue-material';
import 'vue-material/dist/vue-material.min.css';
import 'vue-material/dist/theme/default-dark.css';

import App from './App';
import router from './router';

Vue.config.productionTip = false;

Vue.use(VueWamp, {
  debug: true,
  url: process.env.WAMP_URL,
  realm: process.env.WAMP_REALM,
});

Vue.use(VueMaterial);

/* eslint-disable no-new */
new Vue({
  el: '#app',
  router,
  components: { App },
  template: '<App/>',
});

