<template>
  <div>
    <md-tabs :md-active-tab="activetab">
      <md-tab id="tab-login" md-label="Login">
        <h3>Please Log In</h3>
        <md-field>
            <label>Username</label>
            <md-input v-model="username"></md-input>
        </md-field>
        <md-field>
            <label>Password</label>
            <md-input type="password" v-model="password"></md-input>
        </md-field>
        <button @click="login">Login</button>
      </md-tab>

      <md-tab id="tab-register" md-label="Register">
        <h3>Please Register</h3>
        <md-field>
            <label>Username</label>
            <md-input v-model="username"></md-input>
        </md-field>
        <md-field>
            <label>Full Name</label>
            <md-input v-model="full_name"></md-input>
        </md-field>
        <md-field>
            <label>Password</label>
            <md-input type="password" v-model="password"></md-input>
        </md-field>
        <md-field>
            <label>Confirm Password</label>
            <md-input type="password" v-model="password_confirm"></md-input>
        </md-field>
        <md-field>
            <label>Email</label>
            <md-input v-model="email"></md-input>
        </md-field>
        <md-checkbox v-model="terms">Do you accept the Terms And Conditions?</md-checkbox>
        This site is protected by reCAPTCHA and the Google
    <a href="https://policies.google.com/privacy">Privacy Policy</a> and
    <a href="https://policies.google.com/terms">Terms of Service</a> apply.
        <br>
        <button @click="register">Register</button>
      </md-tab>
    </md-tabs>
  </div>
</template>

<script>
export default {
  name: 'Home',
  data() {
    return {
      username: "",
      password: "",
      full_name: "",
      password_confirm: "",
      email: "",
      terms: false,
      activetab: "Login"
    };
  },
  methods: {
      login() {
          let self = this;
          this.$wamp.call('session.login', [], {username: this.username, password: this.password}).then(function(res) {
              if (res.success) {
                  self.password = "";
                  self.$cookies.set('session', res.session);
                  self.$store.commit('set_user', res.user);
                  self.$router.push('home');
              }
          })
      },
      register() {
          let self = this;
        this.$recaptcha('login').then((token) => {
            self.$wamp.call('user.register', [], {username: self.username, fullname: self.full_name, password: self.password, terms: self.terms, email: self.email, recaptcha: token}).then(function(res) {
                if (res.success) {
                    self.activetab = "Login";
                }
            }, function(err) {
                console.log(err);
            });
        });
      }
  }
};
</script>

<style scoped>
</style>
