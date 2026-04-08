import { useParams } from 'react-router-dom';
import { useMatches } from '../../hooks/use-matches';
import styles from './MatchesPage.module.scss';

const MatchesPage = () => {
    const { id = '' } = useParams<{ id: string }>();
    const { matches, loading, error } = useMatches(id);

    if (loading) return <div className={styles.state}>Caricamento incontri...</div>;
    if (error) return <div className={`${styles.state} ${styles.error}`}>{error}</div>;

    return (
        <div className={styles.page}>
            <div className={styles.header}>
                <h1>Incontri</h1>
            </div>

            {matches.length === 0 ? (
                <p className={styles.empty}>Nessun incontro generato.</p>
            ) : (
                <ul className={styles.list}>
                    {matches.map((m) => (
                        <li key={m._id} className={styles.item}>
                            <span>{m.team1.name}</span>
                            <span className={styles.vs}>vs</span>
                            <span>{m.team2.name}</span>
                            <span className={styles.status}>{m.status}</span>
                        </li>
                    ))}
                </ul>
            )}
        </div>
    );
};

export default MatchesPage;