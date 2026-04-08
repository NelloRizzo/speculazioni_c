import { useState, useEffect, useCallback } from 'react';
import api from '../api/client';
import type {
    ITournament,
    ICreateTournamentPayload,
    IUpdateTournamentPayload,
    ITournamentFilters,
    IApiError,
} from '../types';

interface IUseTournaments {
    tournaments: ITournament[];
    loading: boolean;
    error: string | null;
    refetch: () => Promise<void>;
    create: (payload: ICreateTournamentPayload, poster?: File) => Promise<ITournament>;
    update: (id: string, payload: IUpdateTournamentPayload, poster?: File) => Promise<ITournament>;
    remove: (id: string) => Promise<void>;
    advancePhase: (id: string) => Promise<ITournament>;
}

export const useTournaments = (filters?: ITournamentFilters): IUseTournaments => {
    const [tournaments, setTournaments] = useState<ITournament[]>([]);
    const [loading, setLoading] = useState(false);
    const [error, setError] = useState<string | null>(null);

    const fetch = useCallback(async () => {
        setLoading(true);
        setError(null);
        try {
            const { data } = await api.get<ITournament[]>('/tournaments', { params: filters });
            setTournaments(data);
        } catch (err: unknown) {
            setError((err as { response?: { data?: IApiError } })?.response?.data?.error ?? 'Errore');
        } finally {
            setLoading(false);
        }
    }, [filters]);

    useEffect(() => { void fetch(); }, [fetch]);

    const buildFormData = (
        payload: ICreateTournamentPayload | IUpdateTournamentPayload,
        poster?: File
    ): FormData | typeof payload => {
        if (!poster) return payload;
        const form = new FormData();
        form.append('poster', poster);
        Object.entries(payload).forEach(([k, v]) => {
            if (v !== undefined && v !== null) {
                form.append(k, typeof v === 'object' ? JSON.stringify(v) : String(v));
            }
        });
        return form;
    };

    const create = async (
        payload: ICreateTournamentPayload,
        poster?: File
    ): Promise<ITournament> => {
        const body = buildFormData(payload, poster);
        const headers = poster ? { 'Content-Type': 'multipart/form-data' } : {};
        const { data } = await api.post<ITournament>('/tournaments', body, { headers });
        setTournaments((prev) => [data, ...prev]);
        return data;
    };

    const update = async (
        id: string,
        payload: IUpdateTournamentPayload,
        poster?: File
    ): Promise<ITournament> => {
        const body = buildFormData(payload, poster);
        const headers = poster ? { 'Content-Type': 'multipart/form-data' } : {};
        const { data } = await api.put<ITournament>(`/tournaments/${id}`, body, { headers });
        setTournaments((prev) => prev.map((t) => (t._id === id ? data : t)));
        return data;
    };

    const remove = async (id: string): Promise<void> => {
        await api.delete(`/tournaments/${id}`);
        setTournaments((prev) => prev.filter((t) => t._id !== id));
    };

    const advancePhase = async (id: string): Promise<ITournament> => {
        const { data } = await api.patch<ITournament>(`/tournaments/${id}/phase`);
        setTournaments((prev) => prev.map((t) => (t._id === id ? data : t)));
        return data;
    };

    return { tournaments, loading, error, refetch: fetch, create, update, remove, advancePhase };
};