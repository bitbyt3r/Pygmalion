// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue';
import VueWamp from 'vue-wamp';
import VueMaterial from 'vue-material';
import 'vue-material/dist/vue-material.min.css';
import 'vue-material/dist/theme/default-dark.css';

import App from './App.vue';
import router from './router';

Vue.config.productionTip = false;

// Vue.use(VueWamp, {
//   debug: true,
//   url: 'ws://localhost:8080/ws',
//   realm: 'realm1',
// });

Vue.use(VueMaterial);

/* eslint-disable no-new */
new Vue({
  el: '#app',
  router,
  components: { App },
  render (h) {
    return h('App')
  }
});

