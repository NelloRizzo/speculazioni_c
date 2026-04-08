import { Link } from 'react-router-dom';
import { useAuthContext } from '../../context/auth-context';
import styles from './AdminPage.module.scss';

const MENU_ITEMS = [
    {
        to: '/admin/tournaments',
        icon: '⚔',
        label: 'Tornei',
        desc: 'Crea, modifica ed elimina i tornei. Gestisci fasi e accoppiamenti.',
    },
];

const AdminPage = () => {
    const { user } = useAuthContext();

    return (
        <div className={styles.page}>
            <div className={styles.header}>
                <p className={styles.welcome}>Benvenuto,</p>
                <h1 className={styles.title}>{user?.username}</h1>
                <p className={styles.sub}>Seleziona una sezione da gestire</p>
            </div>

            <div className={styles.grid}>
                {MENU_ITEMS.map(({ to, icon, label, desc }) => (
                    <Link key={to} to={to} className={styles.card}>
                        <span className={styles.cardIcon}>{icon}</span>
                        <div className={styles.cardBody}>
                            <h2 className={styles.cardLabel}>{label}</h2>
                            <p className={styles.cardDesc}>{desc}</p>
                        </div>
                        <span className={styles.cardArrow}>→</span>
                    </Link>
                ))}
            </div>
        </div>
    );
};

export default AdminPage;