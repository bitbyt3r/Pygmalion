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
            name: "Photo Source",
            x: 50,
            y: 150,
            inputs: [],
            outputs: [
                {
                    id: 0,
                    name: "Photos"
                }
            ],
            controls: [
                {
                    id: 0,
                    name: "Source",
                    type: "select",
                    options: [
                        'Folder',
                        'Hardware'
                    ]
                }
            ]
        });
        this.$store.commit('add_node', {
            id: 1,
            name: "Photo Batcher",
            x: 250,
            y: 150,
            inputs: [
                {
                    id: 0,
                    name: "Photos"
                }
            ],
            outputs: [
                {
                    id: 0,
                    name: "Photoset"
                }
            ],
            controls: [
                {
                    id: 0,
                    name: "Quantity",
                    type: "number",
                    min: 0,
                    max: 1000
                }
            ]
        });
        this.$store.commit('add_node', {
            id: 2,
            name: "SIFT Describer",
            x: 450,
            y: 150,
            inputs: [
                {
                    id: 0,
                    name: "Photoset"
                }
            ],
            outputs: [
                {
                    id: 0,
                    name: "Tagged Photos"
                }
            ],
            controls: [
            ]
        });
        this.$store.commit('add_node', {
            id: 3,
            name: "Keypoint Matcher",
            x: 50,
            y: 350,
            inputs: [
                {
                    id: 0,
                    name: "Tagged Photos"
                }
            ],
            outputs: [
                {
                    id: 0,
                    name: "Matches"
                }
            ],
            controls: [
            ]
        });
        this.$store.commit('add_node', {
            id: 4,
            name: "Camera Solver",
            x: 250,
            y: 350,
            inputs: [
                {
                    id: 0,
                    name: "Matches"
                }
            ],
            outputs: [
                {
                    id: 0,
                    name: "Camera Parameters"
                },
                {
                    id: 1,
                    name: "Point Cloud"
                }
            ],
            controls: [
            ]
        });
        this.$store.commit('add_node', {
            id: 5,
            name: "Photo Warper",
            x: 350,
            y: 350,
            inputs: [
                {
                    id: 0,
                    name: "Camera Parameters"
                },
                {
                    id: 1,
                    name: "Photoset"
                }
            ],
            outputs: [
                {
                    id: 0,
                    name: "Photoset"
                }
            ],
            controls: [
            ]
        });
        this.$store.commit('add_node', {
            id: 6,
            name: "Dense Reconstuction",
            x: 650,
            y: 350,
            inputs: [
                {
                    id: 0,
                    name: "Photoset"
                }
            ],
            outputs: [
                {
                    id: 0,
                    name: "Point Cloud"
                }
            ],
            controls: [
            ]
        });
        this.$store.commit('add_node', {
            id: 7,
            name: "OBJ Output",
            x: 850,
            y: 350,
            inputs: [
                {
                    id: 0,
                    name: "Point Cloud"
                }
            ],
            outputs: [
            ],
            controls: [
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
        this.$store.commit('add_connection', {
            id: 1,
            dragging: false,
            output: {
                node: 1,
                output: 0
            },
            input: {
                node: 2,
                input: 0
            }
        });
        this.$store.commit('add_connection', {
            id: 2,
            dragging: false,
            output: {
                node: 2,
                output: 0
            },
            input: {
                node: 3,
                input: 0
            }
        });
        this.$store.commit('add_connection', {
            id: 3,
            dragging: false,
            output: {
                node: 3,
                output: 0
            },
            input: {
                node: 4,
                input: 0
            }
        });
        this.$store.commit('add_connection', {
            id: 4,
            dragging: false,
            output: {
                node: 4,
                output: 0
            },
            input: {
                node: 5,
                input: 0
            }
        });
        this.$store.commit('add_connection', {
            id: 5,
            dragging: false,
            output: {
                node: 5,
                output: 0
            },
            input: {
                node: 6,
                input: 0
            }
        });
        this.$store.commit('add_connection', {
            id: 6,
            dragging: false,
            output: {
                node: 6,
                output: 0
            },
            input: {
                node: 7,
                input: 0
            }
        });
        this.$store.commit('add_connection', {
            id: 7,
            dragging: false,
            output: {
                node: 1,
                output: 0
            },
            input: {
                node: 5,
                input: 1
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
            console.log("Starting drag", event);
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