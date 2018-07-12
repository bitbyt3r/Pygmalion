import Vue from 'vue';
import VueWamp from 'vue-wamp';
import 'vue-material/dist/vue-material.min.css';
import 'vue-material/dist/theme/default-dark.css';

import * as VM from 'vue-material/dist/components';

import App from './App';
import router from './router/index.js';
import store from './store/index.js';

fetch('/conf.json').then(res => res.json()).then((config) => {
    Vue.config.productionTip = config.productionTip;

    Vue.use(VueWamp, {
        debug: config.wamp.debug,
        url: config.wamp.url,
        realm: config.wamp.realm
    });

    Vue.use(VM.MdButton);
    Vue.use(VM.MdContent);
    Vue.use(VM.MdTabs);
    Vue.use(VM.MdIcon);
    Vue.use(VM.MdList);
    Vue.use(VM.MdApp);
    Vue.use(VM.MdDrawer);
    Vue.use(VM.MdToolbar);

    new Vue({
        el: '#app',
        router,
        store,
        components: { App },
        template: '<App/>',
    });
});



