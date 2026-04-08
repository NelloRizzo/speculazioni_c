import { useState } from 'react';
import { useAuthContext } from '../context/auth-context';
import type { ILoginPayload, IAuthResponse, IApiError } from '../types';
import api from '../api/client';

interface IUseAuth {
    login: (payload: ILoginPayload) => Promise<void>;
    logout: () => void;
    loading: boolean;
    error: string | null;
}

export const useAuth = (): IUseAuth => {
    const { login: ctxLogin, logout: ctxLogout } = useAuthContext();
    const [loading, setLoading] = useState(false);
    const [error, setError] = useState<string | null>(null);

    const login = async (payload: ILoginPayload): Promise<void> => {
        setLoading(true);
        setError(null);
        try {
            const { data } = await api.post<IAuthResponse>('/users/login', payload);
            ctxLogin(data.token, data.user);
        } catch (err: unknown) {
            const message =
                (err as { response?: { data?: IApiError } })?.response?.data?.error ??
                'Errore durante il login';
            setError(message);
        } finally {
            setLoading(false);
        }
    };

    const logout = () => {
        ctxLogout();
    };

    return { login, logout, loading, error };
};