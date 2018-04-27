import Vue from 'vue';
import Router from 'vue-router';
import Home from '@/components/Home';
import Capture from '@/components/Capture';
import Process from '@/components/Process';
import Review from '@/components/Review';
import Settings from '@/components/Settings';

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
