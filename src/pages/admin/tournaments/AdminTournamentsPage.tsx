import { useState } from 'react';
import { useTournaments } from '../../../hooks/use-tournaments';
import type { ITournament } from '../../../types';
import styles from './AdminTournamentsPage.module.scss';
import TournamentForm from '../../../components/tournament/TournamentForm';

const PHASE_LABEL: Record<string, string> = {
    registration: 'Iscrizioni',
    in_progress: 'In corso',
    completed: 'Concluso',
};

const FORMAT_LABEL: Record<string, string> = {
    round_robin: 'All\'italiana',
    knockout: 'Eliminazione',
};

const PHASE_ORDER: Record<string, string> = {
    registration: 'Avvia torneo →',
    in_progress: 'Concludi torneo →',
    completed: '',
};

// ── Row ───────────────────────────────────────────────────────────────────────

const TournamentRow = ({
    t,
    onAdvance,
    onDelete,
    onEdit,
}: {
    t: ITournament;
    onAdvance: (t: ITournament) => void;
    onDelete: (t: ITournament) => void;
    onEdit: (t: ITournament) => void;
}) => (
    <div className={styles.row}>
        <div className={styles.rowMain}>
            <div className={styles.rowInfo}>
                <span className={`${styles.phaseBadge} ${styles[t.phase]}`}>
                    {PHASE_LABEL[t.phase]}
                </span>
                <span className={styles.rowName}>{t.name}</span>
                <span className={styles.rowFormat}>{FORMAT_LABEL[t.format]}</span>
            </div>
            <div className={styles.rowMeta}>
                <span>{new Date(t.startDate).toLocaleDateString('it-IT')}</span>
                {t.prizePool > 0 && <span className={styles.prize}>★ {t.prizePool} pt</span>}
            </div>
        </div>

        <div className={styles.rowActions}>
            {t.phase !== 'completed' && (
                <button
                    className={styles.btnAdvance}
                    onClick={() => onAdvance(t)}
                    title={PHASE_ORDER[t.phase]}
                >
                    {PHASE_ORDER[t.phase]}
                </button>
            )}
            {t.phase === 'registration' && (
                <button className={styles.btnEdit} onClick={() => onEdit(t)}>
                    Modifica
                </button>
            )}
            {t.phase === 'registration' && (
                <button className={styles.btnDelete} onClick={() => onDelete(t)}>
                    Elimina
                </button>
            )}
        </div>
    </div>
);

// ── Confirm modal ─────────────────────────────────────────────────────────────

const ConfirmModal = ({
    message,
    onConfirm,
    onCancel,
}: {
    message: string;
    onConfirm: () => void;
    onCancel: () => void;
}) => (
    <div className={styles.overlay}>
        <div className={styles.modal}>
            <p className={styles.modalMsg}>{message}</p>
            <div className={styles.modalActions}>
                <button className={styles.btnCancel} onClick={onCancel}>Annulla</button>
                <button className={styles.btnConfirm} onClick={onConfirm}>Conferma</button>
            </div>
        </div>
    </div>
);

// ── Main page ─────────────────────────────────────────────────────────────────

const AdminTournamentsPage = () => {
    const { tournaments, loading, error, create, update, remove, advancePhase } = useTournaments();
    const [pending, setPending] = useState<{ action: 'delete' | 'advance'; tournament: ITournament } | null>(null);
    const [editTarget, setEditTarget] = useState<ITournament | null>(null);

    const handleDelete = async () => {
        if (!pending || pending.action !== 'delete') return;
        await remove(pending.tournament._id);
        setPending(null);
    };

    const handleAdvance = async () => {
        if (!pending || pending.action !== 'advance') return;
        await advancePhase(pending.tournament._id);
        setPending(null);
    };

    return (
        <div className={styles.page}>
            <div className={styles.header}>
                <div>
                    <h1 className={styles.title}>Tornei</h1>
                    <p className={styles.sub}>{tournaments.length} tornei registrati</p>
                </div>
                <button
                    className={styles.btnNew}
                    onClick={() => setEditTarget({} as ITournament)}
                >
                    + Nuovo torneo
                </button>
            </div>

            {loading && <p className={styles.state}>Caricamento...</p>}
            {error && <p className={`${styles.state} ${styles.err}`}>{error}</p>}

            {!loading && tournaments.length === 0 && (
                <p className={styles.empty}>Nessun torneo. Creane uno!</p>
            )}

            {!loading && tournaments.length > 0 && (
                <div className={styles.list}>
                    {tournaments.map((t) => (
                        <TournamentRow
                            key={t._id}
                            t={t}
                            onAdvance={(t) => setPending({ action: 'advance', tournament: t })}
                            onDelete={(t) => setPending({ action: 'delete', tournament: t })}
                            onEdit={setEditTarget}
                        />
                    ))}
                </div>
            )}

            {/* Confirm delete */}
            {pending?.action === 'delete' && (
                <ConfirmModal
                    message={`Eliminare il torneo "${pending.tournament.name}"? L'operazione non è reversibile.`}
                    onConfirm={handleDelete}
                    onCancel={() => setPending(null)}
                />
            )}

            {/* Confirm advance phase */}
            {pending?.action === 'advance' && (
                <ConfirmModal
                    message={`Avanzare "${pending.tournament.name}" alla fase successiva? Il torneo non potrà più essere modificato.`}
                    onConfirm={handleAdvance}
                    onCancel={() => setPending(null)}
                />
            )}

            {/* Create / Edit form */}
            {editTarget && (
                <TournamentForm
                    initial={editTarget._id ? editTarget : undefined}
                    onSave={async (payload, poster) => {
                        if (editTarget._id) {
                            await update(editTarget._id, payload, poster);
                        } else {
                            await create(payload, poster);
                        }
                    }}
                    onClose={() => setEditTarget(null)}
                />
            )}
        </div>
    );
};

export default AdminTournamentsPage;