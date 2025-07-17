import { useState } from 'react';
import { invoke } from '@tauri-apps/api/core';
import ServerConnectionForm from '../components/ServerConnectionForm';

function ConnectionPage({ onConnectionSuccess }) {
  const [isLoading, setIsLoading] = useState(false);
  const [error, setError] = useState('');
  
  const handleConnect = async (address, port) => {
    setIsLoading(true);
    setError('');
    
    try {
      // Call the Rust function to connect to the server
      const result = await invoke('connect', { address, port: parseInt(port) });
      
      if (result === 0) {
        onConnectionSuccess();
      } else {
        setError('Failed to connect to the server. Error code: ' + result);
      }
    } catch (err) {
      setError('Error connecting to server: ' + err.toString());
    } finally {
      setIsLoading(false);
    }
  };
  
  return (
    <div className="page-container">
      <h1>Connect to Server</h1>
      <p>Please enter the server details to connect</p>
      <ServerConnectionForm onSubmit={handleConnect} isLoading={isLoading} />
      {error && <div className="error-message">{error}</div>}
    </div>
  );
}

export default ConnectionPage;
