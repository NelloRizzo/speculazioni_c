import { Routes, Route, Navigate } from 'react-router-dom';
import { useAuthContext } from './context/auth-context';

// Layouts
import MainLayout from './components/layout/MainLayout';
import AdminLayout from './components/layout/AdminLayout';

// Public pages
import HomePage from './pages/home/HomePage';
import LoginPage from './pages/auth/LoginPage';
import TournamentsPage from './pages/tournaments/TournamentsPage';
import TournamentDetailPage from './pages/tournaments/TournamentDetailPage';
import TeamsPage from './pages/teams/TeamsPage';
import MatchesPage from './pages/matches/MatchesPage';
import MatchDetailPage from './pages/matches/MatchDetailPage';
import NotFoundPage from './pages/NotFoundPage';

// Admin pages
import AdminPage from './pages/admin/AdminPage';
import AdminTournamentsPage from './pages/admin/tournaments/AdminTournamentsPage';

// ─── Guards ───────────────────────────────────────────────────────────────────

const ProtectedRoute = ({ children }: { children: React.ReactNode }) => {
  const { isAuthenticated } = useAuthContext();
  return isAuthenticated ? <>{children}</> : <Navigate to="/login" replace />;
};

const AdminRoute = ({ children }: { children: React.ReactNode }) => {
  const { isAdmin } = useAuthContext();
  return isAdmin ? <>{children}</> : <Navigate to="/" replace />;
};

// ─── App ──────────────────────────────────────────────────────────────────────

const App = () => {
  const { isAuthenticated } = useAuthContext();

  return (
    <Routes>
      {/* Login */}
      <Route
        path="/login"
        element={isAuthenticated ? <Navigate to="/" replace /> : <LoginPage />}
      />

      {/* Pubbliche */}
      <Route path="/" element={<MainLayout />}>
        <Route index element={<HomePage />} />
        <Route path="tournaments" element={<TournamentsPage />} />
        <Route path="tournaments/:id" element={<TournamentDetailPage />} />
        <Route path="tournaments/:id/teams" element={<TeamsPage />} />
        <Route path="tournaments/:id/matches" element={<MatchesPage />} />
        <Route path="matches/:matchId" element={<MatchDetailPage />} />
      </Route>

      {/* Admin */}
      <Route
        path="/admin"
        element={
          <ProtectedRoute>
            <AdminRoute>
              <AdminLayout />
            </AdminRoute>
          </ProtectedRoute>
        }
      >
        <Route index element={<AdminPage />} />
        <Route path="tournaments" element={<AdminTournamentsPage />} />
      </Route>

      <Route path="*" element={<NotFoundPage />} />
    </Routes>
  );
};

export default App;