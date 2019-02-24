import Vue from 'vue';

const vue = new Vue();

const default_state = {
    user: {
        username: ""
    }
};
const getters = {
    user: (state) => state.user
};
const actions = {
};
const mutations = {
    set_user(state, username) {
        vue.$set(state.user, "username", username);
    },
    clear_user(state) {
        vue.$set(state.user, "username", "");
    }
};
export default {
    state: JSON.parse(JSON.stringify(default_state)),
    getters,
    actions,
    mutations
};