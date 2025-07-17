import { useState } from 'react';
import AuthTabs from '../components/AuthTabs';
import LoginForm from '../components/LoginForm';
import SignupForm from '../components/SignupForm';
import { invoke } from '@tauri-apps/api/core';

function AuthPage({ onAuthSuccess, onGoBack }) {
  const [activeTab, setActiveTab] = useState('login');
  const [isLoading, setIsLoading] = useState(false);
  const [error, setError] = useState('');
  
  const handleLogin = async (username, password) => {
    setIsLoading(true);
    setError('');
    
    try {
      // Call the Rust function to sign in
      const result = await invoke('sign_in', { username, password });
      
      if (result === 1) {
        onAuthSuccess();
      } else {
        setError('Login failed. Error code: ' + result);
      }
    } catch (err) {
      setError('Error during login: ' + err.toString());
    } finally {
      setIsLoading(false);
    }
  };
  
  const handleSignup = async (username, nickname, password) => {
    setIsLoading(true);
    setError('');
    
    try {
      // Call the Rust function to sign up
      const result = await invoke('sign_up', { username, nickname, password });
      
      if (result === 1) {
        // Switch to login tab after successful signup
        setActiveTab('login');
        setError('');
      } else {
        setError('Signup failed. Error code: ' + result);
      }
    } catch (err) {
      setError('Error during signup: ' + err.toString());
    } finally {
      setIsLoading(false);
    }
  };
  
  return (
    <div className="page-container">
      <div className="page-header">
        <button onClick={onGoBack} className="back-button">
          <span className="back-arrow">←</span> Back
        </button>
        <h1>Welcome</h1>
      </div>
      <p>Please login or create a new account</p>
      
      <AuthTabs activeTab={activeTab} onTabChange={setActiveTab} />
      
      {activeTab === 'login' ? (
        <LoginForm onSubmit={handleLogin} isLoading={isLoading} />
      ) : (
        <SignupForm onSubmit={handleSignup} isLoading={isLoading} />
      )}
      
      {error && <div className="error-message">{error}</div>}
    </div>
  );
}

export default AuthPage;
