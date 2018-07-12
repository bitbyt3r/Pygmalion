import Vue from 'vue';
import Vuex from 'vuex';

import nodeEditor from './modules/nodeEditor.js';

Vue.use(Vuex);
const debug = true;
export default new Vuex.Store({
    modules: {
        nodeEditor
    },
    strict: debug
});