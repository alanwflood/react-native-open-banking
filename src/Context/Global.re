type globalContext = {
  auth: Auth.authContext,
  institution: (Institution.institutions => Institution.institutions) => unit,
};
