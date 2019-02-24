import Vue from 'vue';
import Vuex from 'vuex';

import nodeEditor from './modules/nodeEditor.js';
import user from './modules/user.js';

Vue.use(Vuex);
const debug = true;
export default new Vuex.Store({
    modules: {
        nodeEditor,
        user
    },
    strict: debug
});