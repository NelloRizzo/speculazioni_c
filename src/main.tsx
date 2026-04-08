import { StrictMode } from 'react';
import { createRoot } from 'react-dom/client';
import { BrowserRouter } from 'react-router-dom';
import { AuthProvider } from './context/auth-context';
import { TournamentProvider } from './context/tournament-context';
import App from './App';
import './styles/globals.scss';

const root = document.getElementById('root');
if (!root) throw new Error('Root element not found');

createRoot(root).render(
  <StrictMode>
    <BrowserRouter>
      <AuthProvider>
        <TournamentProvider>
          <App />
        </TournamentProvider>
      </AuthProvider>
    </BrowserRouter>
  </StrictMode>
);