<template>
    <div>
        <svg ref="connectionsvg" class="connectionsvg"></svg>
        <node v-for="node in nodes" @inputClicked="inputClicked(node, $event)" :key="node.id" :nodeID="node.id"></node>
        <connection v-for="connection in connections" :key="connection.id" :connectionID="connection.id"></connection>
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
    data() {
        return {

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
        this.$store.commit('add_node', {
            id: 0,
            name: "Trigger",
            x: 150,
            y: 150,
            inputs: [],
            outputs: [
                {
                    id: 0,
                    name: "Trigger"
                }
            ],
            controls: [
                {
                    id: 0,
                    name: "Source",
                    type: "select",
                    options: [
                        'web',
                        'hardware'
                    ]
                }
            ]
        });
        this.$store.commit('add_node', {
            id: 1,
            name: "Delay",
            x: 250,
            y: 150,
            inputs: [
                {
                    id: 0,
                    name: "Start"
                }
            ],
            outputs: [
                {
                    id: 0,
                    name: "Output"
                }
            ],
            controls: [
                {
                    id: 0,
                    name: "Duration",
                    type: "number",
                    min: 0,
                    max: 1000
                }
            ]
        });
        this.$store.commit('add_connection', {
            id: 0,
            dragging: false,
            output: {
                node: 0,
                output: 0
            },
            input: {
                node: 1,
                input: 0
            }
        });
    },
    methods: {
        inputClicked(node, event) {
            console.log("Input clicked: ", node, event);
            this.$store.commit('update_connection', {
                id: 0,
                dragging: true
            });
            console.log("Starting drag");
            document.addEventListener('mousedown', this.dragEnd);
        },
        dragEnd(event) {
            console.log("Ending Drag");
            this.$store.commit('update_connection', {
                id: 0,
                dragging: false
            });
            document.removeEventListener('mousedown', this.dragEnd);
        }
    }
};
</script>

<style>
.connectionsvg {
    width: 100%;
    height: 1000px;
}
</style>