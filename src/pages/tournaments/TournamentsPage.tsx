import { useTournaments } from '../../hooks/use-tournaments';
import styles from './TournamentsPage.module.scss';

const TournamentsPage = () => {
    const { tournaments, loading, error } = useTournaments();

    if (loading) return <div className={styles.state}>Caricamento tornei...</div>;
    if (error) return <div className={styles.state + ' ' + styles.error}>{error}</div>;

    return (
        <div className={styles.page}>
            <div className={styles.header}>
                <h1>Tornei</h1>
            </div>

            {tournaments.length === 0 ? (
                <p className={styles.empty}>Nessun torneo disponibile.</p>
            ) : (
                <div className={styles.grid}>
                    {tournaments.map((t) => (
                        <div key={t._id} className={styles.card}>
                            <p>{t.name}</p>
                        </div>
                    ))}
                </div>
            )}
        </div>
    );
};

export default TournamentsPage;