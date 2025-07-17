function AuthTabs({ activeTab, onTabChange }) {
  return (
    <div className="tabs">
      <button 
        className={`tab ${activeTab === 'login' ? 'active' : ''}`}
        onClick={() => onTabChange('login')}
      >
        Login
      </button>
      <button 
        className={`tab ${activeTab === 'signup' ? 'active' : ''}`}
        onClick={() => onTabChange('signup')}
      >
        Sign Up
      </button>
    </div>
  );
}

export default AuthTabs;
