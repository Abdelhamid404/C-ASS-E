// Authentication and Session Management
class AuthManager {
    constructor() {
        this.currentUser = null;
        this.loadSession();
    }

    loadSession() {
        const saved = localStorage.getItem('nctu_sis_user') || localStorage.getItem('currentUser');
        if (saved) {
            try {
                this.currentUser = JSON.parse(saved);
            } catch (e) {
                console.error('Error loading session:', e);
                localStorage.removeItem('nctu_sis_user');
                localStorage.removeItem('currentUser');
            }
        }
    }

    saveSession(user) {
        this.currentUser = user;
        localStorage.setItem('nctu_sis_user', JSON.stringify(user));
        localStorage.setItem('currentUser', JSON.stringify(user));
    }

    clearSession() {
        this.currentUser = null;
        localStorage.removeItem('nctu_sis_user');
        localStorage.removeItem('currentUser');
    }

    isAuthenticated() {
        return this.currentUser !== null;
    }

    getUser() {
        return this.currentUser;
    }

    getRole() {
        return this.currentUser ? this.currentUser.roleId : null;
    }

    isStudent() {
        return this.getRole() === 'ROLE_STUDENT';
    }

    isProfessor() {
        return this.getRole() === 'ROLE_PROFESSOR';
    }

    isStudentAffairs() {
        return this.getRole() === 'ROLE_STUDENT_AFFAIRS';
    }

    isSuperAdmin() {
        return this.getRole() === 'ROLE_SUPERADMIN';
    }

    async login(username, password) {
        try {
            const user = await API.login(username, password);
            if (user) {
                this.saveSession(user);
                return user;
            }
            throw new Error('Login failed');
        } catch (error) {
            throw error;
        }
    }

    async logout() {
        try {
            await API.logout();
        } catch (error) {
            console.error('Logout error:', error);
        }
        this.clearSession();
        window.location.href = 'index.html';
    }

    requireAuth() {
        if (!this.isAuthenticated()) {
            window.location.href = 'index.html';
            return false;
        }
        return true;
    }

    requireRole(role) {
        if (!this.requireAuth()) {
            return false;
        }
        if (this.getRole() !== role) {
            alert('You do not have permission to access this page.');
            window.location.href = 'dashboard.html';
            return false;
        }
        return true;
    }

    requireRoles(roles) {
        if (!this.requireAuth()) {
            return false;
        }
        const role = this.getRole();
        if (!roles.includes(role)) {
            alert('You do not have permission to access this page.');
            window.location.href = 'dashboard.html';
            return false;
        }
        return true;
    }
}

// Global auth manager instance
const auth = new AuthManager();
