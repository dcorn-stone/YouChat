import React, { useState } from "react";
import { invoke } from "@tauri-apps/api/core";


const ServerAddr = ({ onConfigured }) => {
  const [address, setAddress] = useState("");
  const [port, setPort] = useState("");
  const [error, setError] = useState(null);
  const [loading, setLoading] = useState(false);

  const handleSubmit = async (e) => {
    e.preventDefault();

    // Basic validation
    if (!address.trim()) {
      setError("Address is required");
      return;
    }
    const portNumber = parseInt(port, 10);
    if (isNaN(portNumber) || portNumber <= 0 || portNumber > 65535) {
      setError("Port must be a valid number between 1 and 65535");
      return;
    }

    setError(null);
    setLoading(true);

    try {
      const result = await invoke("connect", {
        address,
        port: portNumber,
      });

      if (result === 0) {
        // Connection successful
        localStorage.setItem("server_address", address);
        localStorage.setItem("server_port", portNumber.toString());
        onConfigured(); // notify parent to proceed
      } else {
        setError(`Failed to connect to server (code ${result})`);
      }
    } catch (err) {
      console.error(err);
      setError("Unexpected error occurred while connecting");
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="server-config">
      <h2>Connect to Server</h2>
      <form onSubmit={handleSubmit}>
        <input
          type="text"
          placeholder="Server Address (e.g., 127.0.0.1)"
          value={address}
          onChange={(e) => setAddress(e.target.value)}
          required
          disabled={loading}
        />
        <input
          type="number"
          placeholder="Port (e.g., 5000)"
          value={port}
          onChange={(e) => setPort(e.target.value)}
          required
          disabled={loading}
          min={1}
          max={65535}
        />
        <button type="submit" disabled={loading}>
          {loading ? "Connecting..." : "Connect"}
        </button>
      </form>
      {error && <p style={{ color: "red", marginTop: "1rem" }}>{error}</p>}
    </div>
  );
};

export default ServerAddr;

