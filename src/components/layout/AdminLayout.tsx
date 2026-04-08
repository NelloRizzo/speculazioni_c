import { Outlet, NavLink, Link } from 'react-router-dom';
import { useAuthContext } from '../../context/auth-context';
import logo from '../../assets/scopone.png';
import styles from './AdminLayout.module.scss';

const NAV_ITEMS = [
    { to: '/admin', label: 'Dashboard', icon: '◈', end: true },
    { to: '/admin/tournaments', label: 'Tornei', icon: '⚔', end: false },
];

const AdminLayout = () => {
    const { user, logout } = useAuthContext();

    return (
        <div className={styles.layout}>

            {/* ── Sidebar ── */}
            <aside className={styles.sidebar}>
                <div className={styles.sideTop}>
                    <Link to="/" className={styles.brand}>
                        <img src={logo} alt="Scopone" className={styles.brandLogo} />
                        <div className={styles.brandText}>
                            <span className={styles.brandName}>Scopone</span>
                            <span className={styles.brandSub}>Pannello Admin</span>
                        </div>
                    </Link>

                    <div className={styles.divider} />

                    <nav className={styles.nav}>
                        {NAV_ITEMS.map(({ to, label, icon, end }) => (
                            <NavLink
                                key={to}
                                to={to}
                                end={end}
                                className={({ isActive }) =>
                                    `${styles.navItem} ${isActive ? styles.active : ''}`
                                }
                            >
                                <span className={styles.navIcon}>{icon}</span>
                                <span>{label}</span>
                            </NavLink>
                        ))}
                    </nav>
                </div>

                <div className={styles.sideBottom}>
                    <div className={styles.divider} />
                    <div className={styles.userInfo}>
                        <div className={styles.userAvatar}>
                            {user?.username?.charAt(0).toUpperCase()}
                        </div>
                        <div className={styles.userMeta}>
                            <span className={styles.userName}>{user?.username}</span>
                            <span className={styles.userRole}>Amministratore</span>
                        </div>
                    </div>
                    <button onClick={logout} className={styles.logoutBtn}>
                        Esci
                    </button>
                </div>
            </aside>

            {/* ── Content ── */}
            <main className={styles.main}>
                <Outlet />
            </main>

        </div>
    );
};

export default AdminLayout;