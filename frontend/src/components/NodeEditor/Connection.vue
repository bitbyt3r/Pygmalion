<template>
    <div>
    </div>
</template>

<script>
export default {
    name: 'connection',
    props: [
        'connectionID'
    ],
    data() {
        return {
            path: null,
            svg: null,
            mousex: 0,
            mousey: 0
        }
    },
    computed: {
        start() {
            if (this.svg) {
                const id = 'output.'+this.connection.output.node+'.'+this.connection.output.output;
                if (this.$store.state.nodeEditor.endpoints.hasOwnProperty(id)) {
                    const origin = this.svg.getBoundingClientRect();
                    const dest = this.$store.state.nodeEditor.endpoints[id];
                    return {
                        x: dest.x - origin.x,
                        y: dest.y - origin.y
                    };
                }
            }
            return {x:0, y:0}
        },
        end() {
            if (this.svg) {
                const origin = this.svg.getBoundingClientRect();
                if (this.dragging) {
                    return {
                        x:this.mousex - origin.x,
                        y:this.mousey - origin.y
                    };
                }
                const id = 'input.'+this.connection.input.node+'.'+this.connection.input.input;
                if (this.$store.state.nodeEditor.endpoints.hasOwnProperty(id)) {
                    const dest = this.$store.state.nodeEditor.endpoints[id];
                    return {
                        x: dest.x - origin.x,
                        y: dest.y - origin.y
                    };
                }
            }
            return {
                x:0,
                y:0
            }
        },
        dragging() {
            return this.connection.dragging;
        },
        connection() {
            return this.$store.state.nodeEditor.connections[this.connectionID];
        }
    },
    watch: {
        start() {
            this.updateCurve();
        },
        end() {
            this.updateCurve();
        },
        dragging(newVal, oldVal) {
            if (newVal && !oldVal) {
                document.addEventListener('mousemove', this.updateEnd);
            }
            if (!newVal && oldVal) {
                document.removeEventListener('mousemove', this.updateEnd);
            }
        }
    },
    mounted() {
        this.svg = this.$parent.$refs.connectionsvg;
        this.path = document.createElementNS(this.svg.namespaceURI, "path");
        this.path.setAttribute('fill', 'none');
        this.path.setAttribute('stroke', '#999999');
        this.path.setAttribute('stroke-width', 2);
        this.path.setAttribute('stroke-dasharray', '20,5,5,5,5,5');
        this.updateCurve();
        this.svg.appendChild(this.path);
    },
    methods: {
        updateCurve() {
            const dif = Math.abs(this.start.x-this.end.x) / 1.5;
            const str = "M" + this.start.x + "," + this.start.y + " C" +
                (this.start.x + dif) + "," + this.start.y + " " +
                (this.end.x - dif) + "," + this.end.y + " " +
                (this.end.x) + "," + this.end.y;
            this.path.setAttribute('d', str);
        },
        updateEnd(event) {
            this.mousex = event.clientX;
            this.mousey = event.clientY;
        }
    }
};
</script>

