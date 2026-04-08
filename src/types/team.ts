export interface ITeam {
    _id: string;
    player1: string;
    player2: string;
    name: string;
    createdAt: string;
    updatedAt: string;
}

export type EntryStatus = 'confirmed' | 'disqualified' | 'withdrawn';

export interface ITournamentEntry {
    _id: string;
    tournament: string;
    team: ITeam;
    status: EntryStatus;
    statusReason?: string | null;
    paid: boolean;
    createdAt: string;
    updatedAt: string;
}

export interface ICreateTeamPayload {
    player1: string;
    player2: string;
    name?: string;
}

export type IUpdateTeamPayload = Partial<ICreateTeamPayload>;

export interface IUpdateEntryStatusPayload {
    status: EntryStatus;
    statusReason?: string;
}