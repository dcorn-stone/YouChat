import React, { useState } from "react";

// Login Form Component
function LoginForm({ onLogin, switchToSignup }) {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  return (
    <div>
      <h2>Login</h2>
      <input
        placeholder="Username"
        value={username}
        onChange={e => setUsername(e.target.value)}
      /><br />
      <input
        placeholder="Password"
        type="password"
        value={password}
        onChange={e => setPassword(e.target.value)}
      /><br />
      <button onClick={() => onLogin(username, password)}>Login</button>
      <p>
        Don't have an account? <button onClick={switchToSignup}>Sign up</button>
      </p>
    </div>
  );
}

// Signup Form Component
function SignupForm({ onSignup, switchToLogin }) {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  return (
    <div>
      <h2>Sign Up</h2>
      <input
        placeholder="Username"
        value={username}
        onChange={e => setUsername(e.target.value)}
      /><br />
      <input
        placeholder="Password"
        type="password"
        value={password}
        onChange={e => setPassword(e.target.value)}
      /><br />
      <button onClick={() => onSignup(username, password)}>Sign up</button>
      <p>
        Already have an account? <button onClick={switchToLogin}>Log in</button>
      </p>
    </div>
  );
}

// Main Chat Component (placeholder for now)
function Chat({ user }) {
  return <h2>Welcome, {user}!</h2>;
}

// App Component
function App() {
  const [user, setUser] = useState(null);
  const [showSignup, setShowSignup] = useState(false);

  // Placeholder login/signup handlers
  const handleLogin = (username, password) => {
    // Here you'd check credentials, possibly with Tauri backend
    setUser(username); // For now, just set user
  };
  const handleSignup = (username, password) => {
    // Here you'd register the user, possibly with Tauri backend
    setUser(username); // For now, just set user
  };

  if (!user) {
    return showSignup
      ? (
        <SignupForm
          onSignup={handleSignup}
          switchToLogin={() => setShowSignup(false)}
        />
      )
      : (
        <LoginForm
          onLogin={handleLogin}
          switchToSignup={() => setShowSignup(true)}
        />
      );
  }

  return <Chat user={user} />;
}

export default App;
