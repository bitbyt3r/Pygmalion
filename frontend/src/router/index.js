import Vue from 'vue';
import Router from 'vue-router';
import Home from '../components/Home.vue';
import Capture from '../components/Capture.vue';
import Process from '../components/Process.vue';
import Review from '../components/Review.vue';
import Settings from '../components/Settings.vue';

Vue.use(Router);

export default new Router({
  routes: [
    {
      path: '/',
      name: 'Home',
      component: Home,
    },
    {
      path: '/capture',
      name: 'Capture',
      component: Capture,
    },
    {
      path: '/process',
      name: 'Process',
      component: Process,
    },
    {
      path: '/review',
      name: 'Review',
      component: Review,
    },
    {
      path: '/settings',
      name: 'Settings',
      component: Settings,
    },
  ],
});
