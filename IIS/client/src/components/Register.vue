<template>
  <div class="col-md-6 col-md-offset-3 mx-auto">
    <b-alert v-model="showDismissibleAlert" variant="danger" dismissible>
      {{showAlert}}
    </b-alert>
    <b-form @submit="onSubmit" >

      <b-form-group id="input-group-1" label="Your login:" label-for="input-1">
        <b-form-input
          id="input-1"
          v-model="form.nickname"
          required
          placeholder="Enter login"></b-form-input>
      </b-form-group>

      <b-form-group id="input-group-2" label="Your full name:" label-for="input-2">
        <b-form-input
          id="input-2"
          v-model="form.name"
          required
          placeholder="Enter name"></b-form-input>
      </b-form-group>


      <b-form-group
        id="input-group-5"
        label="Email:"
        label-for="input-5"
        description="We'll never share your email with anyone else."
      >
        <b-form-input
          id="input-5"
          v-model="form.email"
          type="email"
          required
          placeholder="Enter email"></b-form-input>
      </b-form-group>

      <b-form-group
        id="input-group-3"
        label="Password:"
        label-for="input-3"
        description="Please make sure your password is secure."
      >
        <b-form-input
          id="input-3"
          v-model="form.password"
          type="password"
          required
          placeholder="Enter password"></b-form-input>
      </b-form-group>

      <b-form-group
        id="input-group-6"
        label="Password verification:"
        label-for="input-6"
      >
        <b-form-input
          id="input-6"
          v-model="repeatPwd"
          type="password"
          :state="equalState"
          required
          placeholder="Repeat password"></b-form-input>
      </b-form-group>

      <b-form-group
        id="input-group-4"
        label="Birthdate:"
        label-for="input-4"
      >
        <b-form-input
          id="input-4"
          v-model="form.birthdate"
          type="date"
          required></b-form-input>
      </b-form-group>
      <b-button type="submit" class="mr-3" variant="primary">Register</b-button>
      <b-button @click="login" variant="primary">Login</b-button>
    </b-form>
  </div>
</template>

<script>
import Axios from 'axios';

export default {
  name: 'Register',
  computed: {
    equalState() {
      if (this.form.password === '' || this.repeatPwd.length < this.form.password.length) {
        return null;
      }
      return this.form.password === this.repeatPwd;
    },
  },
  data() {
    return {
      form: {
        password: '',
        nickname: '',
        name: '',
        email: '',
        birthdate: '',
      },
      repeatPwd: '',
      showDismissibleAlert: false,
      showAlert: '',
    };
  },
  methods: {
    login() {
      this.$router.push('/login');
    },
    onSubmit(event) {
      event.preventDefault();
      if (this.equalState) {
        const req = Axios.create({
          withCredentials: true,
        });
        req.post(this.$endpoints.user, JSON.stringify(this.form), {
          headers: {
            'Content-Type': 'application/json',
          },
        })
          .then(() => this.$router.push('/login'))
          .catch((error) => {
            this.showAlert = error.response.data.error;
            this.showDismissibleAlert = true;
          });
      } else {
        event.preventDefault();
      }
    },
  },
};
</script>

<style scoped>

</style>
