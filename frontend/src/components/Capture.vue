<template>
  <div class="md-layout">
    <div class="md-layout-item">
      <md-button class="md-primary md-raised" @click="new_capture">Add</md-button>
      <node-editor @save="save_capture"></node-editor>
    </div>
  </div>
</template>

<script>
import NodeEditor from './NodeEditor/NodeEditor.vue';

export default {
  name: 'Capture',
  components: {
    NodeEditor
  },
  data() {
    return {
    };
  },
  mounted() {
    this.$wamp.call('get_capture_graphs').then(function(res) {
      console.log("Got graphs", res);
    })
  },
  methods: {
    save_capture(event) {
      console.log("Saving capture", event);
    },
    new_capture() {
      console.log("Adding new capture");
    }
  }
};
</script>
