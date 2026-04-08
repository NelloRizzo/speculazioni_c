import { createContext, useContext, useState, useEffect, type ReactNode } from 'react';
import { type IUser } from '../types';

interface IAuthContext {
    user: IUser | null;
    token: string | null;
    isAuthenticated: boolean;
    isAdmin: boolean;
    isReferee: boolean;
    login: (token: string, user: IUser) => void;
    logout: () => void;
}

const AuthContext = createContext<IAuthContext | null>(null);

const TOKEN_KEY = 'scopone_token';
const USER_KEY = 'scopone_user';

export const AuthProvider = ({ children }: { children: ReactNode }) => {
    const [token, setToken] = useState<string | null>(
        () => localStorage.getItem(TOKEN_KEY)
    );
    const [user, setUser] = useState<IUser | null>(() => {
        const stored = localStorage.getItem(USER_KEY);
        return stored ? (JSON.parse(stored) as IUser) : null;
    });

    // Sincronizza localStorage quando token/user cambiano
    useEffect(() => {
        if (token && user) {
            localStorage.setItem(TOKEN_KEY, token);
            localStorage.setItem(USER_KEY, JSON.stringify(user));
        } else {
            localStorage.removeItem(TOKEN_KEY);
            localStorage.removeItem(USER_KEY);
        }
    }, [token, user]);

    const login = (newToken: string, newUser: IUser) => {
        setToken(newToken);
        setUser(newUser);
    };

    const logout = () => {
        setToken(null);
        setUser(null);
    };

    const hasRole = (roleName: string) =>
        user?.roles.some((r) => r.name === roleName) ?? false;

    const value: IAuthContext = {
        user,
        token,
        isAuthenticated: !!token && !!user,
        isAdmin: hasRole('admin'),
        isReferee: hasRole('referee'),
        login,
        logout,
    };

    return <AuthContext.Provider value={value} > {children} </AuthContext.Provider>;
};

// eslint-disable-next-line react-refresh/only-export-components
export const useAuthContext = (): IAuthContext => {
    const ctx = useContext(AuthContext);
    if (!ctx) throw new Error('useAuthContext must be used within AuthProvider');
    return ctx;
};