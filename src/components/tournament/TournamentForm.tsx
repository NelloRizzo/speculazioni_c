import { useState, type SubmitEvent, useEffect } from 'react';
import type { ITournament, ICreateTournamentPayload, IPrizeEntry } from '../../types';
import styles from './TournamentForm.module.scss';

interface Props {
    initial?: ITournament;
    onSave: (payload: ICreateTournamentPayload, poster?: File) => Promise<void>;
    onClose: () => void;
}

const EMPTY: ICreateTournamentPayload = {
    name: '',
    format: 'round_robin',
    startDate: '',
    endDate: '',
    location: '',
    description: '',
    rules: '',
    entryFee: undefined,
    minTeams: 4,
    prizePool: 0,
    prizeDistribution: [],
    tiebreak: false,
};

// Converte una ISO string in formato input date (YYYY-MM-DD)
const toDateInput = (iso?: string | null) =>
    iso ? iso.slice(0, 10) : '';

const TournamentForm = ({ initial, onSave, onClose }: Props) => {
    const isEdit = !!initial?._id;

    const [fields, setFields] = useState<ICreateTournamentPayload>(() =>
        initial
            ? {
                name: initial.name,
                format: initial.format,
                startDate: toDateInput(initial.startDate),
                endDate: toDateInput(initial.endDate),
                location: initial.location ?? '',
                description: initial.description ?? '',
                rules: initial.rules ?? '',
                entryFee: initial.entryFee ?? undefined,
                minTeams: initial.minTeams,
                prizePool: initial.prizePool,
                prizeDistribution: initial.prizeDistribution ?? [],
                tiebreak: initial.tiebreak,
            }
            : EMPTY
    );

    const [poster, setPoster] = useState<File | undefined>();
    const [posterPreview, setPosterPreview] = useState<string | null>(initial?.posterUrl ?? null);
    const [prizeInput, setPrizeInput] = useState<{ position: string; points: string }>({
        position: '',
        points: '',
    });
    const [loading, setLoading] = useState(false);
    const [error, setError] = useState<string | null>(null);

    // Revoca l'object URL al cleanup
    useEffect(() => {
        return () => {
            if (posterPreview && posterPreview.startsWith('blob:')) {
                URL.revokeObjectURL(posterPreview);
            }
        };
    }, [posterPreview]);

    const set = <K extends keyof ICreateTournamentPayload>(
        key: K,
        value: ICreateTournamentPayload[K]
    ) => setFields((f) => ({ ...f, [key]: value }));

    const handlePoster = (e: React.ChangeEvent<HTMLInputElement>) => {
        const file = e.target.files?.[0];
        if (!file) return;
        setPoster(file);
        setPosterPreview(URL.createObjectURL(file));
    };

    const addPrize = () => {
        const pos = parseInt(prizeInput.position, 10);
        const pts = parseInt(prizeInput.points, 10);
        if (isNaN(pos) || isNaN(pts) || pos < 1 || pts < 0) return;
        if (fields.prizeDistribution?.some((e) => e.position === pos)) return;
        const updated = [...(fields.prizeDistribution ?? []), { position: pos, points: pts }]
            .sort((a, b) => a.position - b.position);
        set('prizeDistribution', updated);
        setPrizeInput({ position: '', points: '' });
    };

    const removePrize = (pos: number) =>
        set('prizeDistribution', fields.prizeDistribution?.filter((e) => e.position !== pos) ?? []);

    const handleSubmit = async (e: SubmitEvent) => {
        e.preventDefault();
        setError(null);
        setLoading(true);
        try {
            const payload: ICreateTournamentPayload = {
                ...fields,
                startDate: fields.startDate,
                endDate: fields.endDate || undefined,
                entryFee: fields.entryFee !== undefined && fields.entryFee !== null
                    ? Number(fields.entryFee)
                    : undefined,
                prizePool: Number(fields.prizePool),
                minTeams: Number(fields.minTeams),
            };
            await onSave(payload, poster);
            onClose();
        } catch (err: unknown) {
            setError(
                (err as { response?: { data?: { error?: string } } })?.response?.data?.error ??
                'Errore durante il salvataggio'
            );
        } finally {
            setLoading(false);
        }
    };

    const prizeTotal = fields.prizeDistribution?.reduce((s, e) => s + e.points, 0) ?? 0;

    return (
        <div className={styles.overlay} onClick={(e) => e.target === e.currentTarget && onClose()}>
            <div className={styles.modal}>

                {/* Header */}
                <div className={styles.modalHeader}>
                    <h2>{isEdit ? `Modifica "${initial?.name}"` : 'Nuovo torneo'}</h2>
                    <button className={styles.closeBtn} onClick={onClose} aria-label="Chiudi">✕</button>
                </div>

                <form onSubmit={handleSubmit} className={styles.form}>

                    {/* ── Sezione base ── */}
                    <fieldset className={styles.fieldset}>
                        <legend>Informazioni base</legend>

                        <div className={styles.field}>
                            <label>Nome torneo *</label>
                            <input
                                type="text"
                                value={fields.name}
                                onChange={(e) => set('name', e.target.value)}
                                placeholder="es. Torneo Primavera 2025"
                                required
                            />
                        </div>

                        <div className={styles.row2}>
                            <div className={styles.field}>
                                <label>Formato *</label>
                                <select
                                    value={fields.format}
                                    onChange={(e) => set('format', e.target.value as 'round_robin' | 'knockout')}
                                >
                                    <option value="round_robin">Girone all'italiana</option>
                                    <option value="knockout">Eliminazione diretta</option>
                                </select>
                            </div>
                            <div className={styles.field}>
                                <label>Min. squadre *</label>
                                <input
                                    type="number"
                                    min={4}
                                    value={fields.minTeams}
                                    onChange={(e) => set('minTeams', parseInt(e.target.value, 10))}
                                    required
                                />
                            </div>
                        </div>

                        <div className={styles.row2}>
                            <div className={styles.field}>
                                <label>Data inizio *</label>
                                <input
                                    type="date"
                                    value={fields.startDate}
                                    onChange={(e) => set('startDate', e.target.value)}
                                    required
                                />
                            </div>
                            <div className={styles.field}>
                                <label>Data fine</label>
                                <input
                                    type="date"
                                    value={fields.endDate ?? ''}
                                    onChange={(e) => set('endDate', e.target.value)}
                                />
                            </div>
                        </div>

                        <div className={styles.row2}>
                            <div className={styles.field}>
                                <label>Sede</label>
                                <input
                                    type="text"
                                    value={fields.location ?? ''}
                                    onChange={(e) => set('location', e.target.value)}
                                    placeholder="es. Ascea"
                                />
                            </div>
                            <div className={styles.field}>
                                <label>Quota iscrizione (€)</label>
                                <input
                                    type="number"
                                    min={0}
                                    step="0.01"
                                    value={fields.entryFee ?? ''}
                                    onChange={(e) =>
                                        set('entryFee', e.target.value ? parseFloat(e.target.value) : undefined)
                                    }
                                    placeholder="es. 20"
                                />
                            </div>
                        </div>

                        <div className={styles.field}>
                            <label>Descrizione</label>
                            <textarea
                                rows={2}
                                value={fields.description ?? ''}
                                onChange={(e) => set('description', e.target.value)}
                                placeholder="Breve descrizione del torneo..."
                            />
                        </div>
                    </fieldset>

                    {/* ── Regolamento ── */}
                    <fieldset className={styles.fieldset}>
                        <legend>Regolamento</legend>
                        <div className={styles.field}>
                            <label>Testo in Markdown</label>
                            <textarea
                                rows={6}
                                value={fields.rules ?? ''}
                                onChange={(e) => set('rules', e.target.value)}
                                placeholder="## Regolamento&#10;&#10;Scrivi il regolamento in Markdown..."
                                className={styles.mono}
                            />
                        </div>
                    </fieldset>

                    {/* ── Locandina ── */}
                    <fieldset className={styles.fieldset}>
                        <legend>Locandina</legend>
                        <div className={styles.posterRow}>
                            {posterPreview && (
                                <img src={posterPreview} alt="Anteprima" className={styles.posterPreview} />
                            )}
                            <div className={styles.field}>
                                <label>Immagine (max 5MB)</label>
                                <input type="file" accept="image/*" onChange={handlePoster} />
                            </div>
                        </div>
                    </fieldset>

                    {/* ── Punti ── */}
                    <fieldset className={styles.fieldset}>
                        <legend>Monte punti</legend>

                        <div className={styles.row2}>
                            <div className={styles.field}>
                                <label>Prize pool totale</label>
                                <input
                                    type="number"
                                    min={0}
                                    value={fields.prizePool}
                                    onChange={(e) => set('prizePool', parseInt(e.target.value, 10) || 0)}
                                />
                            </div>
                            <div className={styles.field}>
                                <label>Spareggio in caso di parità</label>
                                <div className={styles.toggle}>
                                    <input
                                        type="checkbox"
                                        id="tiebreak"
                                        checked={fields.tiebreak}
                                        onChange={(e) => set('tiebreak', e.target.checked)}
                                    />
                                    <label htmlFor="tiebreak" className={styles.toggleLabel}>
                                        {fields.tiebreak ? 'Sì' : 'No'}
                                    </label>
                                </div>
                            </div>
                        </div>

                        {/* Distribuzione premi */}
                        <div className={styles.prizeSection}>
                            <p className={styles.prizeTitle}>Distribuzione per posizione</p>
                            <div className={styles.prizeAdd}>
                                <input
                                    type="number"
                                    min={1}
                                    placeholder="Pos."
                                    value={prizeInput.position}
                                    onChange={(e) => setPrizeInput((p) => ({ ...p, position: e.target.value }))}
                                    className={styles.prizeSmall}
                                />
                                <input
                                    type="number"
                                    min={0}
                                    placeholder="Punti"
                                    value={prizeInput.points}
                                    onChange={(e) => setPrizeInput((p) => ({ ...p, points: e.target.value }))}
                                    className={styles.prizeSmall}
                                />
                                <button type="button" className={styles.prizeAddBtn} onClick={addPrize}>
                                    + Aggiungi
                                </button>
                            </div>

                            {(fields.prizeDistribution?.length ?? 0) > 0 && (
                                <div className={styles.prizeList}>
                                    {fields.prizeDistribution?.map((e: IPrizeEntry) => (
                                        <div key={e.position} className={styles.prizeRow}>
                                            <span className={styles.prizePos}>#{e.position}</span>
                                            <span className={styles.prizePts}>{e.points} pt</span>
                                            <button
                                                type="button"
                                                className={styles.prizeRemove}
                                                onClick={() => removePrize(e.position)}
                                            >
                                                ✕
                                            </button>
                                        </div>
                                    ))}
                                    <p className={styles.prizeTotal}>
                                        Totale distribuito: <strong>{prizeTotal}</strong> / {fields.prizePool} pt
                                        {prizeTotal > (fields.prizePool ?? 0) && (
                                            <span className={styles.prizeWarn}> ⚠ supera il prize pool</span>
                                        )}
                                    </p>
                                </div>
                            )}
                        </div>
                    </fieldset>

                    {/* Error & actions */}
                    {error && <p className={styles.error}>{error}</p>}

                    <div className={styles.formActions}>
                        <button type="button" className={styles.btnCancel} onClick={onClose}>
                            Annulla
                        </button>
                        <button type="submit" className={styles.btnSave} disabled={loading}>
                            {loading ? 'Salvataggio...' : isEdit ? 'Salva modifiche' : 'Crea torneo'}
                        </button>
                    </div>

                </form>
            </div>
        </div>
    );
};

export default TournamentForm;