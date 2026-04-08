import { useState, useEffect, useCallback } from 'react';
import api from '../api/client';
import type {
    IMatch,
    IMatchResult,
    IAddManchePayload,
    IGenerateMatchesPayload,
    IMatchFilters,
    IApiError,
} from '../types';

interface IUseMatches {
    matches: IMatch[];
    loading: boolean;
    error: string | null;
    refetch: () => Promise<void>;
    generate: (payload?: IGenerateMatchesPayload) => Promise<IMatch[]>;
    deleteAll: () => Promise<void>;
    scheduleMatch: (matchId: string, scheduledAt: string) => Promise<IMatch>;
}

export const useMatches = (
    tournamentId: string,
    filters?: IMatchFilters
): IUseMatches => {
    const [matches, setMatches] = useState<IMatch[]>([]);
    const [loading, setLoading] = useState(false);
    const [error, setError] = useState<string | null>(null);

    const fetch = useCallback(async () => {
        setLoading(true);
        setError(null);
        try {
            const { data } = await api.get<IMatch[]>(`/matches/${tournamentId}`, { params: filters });
            setMatches(data);
        } catch (err: unknown) {
            setError((err as { response?: { data?: IApiError } })?.response?.data?.error ?? 'Errore');
        } finally {
            setLoading(false);
        }
    }, [tournamentId, filters]);

    useEffect(() => { void fetch(); }, [fetch]);

    const generate = async (payload?: IGenerateMatchesPayload): Promise<IMatch[]> => {
        const { data } = await api.post<{ matches: IMatch[] }>(
            `/matches/${tournamentId}/generate`,
            payload ?? {}
        );
        setMatches(data.matches);
        return data.matches;
    };

    const deleteAll = async (): Promise<void> => {
        await api.delete(`/matches/${tournamentId}`);
        setMatches([]);
    };

    const scheduleMatch = async (matchId: string, scheduledAt: string): Promise<IMatch> => {
        const { data } = await api.patch<IMatch>(`/matches/match/${matchId}/schedule`, { scheduledAt });
        setMatches((prev) => prev.map((m) => (m._id === matchId ? data : m)));
        return data;
    };

    return { matches, loading, error, refetch: fetch, generate, deleteAll, scheduleMatch };
};

// ─── Punteggi di un singolo incontro ─────────────────────────────────────────

interface IUseScore {
    match: IMatch | null;
    loading: boolean;
    error: string | null;
    refetch: () => Promise<void>;
    start: () => Promise<IMatch>;
    addManche: (gameIndex: number, payload: IAddManchePayload) => Promise<IMatchResult>;
    removeLastManche: (gameIndex: number) => Promise<IMatch>;
}

export const useScore = (matchId: string): IUseScore => {
    const [match, setMatch] = useState<IMatch | null>(null);
    const [loading, setLoading] = useState(false);
    const [error, setError] = useState<string | null>(null);

    const fetch = useCallback(async () => {
        setLoading(true);
        setError(null);
        try {
            const { data } = await api.get<IMatch>(`/scores/${matchId}`);
            setMatch(data);
        } catch (err: unknown) {
            setError((err as { response?: { data?: IApiError } })?.response?.data?.error ?? 'Errore');
        } finally {
            setLoading(false);
        }
    }, [matchId]);

    useEffect(() => { void fetch(); }, [fetch]);

    const start = async (): Promise<IMatch> => {
        const { data } = await api.patch<IMatch>(`/scores/${matchId}/start`);
        setMatch(data);
        return data;
    };

    const addManche = async (
        gameIndex: number,
        payload: IAddManchePayload
    ): Promise<IMatchResult> => {
        const { data } = await api.post<IMatchResult>(
            `/scores/${matchId}/game/${gameIndex}/manche`,
            payload
        );
        setMatch(data.match);
        return data;
    };

    const removeLastManche = async (gameIndex: number): Promise<IMatch> => {
        const { data } = await api.delete<IMatch>(
            `/scores/${matchId}/game/${gameIndex}/manche`
        );
        setMatch(data);
        return data;
    };

    return { match, loading, error, refetch: fetch, start, addManche, removeLastManche };
};