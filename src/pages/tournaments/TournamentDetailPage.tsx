import { useParams } from 'react-router-dom';
import styles from './TournamentDetailPage.module.scss';

const TournamentDetailPage = () => {
    const { id } = useParams<{ id: string }>();

    return (
        <div className={styles.page}>
            <h1>Dettaglio Torneo</h1>
            <p className={styles.id}>ID: {id}</p>
        </div>
    );
};

export default TournamentDetailPage;