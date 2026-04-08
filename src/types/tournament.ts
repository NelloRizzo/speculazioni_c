export type TournamentFormat = 'round_robin' | 'knockout';
export type TournamentPhase = 'registration' | 'in_progress' | 'completed';

export interface IPrizeEntry {
    position: number;
    points: number;
}

export interface ITournament {
    _id: string;
    name: string;
    format: TournamentFormat;
    phase: TournamentPhase;
    startDate: string;
    endDate?: string | null;
    location?: string | null;
    description?: string | null;
    rules?: string | null;
    posterUrl?: string | null;
    posterPublicId?: string | null;
    entryFee?: number | null;
    minTeams: number;
    prizePool: number;
    prizeDistribution: IPrizeEntry[];
    tiebreak: boolean;
    createdAt: string;
    updatedAt: string;
}

export interface ICreateTournamentPayload {
    name: string;
    format: TournamentFormat;
    startDate: string;
    endDate?: string;
    location?: string;
    description?: string;
    rules?: string;
    entryFee?: number;
    minTeams: number;
    prizePool?: number;
    prizeDistribution?: IPrizeEntry[];
    tiebreak?: boolean;
}

export type IUpdateTournamentPayload = Partial<ICreateTournamentPayload>;