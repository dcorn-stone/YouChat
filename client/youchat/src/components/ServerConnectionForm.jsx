import { useState } from 'react';

function ServerConnectionForm({ onSubmit, isLoading }) {
  const [address, setAddress] = useState('');
  const [port, setPort] = useState('');
  const [errors, setErrors] = useState({});
  
  const validate = () => {
    const newErrors = {};
    
    if (!address.trim()) {
      newErrors.address = 'Server address is required';
    }
    
    if (!port.trim()) {
      newErrors.port = 'Port is required';
    } else if (!/^\d+$/.test(port) || parseInt(port) < 1 || parseInt(port) > 65535) {
      newErrors.port = 'Port must be a valid number between 1 and 65535';
    }
    
    setErrors(newErrors);
    return Object.keys(newErrors).length === 0;
  };
  
  const handleSubmit = (e) => {
    e.preventDefault();
    
    if (validate()) {
      onSubmit(address, port);
    }
  };
  
  return (
    <form onSubmit={handleSubmit} className="form">
      <div className="form-group">
        <label htmlFor="address">Server Address</label>
        <input
          type="text"
          id="address"
          value={address}
          onChange={(e) => setAddress(e.target.value)}
          placeholder="e.g., 192.168.1.1 or example.com"
          disabled={isLoading}
          className={errors.address ? 'input-error' : ''}
        />
        {errors.address && <div className="error-text">{errors.address}</div>}
      </div>
      
      <div className="form-group">
        <label htmlFor="port">Port</label>
        <input
          type="text"
          id="port"
          value={port}
          onChange={(e) => setPort(e.target.value)}
          placeholder="e.g., 8080"
          disabled={isLoading}
          className={errors.port ? 'input-error' : ''}
        />
        {errors.port && <div className="error-text">{errors.port}</div>}
      </div>
      
      <button 
        type="submit" 
        disabled={isLoading} 
        className="button primary"
      >
        {isLoading ? 'Connecting...' : 'Connect'}
      </button>
    </form>
  );
}

export default ServerConnectionForm;
