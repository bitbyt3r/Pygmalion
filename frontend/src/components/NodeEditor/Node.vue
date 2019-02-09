<template>
    <div class="node" v-bind:style="style">
        <header @mousedown="dragStart">
            {{ node.name }}
        </header>
        <div class="connections">
            <ul>
                <li v-for="input in node.inputs" :key="input.id" class="connection input">
                    <i class="dot" :id="'input.'+node.id+'.'+input.id" @click="inputClicked(input, $event)">&nbsp;</i>
                    <span>{{ input.name }}</span>
                </li>
            </ul>
            <ul>
                <li v-for="output in node.outputs" :key="output.id" class="connection output">
                    <i class="dot" :id="'output.'+node.id+'.'+output.id" @click="outputClicked(output, $event)">&nbsp;</i>
                    <span>{{ output.name }}</span>
                </li>
            </ul>
        </div>
        <control v-for="control in node.controls" :key="control.id" :control="control"></control>
    </div>
</template>

<script>
import Control from './Control.vue';

export default {
    name: 'node',
    components: {
        Control,
    },
    props: [
        'nodeID'
    ],
    data() {
        return {
            dragX: 0,
            dragY: 0
        };
    },
    computed: {
        style() {
            return {
                left: this.node.x + "px",
                top: this.node.y + "px"
            };
        },
        node() {
            return this.$store.state.nodeEditor.nodes[this.nodeID];
        }
    },
    watch: {
    },
    methods: {
        inputClicked(input, event) {
            event.target.classList.add('selected');
            this.$emit('inputClicked', {input, event});
        },
        outputClicked(output, event) {
            event.target.classList.add('selected');
            this.$emit('outputClicked', {output, event});
        },
        dragStart(event) {
            document.addEventListener('mousemove', this.dragMove);
            document.addEventListener('mouseup', this.dragEnd);
            this.dragX = event.clientX;
            this.dragY = event.clientY;
        },
        dragMove(event) {
            const newnode = {
                x: this.node.x + event.clientX - this.dragX,
                y: this.node.y + event.clientY - this.dragY,
                id: this.node.id
            };
            this.$store.commit('update_node', newnode);
            this.updateConnections();
            this.dragX = event.clientX;
            this.dragY = event.clientY;
        },
        dragEnd(event) {
            const newnode = {
                x: this.node.x + event.clientX - this.dragX,
                y: this.node.y + event.clientY - this.dragY,
                id: this.node.id
            };
            this.$store.commit('update_node', newnode);
            this.updateConnections();
            this.dragX = event.clientX;
            this.dragY = event.clientY;
            document.removeEventListener('mousemove', this.dragMove);
            document.removeEventListener('mouseup', this.dragEnd);
        },
        updateConnections() {
            const self = this;
            this.node.inputs.forEach(function(input) {
                const id = 'input.'+self.node.id+'.'+input.id;
                const element = document.getElementById(id);
                if (element) {
                    const rect = element.getBoundingClientRect();        
                    const point = {
                        x: rect.x + rect.width/2,
                        y: rect.y + rect.height/2
                    };
                    self.$store.commit('update_endpoint', {
                        id: id,
                        pos: {x:point.x, y:point.y}
                    })
                }
            });
            this.node.outputs.forEach(function(output) {
                const id = 'output.'+self.node.id+'.'+output.id;
                const element = document.getElementById(id);
                if (element) {
                    const rect = element.getBoundingClientRect();
                    const point = {
                        x: rect.x + rect.width/2,
                        y: rect.y + rect.height/2
                    };
                    self.$store.commit('update_endpoint', {
                        id: id,
                        pos: {x:point.x, y:point.y}
                    })
                }
            });
        }
    },
    mounted() {
        this.updateConnections();
        window.addEventListener('resize', this.updateConnections);
    },
};
</script>

<style>
.node {
    position: absolute;
	background-color:rgba(63,63,63,.7);
	display:inline-block;
	border-radius:5px;
	box-shadow: 0px 5px 10px #000000;
}

.node > header {
	display:block;
	background-color:#297286;
	color:white;
	cursor:pointer;
	border-radius:5px 5px 0px 0px;
	text-align:center;
	padding:4px 12px;
	-webkit-touch-callout: none;
	-webkit-user-select: none;
	-moz-user-select: none;
	-ms-user-select: none;
	user-select: none;
}

ul {
	margin:0px;
	padding:0px;
    list-style:none;
    float: left;
}

.connections:after {
    content: "";
    display: table;
    clear: both;
}

.connection {
	position:relative;
	margin-top:8px;
    margin-bottom:8px;
}

span {
	padding-left:1em;
	padding-right:1em;
	color:white;
}

.dot {
	position:absolute;
	width:0.6em;
	height:0.6em;
	background-color:gray;
	border-radius:1em;
	cursor:pointer;
}

.dot:hover {
	background-color:red;
}

.input .dot {
	left:-0.3em;
	top:0.4em;
}

.output {
	text-align:right;
}

.output .dot {
	right:-0.25em;
	top:0.4em;
}

.selected.dot {
	background-color:#72a836;
}
</style>