<template>
  <div>
    <b-form @submit="updatePassword">
      <b-form-group id="input-group-1" label="Old password" label-for="input-1">
        <b-form-input
          id="input-1"
          type="password"
          required
          v-model="oldPwd"
          placeholder="Old password"
        ></b-form-input>
      </b-form-group>

      <b-form-group
        id="input-group-2"
        label="New password"
        label-for="input-2"
        description="Please make sure your password is safe enough"
      >
        <b-form-input
          id="input-2"
          type="password"
          required
          placeholder="New password"
          v-model="newPwd"
        ></b-form-input>
      </b-form-group>

      <b-form-group id="input-group-3" label="Repeat password" label-for="input-3">
        <b-form-input
          id="input-3"
          type="password"
          required
          placeholder="Repeat password"
          :state="equalState"
          v-model="repeatPwd"
        ></b-form-input>
      </b-form-group>
      <b-button type="submit" variant="success">Submit</b-button>
    </b-form>
    <div class="mt-5">
      <b-alert v-model="showDismissibleAlert" :variant="status" dismissible>
        Password could not be changed!
      </b-alert>
    </div>
  </div>
</template>

<script>
import axios from 'axios';

export default {
  name: 'UpdatePassword',
  computed: {
    equalState() {
      if (this.newPwd === '' || this.repeatPwd.length < this.newPwd.length) {
        return null;
      }
      return this.newPwd === this.repeatPwd;
    },
  },
  data() {
    return {
      oldPwd: '',
      newPwd: '',
      repeatPwd: '',
      showDismissibleAlert: false,
      status: 'danger',
    };
  },
  methods: {
    updatePassword(e) {
      const req = axios.create({
        withCredentials: true,
      });
      if (this.equalState === true) {
        req.put(this.$endpoints.password, {
          old_password: this.oldPwd,
          new_password: this.newPwd,
          headers: { 'Content-Type': 'application/json' },
        })
          .then(() => {
            this.updateUser();
            this.status = 'success';
          })
          .catch((error) => {
            this.showDismissibleAlert = true;
            console.log(`UserProfile.vue:acceptHim: ${error}`);
            this.status = 'danger';
          });
      }
      this.showDismissibleAlert = true;
      e.preventDefault();
      // }
    },
  },
};
</script>

<style scoped>

</style>
