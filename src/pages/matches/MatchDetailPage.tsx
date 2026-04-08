import { useParams } from 'react-router-dom';
import { useScore } from '../../hooks/use-matches';
import styles from './MatchDetailPage.module.scss';

const MatchDetailPage = () => {
    const { matchId = '' } = useParams<{ matchId: string }>();
    const { match, loading, error } = useScore(matchId);

    if (loading) return <div className={styles.state}>Caricamento incontro...</div>;
    if (error) return <div className={`${styles.state} ${styles.error}`}>{error}</div>;
    if (!match) return <div className={styles.state}>Incontro non trovato.</div>;

    return (
        <div className={styles.page}>
            <div className={styles.teams}>
                <span>{match.team1.name}</span>
                <span className={styles.vs}>vs</span>
                <span>{match.team2.name}</span>
            </div>
            <p className={styles.status}>{match.status}</p>
        </div>
    );
};

export default MatchDetailPage;