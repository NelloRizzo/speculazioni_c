export interface IRole {
    _id: string;
    name: string;
    description: string;
    createdAt: string;
    updatedAt: string;
}

export interface IUser {
    _id: string;
    username: string;
    email: string;
    roles: IRole[];
    active: boolean;
    createdAt: string;
    updatedAt: string;
}

export interface IAuthResponse {
    token: string;
    user: IUser;
}

export interface ILoginPayload {
    email: string;
    password: string;
}

export interface ICreateUserPayload {
    username: string;
    email: string;
    password: string;
    roleNames?: string[];
}

export interface IUpdateUserPayload {
    username?: string;
    email?: string;
    password?: string;
}