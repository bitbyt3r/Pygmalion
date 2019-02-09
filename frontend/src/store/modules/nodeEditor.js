import Vue from 'vue';

const vue = new Vue();

const default_state = {
    nodes: {},
    connections: {},
    endpoints: {}
};
const getters = {
    nodes: (state) => state.nodes,
    nodelist: (state) => Object.values(state.nodes),
    connections: (state) => state.connections,
    connectionlist: (state) => Object.values(state.connections)
};
const actions = {
};
const mutations = {
    add_node(state, node) {
        vue.$set(state.nodes, node.id, node);
    },
    remove_node(state, nodeID) {
        vue.$delete(state.nodes, nodeID);
    },
    update_node(state, newnode) {
        const node = state.nodes[newnode.id];
        Object.assign(node, newnode);
    },

    add_connection(state, conn) {
        vue.$set(state.connections, conn.id, conn);
    },
    remove_connection(state, connID) {
        vue.$delete(state.connections, connID);
    },
    update_connection(state, connection) {
        console.log(connection);
        const conn = state.connections[connection.id];
        Object.assign(conn, connection);
    },

    update_endpoint(state, endpoint) {
        vue.$set(state.endpoints, endpoint.id, endpoint.pos);
    }
};
export default {
    state: JSON.parse(JSON.stringify(default_state)),
    getters,
    actions,
    mutations
};