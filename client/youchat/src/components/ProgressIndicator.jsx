function ProgressIndicator({ currentStep, totalSteps }) {
  return (
    <div className="progress-indicator">
      {Array.from({ length: totalSteps }, (_, index) => (
        <div 
          key={index} 
          className={`progress-dot ${index === currentStep - 1 ? 'active' : ''}`}
          aria-label={`Step ${index + 1} of ${totalSteps}`}
        />
      ))}
    </div>
  );
}

export default ProgressIndicator;
