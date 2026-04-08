import { Outlet, Link } from 'react-router-dom';
import { useAuthContext } from '../../context/auth-context';
import logo from '../../assets/scopone.png';
import styles from './MainLayout.module.scss';

const MainLayout = () => {
    const { isAuthenticated, isAdmin, logout } = useAuthContext();

    return (
        <div className={styles.layout}>
            <header className={styles.header}>
                <Link to="/" className={styles.brand}>
                    <img src={logo} alt="Scopone" className={styles.brandLogo} />
                    <span className={styles.brandName}>Scopone Scientifico</span>
                </Link>

                <nav className={styles.nav}>
                    <Link to="/tournaments" className={styles.navLink}>Tornei</Link>
                    {isAdmin && (
                        <Link to="/admin" className={styles.navLink}>Admin</Link>
                    )}
                    {isAuthenticated ? (
                        <button onClick={logout} className={styles.navButton}>Esci</button>
                    ) : (
                        <Link to="/login" className={styles.navLink}>Accedi</Link>
                    )}
                </nav>
            </header>

            <main className={styles.main}>
                <Outlet />
            </main>
        </div>
    );
};

export default MainLayout;