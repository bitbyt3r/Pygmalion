<template>
    <div>
        <md-button class="md-raised md-primary" @click="save">Save</md-button>
        <svg ref="connectionsvg" class="connectionsvg"></svg>
        <node v-for="node in nodes" @outputClicked="outputClicked(node, $event)" @inputClicked="inputClicked(node, $event)" :key="'node-'+node.id" :nodeID="node.id"></node>
        <connection v-for="connection in connections" :key="'connection-'+connection.id" :connectionID="connection.id"></connection>
    </div>
</template>

<script>
import Node from "./Node.vue";
import Connection from "./Connection.vue";

export default {
    name: 'node-editor',
    components: {
        Node,
        Connection
    },
    props: {
    },
    data() {
        return {
            active_connection: -1,
            dragging: false
        };
    },
    computed: {
        connections() {
            return this.$store.getters.connectionlist;
        },
        nodes() {
            console.log(this.$store.getters.nodelist);
            return this.$store.getters.nodelist;
        }
    },
    mounted() {
    },
    methods: {
        inputClicked(node, event) {
            if (this.dragging) {
                this.dragging = false;
                return;
            }
            for (var connectionID in this.$store.state.nodeEditor.connections) {
                var connection = this.$store.state.nodeEditor.connections[connectionID];
                if (connection.input.node === node.id) {
                    if (connection.input.input === event.input.id) {
                        this.active_connection = connection.id;
                    }
                }
            }
            if (this.active_connection < 0) {
                console.log("Could not locate connection that was clicked.");
                return;
            }
            this.$store.commit('update_connection', {
                id: this.active_connection,
                dragging: true
            });
            this.dragging = true;
            document.addEventListener('mousedown', this.dragEnd);
        },
        dragEnd(event) {
            console.log("Ending Drag", event.target.id.split("."));
            var addr = event.target.id.split(".");
            if (addr.length != 3 || addr[0] != "input") {
                this.$store.commit('remove_connection', this.active_connection);
                this.active_connection = -1;
                this.dragging = false;
            } else {
                this.$store.commit('update_connection', {
                    id: this.active_connection,
                    dragging: false,
                    input: {
                        node: parseInt(addr[1], 10),
                        input: parseInt(addr[2], 10)
                    }
                });
                this.active_connection = -1;
            }
            document.removeEventListener('mousedown', this.dragEnd);
        },
        outputClicked(node, event) {
            if (this.dragging) {
                return;
            }
            var maxID = 0;
            for (var connectionID in this.$store.state.nodeEditor.connections) {
                if (connectionID > maxID) {
                    maxID = parseInt(connectionID, 10);
                }
            }
            maxID = maxID + 1;
            this.$store.commit('add_connection', {
                id: maxID,
                dragging: true,
                output: {
                    node: node.id,
                    output: event.output.id
                },
                input: {
                    node: 0,
                    input: 0
                }
            });
            this.active_connection = maxID;
            this.dragging = true;
            document.addEventListener('mousedown', this.dragEnd);
        },
        save() {
            this.$emit('save', {nodes: this.nodes, connections: this.connections});
        }
    }
};
</script>

<style>
.connectionsvg {
    width: 100%;
    height: 500px;
}
</style>