import { useState, useEffect, useCallback } from 'react';
import api from '../api/client';
import type {
    ITeam,
    ITournamentEntry,
    ICreateTeamPayload,
    IUpdateTeamPayload,
    IUpdateEntryStatusPayload,
    IApiError,
} from '../types';

// ─── Squadre ──────────────────────────────────────────────────────────────────

interface IUseTeams {
    teams: ITeam[];
    loading: boolean;
    error: string | null;
    refetch: () => Promise<void>;
    create: (payload: ICreateTeamPayload) => Promise<ITeam>;
    update: (id: string, payload: IUpdateTeamPayload) => Promise<ITeam>;
    remove: (id: string) => Promise<void>;
}

export const useTeams = (): IUseTeams => {
    const [teams, setTeams] = useState<ITeam[]>([]);
    const [loading, setLoading] = useState(false);
    const [error, setError] = useState<string | null>(null);

    const fetch = useCallback(async () => {
        setLoading(true);
        setError(null);
        try {
            const { data } = await api.get<ITeam[]>('/teams');
            setTeams(data);
        } catch (err: unknown) {
            setError((err as { response?: { data?: IApiError } })?.response?.data?.error ?? 'Errore');
        } finally {
            setLoading(false);
        }
    }, []);

    useEffect(() => { void fetch(); }, [fetch]);

    const create = async (payload: ICreateTeamPayload): Promise<ITeam> => {
        const { data } = await api.post<ITeam>('/teams', payload);
        setTeams((prev) => [...prev, data]);
        return data;
    };

    const update = async (id: string, payload: IUpdateTeamPayload): Promise<ITeam> => {
        const { data } = await api.put<ITeam>(`/teams/${id}`, payload);
        setTeams((prev) => prev.map((t) => (t._id === id ? data : t)));
        return data;
    };

    const remove = async (id: string): Promise<void> => {
        await api.delete(`/teams/${id}`);
        setTeams((prev) => prev.filter((t) => t._id !== id));
    };

    return { teams, loading, error, refetch: fetch, create, update, remove };
};

// ─── Iscrizioni al torneo ─────────────────────────────────────────────────────

interface IUseTournamentEntries {
    entries: ITournamentEntry[];
    loading: boolean;
    error: string | null;
    refetch: () => Promise<void>;
    register: (teamId: string) => Promise<ITournamentEntry>;
    updateStatus: (teamId: string, payload: IUpdateEntryStatusPayload) => Promise<ITournamentEntry>;
    markPaid: (teamId: string) => Promise<ITournamentEntry>;
    remove: (teamId: string) => Promise<void>;
}

export const useTournamentEntries = (tournamentId: string): IUseTournamentEntries => {
    const [entries, setEntries] = useState<ITournamentEntry[]>([]);
    const [loading, setLoading] = useState(false);
    const [error, setError] = useState<string | null>(null);

    const fetch = useCallback(async () => {
        setLoading(true);
        setError(null);
        try {
            const { data } = await api.get<ITournamentEntry[]>(`/teams/tournament/${tournamentId}`);
            setEntries(data);
        } catch (err: unknown) {
            setError((err as { response?: { data?: IApiError } })?.response?.data?.error ?? 'Errore');
        } finally {
            setLoading(false);
        }
    }, [tournamentId]);

    useEffect(() => { void fetch(); }, [fetch]);

    const base = `/teams/tournament/${tournamentId}`;

    const register = async (teamId: string): Promise<ITournamentEntry> => {
        const { data } = await api.post<ITournamentEntry>(`${base}/${teamId}`);
        setEntries((prev) => [...prev, data]);
        return data;
    };

    const updateStatus = async (
        teamId: string,
        payload: IUpdateEntryStatusPayload
    ): Promise<ITournamentEntry> => {
        const { data } = await api.patch<ITournamentEntry>(`${base}/${teamId}/status`, payload);
        setEntries((prev) => prev.map((e) => (e.team._id === teamId ? data : e)));
        return data;
    };

    const markPaid = async (teamId: string): Promise<ITournamentEntry> => {
        const { data } = await api.patch<ITournamentEntry>(`${base}/${teamId}/paid`);
        setEntries((prev) => prev.map((e) => (e.team._id === teamId ? data : e)));
        return data;
    };

    const remove = async (teamId: string): Promise<void> => {
        await api.delete(`${base}/${teamId}`);
        setEntries((prev) => prev.filter((e) => e.team._id !== teamId));
    };

    return { entries, loading, error, refetch: fetch, register, updateStatus, markPaid, remove };
};