<template>
  <div class="md-layout">
      <div class="md-layout-item">
        <node-editor></node-editor>
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
  },
};
</script>
