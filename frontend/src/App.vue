<template>
  <div id="app">
    <md-app>
      <md-app-toolbar>
        <md-button class="md-icon-button" @click="menuVisible = !menuVisible">
          <md-icon>menu</md-icon>
        </md-button>
        <h3 class="md-title" style="flex: 1">Pygmalion</h3>
        <div class="md-medium-hide nav-buttons">
          <router-link to="/">
            <md-button><md-icon>home</md-icon>Home</md-button>
          </router-link>
          <router-link to="/capture">
            <md-button><md-icon>camera</md-icon>Capture</md-button>
          </router-link>
          <router-link to="/process">
            <md-button><md-icon>hourglass_full</md-icon>Process</md-button>
          </router-link>
          <router-link to="/review">
            <md-button><md-icon>collections</md-icon>Review</md-button>
          </router-link>
          <router-link to="/settings">
            <md-button><md-icon>settings</md-icon>Settings</md-button>
          </router-link>
        </div>
      </md-app-toolbar>
      <md-app-drawer :md-active.sync="menuVisible">
        <md-toolbar class="md-transparent" md-elevation="0">
          Navigation
        </md-toolbar>
        <md-list>
          <md-list-item>
            <router-link to="/">
              <md-icon>home</md-icon>
              <span class="md-list-item-text">Home</span>
            </router-link>
          </md-list-item>
          <md-list-item>
            <router-link to="/capture">
              <md-icon>camera</md-icon>
              <span class="md-list-item-text">Capture</span>
            </router-link>
          </md-list-item>
          <md-list-item>
            <router-link to="/process">
              <md-icon>hourglass_full</md-icon>
              <span class="md-list-item-text">Process</span>
            </router-link>
          </md-list-item>
          <md-list-item>
            <router-link to="/review">
              <md-icon>collections</md-icon>
              <span class="md-list-item-text">Review</span>
            </router-link>
          </md-list-item>
          <md-list-item>
            <router-link to="/settings">
              <md-icon>settings</md-icon>
              <span class="md-list-item-text">Settings</span>
            </router-link>
          </md-list-item>
        </md-list>
      </md-app-drawer>
      <md-app-content>
        <router-view/>
      </md-app-content>
    </md-app>
  </div>
</template>

<script>
export default {
  name: 'App',
  data() {
    return {
      menuVisible: false,
    };
  },
};
</script>

<style>
.md-app-scroller {
  min-height: 1000px;
}
.nav-buttons {
  flex: 8;
}
</style>
