import { useEffect, useState } from 'react';
import { Link } from 'react-router-dom';
import { useTournaments } from '../../hooks/use-tournaments';
import type { ITournament } from '../../types';
import styles from './HomePage.module.scss';
import logo from '../../assets/scopone.png';

const PHASE_LABEL: Record<string, string> = {
    registration: 'Iscrizioni aperte',
    in_progress: 'In corso',
    completed: 'Concluso',
};

const FORMAT_LABEL: Record<string, string> = {
    round_robin: 'Girone all\'italiana',
    knockout: 'Eliminazione diretta',
};

const TournamentCard = ({ t }: { t: ITournament }) => (
    <Link to={`/tournaments/${t._id}`} className={styles.card}>
        <div className={styles.cardInner}>
            {t.posterUrl && (
                <div className={styles.cardPoster}>
                    <img src={t.posterUrl} alt={t.name} />
                </div>
            )}
            <div className={styles.cardBody}>
                <span className={`${styles.phase} ${styles[t.phase]}`}>
                    {PHASE_LABEL[t.phase]}
                </span>
                <h3 className={styles.cardName}>{t.name}</h3>
                <p className={styles.cardFormat}>{FORMAT_LABEL[t.format]}</p>
                {t.location && (
                    <p className={styles.cardLocation}>⚑ {t.location}</p>
                )}
                <div className={styles.cardFooter}>
                    <span>{new Date(t.startDate).toLocaleDateString('it-IT', { day: 'numeric', month: 'long', year: 'numeric' })}</span>
                    {t.prizePool > 0 && <span className={styles.prize}>★ {t.prizePool} pt</span>}
                </div>
            </div>
        </div>
    </Link>
);

const HomePage = () => {
    const { tournaments, loading } = useTournaments();
    const [visible, setVisible] = useState(false);

    useEffect(() => {
        const t = setTimeout(() => setVisible(true), 80);
        return () => clearTimeout(t);
    }, []);

    const active = tournaments.filter((t) => t.phase === 'in_progress');
    const open = tournaments.filter((t) => t.phase === 'registration');
    const completed = tournaments.filter((t) => t.phase === 'completed').slice(0, 3);

    return (
        <div className={`${styles.page} ${visible ? styles.visible : ''}`}>

            {/* ── Hero ─────────────────────────────────────────── */}
            <section className={styles.hero}>
                <div className={styles.heroOrb} aria-hidden />
                <div className={styles.heroContent}>
                    <img src={logo} alt="Logo Scopone" className={styles.heroLogo} />
                    <div className={styles.heroText}>
                        <p className={styles.heroEyebrow}>Scopone Scientifico</p>
                        <h1 className={styles.heroTitle}>Scarti Frusci e Pigli Primera</h1>
                        <p className={styles.heroSub}>
                            Passare il tempo insieme divertendosi.
                        </p>
                        <div className={styles.heroCtas}>
                            <Link to="/tournaments" className={styles.ctaPrimary}>
                                Vedi i tornei
                            </Link>
                        </div>
                    </div>
                </div>
                <div className={styles.heroDivider} aria-hidden>
                    <span>✦</span><span>✦</span><span>✦</span>
                </div>
            </section>

            {/* ── Tornei in corso ───────────────────────────────── */}
            {!loading && active.length > 0 && (
                <section className={styles.section}>
                    <div className={styles.sectionHeader}>
                        <span className={styles.sectionIcon}>⚔</span>
                        <h2>In corso</h2>
                    </div>
                    <div className={styles.grid}>
                        {active.map((t) => <TournamentCard key={t._id} t={t} />)}
                    </div>
                </section>
            )}

            {/* ── Iscrizioni aperte ────────────────────────────── */}
            {!loading && open.length > 0 && (
                <section className={styles.section}>
                    <div className={styles.sectionHeader}>
                        <span className={styles.sectionIcon}>✎</span>
                        <h2>Iscrizioni aperte</h2>
                    </div>
                    <div className={styles.grid}>
                        {open.map((t) => <TournamentCard key={t._id} t={t} />)}
                    </div>
                </section>
            )}

            {/* ── Ultimi conclusi ──────────────────────────────── */}
            {!loading && completed.length > 0 && (
                <section className={styles.section}>
                    <div className={styles.sectionHeader}>
                        <span className={styles.sectionIcon}>◉</span>
                        <h2>Conclusi di recente</h2>
                    </div>
                    <div className={styles.grid}>
                        {completed.map((t) => <TournamentCard key={t._id} t={t} />)}
                    </div>
                </section>
            )}

            {/* ── Empty state ───────────────────────────────────── */}
            {!loading && tournaments.length === 0 && (
                <section className={styles.empty}>
                    <p className={styles.emptyIcon}>♠</p>
                    <p>Nessun torneo disponibile al momento.</p>
                </section>
            )}

            {/* ── Footer ornament ───────────────────────────────── */}
            <footer className={styles.foot}>
                <span>♣</span>
                <span>Scarti · Frusci · Pigli Primera</span>
                <span>♦</span>
            </footer>

        </div>
    );
};

export default HomePage;