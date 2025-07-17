import { useState } from 'react';
import ConnectionPage from './pages/ConnectionPage';
import AuthPage from './pages/AuthPage';
import ProgressIndicator from './components/ProgressIndicator';
import './styles.css';

function App() {
  const [isConnected, setIsConnected] = useState(false);
  const [isAuthenticated, setIsAuthenticated] = useState(false);
  
  // Handle successful connection
  const handleConnectionSuccess = () => {
    setIsConnected(true);
  };
  
  // Handle successful authentication
  const handleAuthSuccess = () => {
    setIsAuthenticated(true);
  };
  
  // Reset connection (for testing or logout)
  const handleDisconnect = () => {
    setIsConnected(false);
    setIsAuthenticated(false);
  };
  
  // Go back to connection page
  const handleGoBack = () => {
    setIsConnected(false);
  };

  // Calculate current step
  const getCurrentStep = () => {
    if (isAuthenticated) return 3;
    if (isConnected) return 2;
    return 1;
  };
  
  return (
    <div className="app-container">
      {isAuthenticated ? (
        <div className="main-app">
          <h1>Welcome to the Application</h1>
          <p>You are now logged in!</p>
          <button onClick={handleDisconnect} className="button">Logout</button>
        </div>
      ) : isConnected ? (
        <AuthPage onAuthSuccess={handleAuthSuccess} onGoBack={handleGoBack} />
      ) : (
        <ConnectionPage onConnectionSuccess={handleConnectionSuccess} />
      )}
      
      <ProgressIndicator currentStep={getCurrentStep()} totalSteps={3} />
    </div>
  );
}

export default App;
