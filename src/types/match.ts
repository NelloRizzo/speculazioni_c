import type { ITeam } from './team';

export type MatchStatus = 'scheduled' | 'in_progress' | 'completed';
export type MatchType = 'round_robin' | 'knockout' | 'tiebreak';
export type GameWinner = 'team1' | 'team2' | null;

export interface IManche {
    points1: number;
    points2: number;
    penalties1: number;
    penalties2: number;
}

export interface IGame {
    manches: IManche[];
    score1: number;
    score2: number;
    completed: boolean;
    winner: GameWinner;
}

export interface IMatch {
    _id: string;
    tournament: string;
    team1: ITeam;
    team2: ITeam;
    type: MatchType;
    round: number;
    bracketPosition?: number | null;
    games: IGame[];
    status: MatchStatus;
    winner?: ITeam | null;
    loser?: ITeam | null;
    scheduledAt?: string | null;
    completedAt?: string | null;
    createdAt: string;
    updatedAt: string;
}

export interface IAddManchePayload {
    points1: number;
    points2: number;
    penalties1?: number;
    penalties2?: number;
}

export interface IGenerateMatchesPayload {
    seededTeamIds?: string[];
}

export interface IMatchResult {
    match: IMatch;
    gameCompleted: boolean;
    matchCompleted: boolean;
}