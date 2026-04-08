// Risposta generica con messaggio
export interface IMessageResponse {
    message: string;
}

// Risposta paginata (per future esigenze)
export interface IPaginatedResponse<T> {
    data: T[];
    total: number;
    page: number;
    limit: number;
}

// Errore API
export interface IApiError {
    error: string;
}

// Filtri comuni per le query
export interface ITournamentFilters {
    phase?: 'registration' | 'in_progress' | 'completed';
    format?: 'round_robin' | 'knockout';
}

export interface IMatchFilters {
    round?: number;
    type?: 'round_robin' | 'knockout' | 'tiebreak';
    status?: 'scheduled' | 'in_progress' | 'completed';
}