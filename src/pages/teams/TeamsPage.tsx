import { useParams } from 'react-router-dom';
import { useTournamentEntries } from '../../hooks/use-teams';
import styles from './TeamsPage.module.scss';

const TeamsPage = () => {
    const { id = '' } = useParams<{ id: string }>();
    const { entries, loading, error } = useTournamentEntries(id);

    if (loading) return <div className={styles.state}>Caricamento squadre...</div>;
    if (error) return <div className={`${styles.state} ${styles.error}`}>{error}</div>;

    return (
        <div className={styles.page}>
            <div className={styles.header}>
                <h1>Squadre</h1>
            </div>

            {entries.length === 0 ? (
                <p className={styles.empty}>Nessuna squadra iscritta.</p>
            ) : (
                <ul className={styles.list}>
                    {entries.map((e) => (
                        <li key={e._id} className={styles.item}>
                            <span className={styles.name}>{e.team.name}</span>
                            <span className={styles.status}>{e.status}</span>
                        </li>
                    ))}
                </ul>
            )}
        </div>
    );
};

export default TeamsPage;