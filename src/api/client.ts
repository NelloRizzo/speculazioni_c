import axios from 'axios';

const api = axios.create({
    baseURL: import.meta.env.VITE_API_URL as string ?? 'http://localhost:3000/api',
    headers: { 'Content-Type': 'application/json' },
});

// Inietta il token JWT su ogni richiesta se presente
api.interceptors.request.use((config) => {
    const token = localStorage.getItem('scopone_token');
    if (token) config.headers.Authorization = `Bearer ${token}`;
    return config;
});

// Reindirizza al login se il token è scaduto
api.interceptors.response.use(
    (response) => response,
    (error: unknown) => {
        const status = (error as { response?: { status?: number } })?.response?.status;
        if (status === 401) {
            localStorage.removeItem('scopone_token');
            localStorage.removeItem('scopone_user');
            window.location.href = '/login';
        }
        return Promise.reject(error);
    }
);

export default api;