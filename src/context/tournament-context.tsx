import { createContext, useContext, useState, type ReactNode } from 'react';
import { type ITournament } from '../types';

interface ITournamentContext {
    activeTournament: ITournament | null;
    setActiveTournament: (tournament: ITournament | null) => void;
}

const TournamentContext = createContext<ITournamentContext | null>(null);

export const TournamentProvider = ({ children }: { children: ReactNode }) => {
    const [activeTournament, setActiveTournament] = useState<ITournament | null>(null);

    return (
        <TournamentContext.Provider value={{ activeTournament, setActiveTournament }}>
            {children}
        </TournamentContext.Provider>
    );
};

// eslint-disable-next-line react-refresh/only-export-components
export const useTournamentContext = (): ITournamentContext => {
    const ctx = useContext(TournamentContext);
    if (!ctx) throw new Error('useTournamentContext must be used within TournamentProvider');
    return ctx;
};