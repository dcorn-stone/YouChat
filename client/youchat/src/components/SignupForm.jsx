import { useState } from 'react';

function SignupForm({ onSubmit, isLoading }) {
  const [username, setUsername] = useState('');
  const [nickname, setNickname] = useState('');
  const [password, setPassword] = useState('');
  const [confirmPassword, setConfirmPassword] = useState('');
  const [errors, setErrors] = useState({});
  
  const validate = () => {
    const newErrors = {};
    
    if (!username.trim()) {
      newErrors.username = 'Username is required';
    } else if (username.length < 3) {
      newErrors.username = 'Username must be at least 3 characters';
    }
    
    if (!nickname.trim()) {
      newErrors.nickname = 'Nickname is required';
    }
    
    if (!password) {
      newErrors.password = 'Password is required';
    } else if (password.length < 6) {
      newErrors.password = 'Password must be at least 6 characters';
    }
    
    if (password !== confirmPassword) {
      newErrors.confirmPassword = 'Passwords do not match';
    }
    
    setErrors(newErrors);
    return Object.keys(newErrors).length === 0;
  };
  
  const handleSubmit = (e) => {
    e.preventDefault();
    
    if (validate()) {
      onSubmit(username, nickname, password);
    }
  };
  
  return (
    <form onSubmit={handleSubmit} className="form">
      <div className="form-group">
        <label htmlFor="signup-username">Username</label>
        <input
          type="text"
          id="signup-username"
          value={username}
          onChange={(e) => setUsername(e.target.value)}
          disabled={isLoading}
          className={errors.username ? 'input-error' : ''}
        />
        {errors.username && <div className="error-text">{errors.username}</div>}
      </div>
      
      <div className="form-group">
        <label htmlFor="nickname">Nickname</label>
        <input
          type="text"
          id="nickname"
          value={nickname}
          onChange={(e) => setNickname(e.target.value)}
          disabled={isLoading}
          className={errors.nickname ? 'input-error' : ''}
        />
        {errors.nickname && <div className="error-text">{errors.nickname}</div>}
      </div>
      
      <div className="form-group">
        <label htmlFor="signup-password">Password</label>
        <input
          type="password"
          id="signup-password"
          value={password}
          onChange={(e) => setPassword(e.target.value)}
          disabled={isLoading}
          className={errors.password ? 'input-error' : ''}
        />
        {errors.password && <div className="error-text">{errors.password}</div>}
      </div>
      
      <div className="form-group">
        <label htmlFor="confirm-password">Confirm Password</label>
        <input
          type="password"
          id="confirm-password"
          value={confirmPassword}
          onChange={(e) => setConfirmPassword(e.target.value)}
          disabled={isLoading}
          className={errors.confirmPassword ? 'input-error' : ''}
        />
        {errors.confirmPassword && <div className="error-text">{errors.confirmPassword}</div>}
      </div>
      
      <button 
        type="submit" 
        disabled={isLoading} 
        className="button primary"
      >
        {isLoading ? 'Creating Account...' : 'Sign Up'}
      </button>
    </form>
  );
}

export default SignupForm;
