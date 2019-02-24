import Vue from 'vue';
import Router from 'vue-router';
import Home from '../components/Home.vue';
import Capture from '../components/Capture.vue';
import Process from '../components/Process.vue';
import Review from '../components/Review.vue';
import Settings from '../components/Settings.vue';
import Login from '../components/Login.vue';

Vue.use(Router);

let router = new Router({
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
      meta: {
        requiresAuth: true
      }
    },
    {
      path: '/process',
      name: 'Process',
      component: Process,
      meta: {
        requiresAuth: true
      }
    },
    {
      path: '/review',
      name: 'Review',
      component: Review,
      meta: {
        requiresAuth: true
      }
    },
    {
      path: '/settings',
      name: 'Settings',
      component: Settings,
      meta: {
        requiresAuth: true
      }
    },
    {
      path: '/login',
      name: 'Login',
      component: Login,
    }
  ],
});

router.beforeEach((to, from, next) => {
  if(to.matched.some(record => record.meta.requiresAuth)) {
      if (!window.hasOwnProperty('vue')) {
        next();
        return;
      }
      if (window.vue.$cookies.get('session') == null) {
          next({
              path: '/login'
          })
      } else {
          let session = window.vue.$cookies.get('session');
          window.vue.$wamp.call('session.renew', [], {session: session}).then(function(res) {
            if (res.success) {
              window.vue.$store.commit('set_user', res.user);
            } else {
              window.vue.$store.commit('clear_user');
            }
          }, function(err) {
            window.vue.$store.commit('clear_user');
          });
          next();
      }
  } else {
      next() 
  }
})

export default router;