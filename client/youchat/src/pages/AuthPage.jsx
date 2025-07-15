import React, { useState } from "react";
import { invoke } from "@tauri-apps/api/core";
import "./AuthPage.css";

const AuthPage = () => {
  const [mode, setMode] = useState("login");
  const [username, setUsername] = useState("");
  const [nickname, setNickname] = useState("");
  const [password, setPassword] = useState("");
  const [status, setStatus] = useState(null);


  const handleSubmit = async (e) => {
    e.preventDefault();
    try{
      const payload = 
        mode === "login"
          ? {username, password}
          : {username, nickname, password};

      const command = mode === "login" ? "sign_in" : "sign_up";

      const result = await invoke(command, payload);

      if (result === 1){
        setStatus("success")
      } else if (result === 0){
        setStatus(`The username is already taken`)
      }else {
        setStatus(`Failed to sign up (code: ${result})`)
      }

    } catch (err) {
      console.error(err);
      setStatus("Error");
    }
  };

  return (
    <div className="auth-container">
      <h2>{mode === "login" ? "Login" : "Sign Up"}</h2>
      <form className="auth-form" onSubmit={handleSubmit}>
        <input
          type="text"
          placeholder="Username"
          value={username}
          required
          onChange={(e) => setUsername(e.target.value)}
        />
        {mode === "signup" && (
          <input
            type="text"
            placeholder="Nickname"
            value={nickname}
            required
            onChange={(e) => setNickname(e.target.value)}
          />
        )}
        <input
          type="password"
          placeholder="Password"
          value={password}
          required
          onChange={(e) => setPassword(e.target.value)}
        />
        <button type="submit">
          {mode === "login" ? "Login" : "Create Account"}
        </button>
      </form>

      <p style={{ marginTop: "1rem" }}>
        {mode === "login" ? "No account?" : "Already have an account?"}{" "}
        <button
          type="button"
          style={{
            background: "none",
            color: "blue",
            border: "none",
            cursor: "pointer",
            textDecoration: "underline",
          }}
          onClick={() => {
            setMode(mode === "login" ? "signup" : "login");
            setStatus(null);
          }}
        >
          {mode === "login" ? "Sign Up" : "Login"}
        </button>
      </p>

      {status && <p style={{ marginTop: "1rem", fontWeight: "bold" }}>{status}</p>}
    </div>
  );
};

export default AuthPage;



