import React from 'react';

interface LogoProps {
  size?: number;
  animated?: boolean;
  className?: string;
}

/**
 * Dubai Bank AI Logo - Brain Wave D
 * 
 * Usage:
 *   <DubaiBankLogo />                    // Default 64px, animated
 *   <DubaiBankLogo size={120} />         // Larger size
 *   <DubaiBankLogo animated={false} />   // Static version
 */
export const DubaiBankLogo: React.FC<LogoProps> = ({ 
  size = 64, 
  animated = true,
  className = ''
}) => {
  const uniqueId = React.useId().replace(/:/g, '');
  
  return (
    <svg 
      width={size} 
      height={size} 
      viewBox="0 0 200 200" 
      fill="none" 
      xmlns="http://www.w3.org/2000/svg"
      className={className}
      role="img"
      aria-label="Dubai Bank Logo"
    >
      <defs>
        <linearGradient id={`gold-${uniqueId}`} x1="0%" y1="0%" x2="100%" y2="100%">
          <stop offset="0%" stopColor="#C9A227" />
          <stop offset="100%" stopColor="#E5C158" />
        </linearGradient>
        <linearGradient id={`bg-${uniqueId}`} x1="0%" y1="0%" x2="100%" y2="100%">
          <stop offset="0%" stopColor="#0D1B2A" />
          <stop offset="100%" stopColor="#1B3A4B" />
        </linearGradient>
        <filter id={`glow-${uniqueId}`} x="-50%" y="-50%" width="200%" height="200%">
          <feGaussianBlur stdDeviation="3" result="coloredBlur"/>
          <feMerge>
            <feMergeNode in="coloredBlur"/>
            <feMergeNode in="SourceGraphic"/>
          </feMerge>
        </filter>
        
        {animated && (
          <style>
            {`
              @keyframes wave-flow-${uniqueId} {
                0%, 100% {
                  d: path("M80 100 Q90 80 100 100 Q110 120 120 100 Q130 80 140 100");
                }
                50% {
                  d: path("M80 100 Q90 120 100 100 Q110 80 120 100 Q130 120 140 100");
                }
              }
              
              @keyframes pulse-${uniqueId} {
                0%, 100% {
                  opacity: 1;
                  r: 6;
                }
                50% {
                  opacity: 0.7;
                  r: 8;
                }
              }
              
              @keyframes glow-pulse-${uniqueId} {
                0%, 100% {
                  filter: url(#glow-${uniqueId}) drop-shadow(0 0 8px rgba(201, 162, 39, 0.4));
                }
                50% {
                  filter: url(#glow-${uniqueId}) drop-shadow(0 0 16px rgba(201, 162, 39, 0.7));
                }
              }
              
              .wave-path-${uniqueId} {
                animation: wave-flow-${uniqueId} 2s ease-in-out infinite;
              }
              
              .pulse-dot-${uniqueId} {
                animation: pulse-${uniqueId} 2s ease-in-out infinite;
              }
              
              .d-shape-${uniqueId} {
                animation: glow-pulse-${uniqueId} 3s ease-in-out infinite;
              }
            `}
          </style>
        )}
      </defs>
      
      {/* Background */}
      <rect width="200" height="200" rx="40" fill={`url(#bg-${uniqueId})`} />
      
      {/* D Shape */}
      <g 
        filter={`url(#glow-${uniqueId})`}
        className={animated ? `d-shape-${uniqueId}` : ''}
      >
        <path 
          d="M50 35 L50 165 L100 165 C145 165 170 135 170 100 C170 65 145 35 100 35 L50 35 Z
             M70 55 L100 55 C130 55 150 75 150 100 C150 125 130 145 100 145 L70 145 L70 55 Z" 
          fill={`url(#gold-${uniqueId})`}
          fillRule="evenodd"
        />
      </g>
      
      {/* Brain Wave / AI Signal */}
      <path 
        className={animated ? `wave-path-${uniqueId}` : ''}
        d="M80 100 Q90 80 100 100 Q110 120 120 100 Q130 80 140 100" 
        stroke="#0D1B2A" 
        strokeWidth="4" 
        fill="none"
        strokeLinecap="round"
      />
      
      {/* Center Pulse Dot */}
      <circle 
        className={animated ? `pulse-dot-${uniqueId}` : ''}
        cx="100" 
        cy="100" 
        r="6" 
        fill="#0D1B2A" 
      />
    </svg>
  );
};

/**
 * Full Logo with Text - for headers and branding
 */
export const DubaiBankLogoFull: React.FC<LogoProps & { showTagline?: boolean }> = ({ 
  size = 48,
  animated = true,
  showTagline = true,
  className = ''
}) => {
  return (
    <div 
      className={className}
      style={{ 
        display: 'flex', 
        alignItems: 'center', 
        gap: size * 0.25,
      }}
    >
      <DubaiBankLogo size={size} animated={animated} />
      <div style={{ display: 'flex', flexDirection: 'column' }}>
        <span style={{ 
          color: '#C9A227', 
          fontSize: size * 0.45,
          fontWeight: 700,
          letterSpacing: '0.5px',
          lineHeight: 1.1,
        }}>
          Dubai Bank
        </span>
        {showTagline && (
          <span style={{ 
            color: 'rgba(255,255,255,0.5)', 
            fontSize: size * 0.2,
            letterSpacing: '2px',
            textTransform: 'uppercase',
          }}>
            Think Forward
          </span>
        )}
      </div>
    </div>
  );
};
