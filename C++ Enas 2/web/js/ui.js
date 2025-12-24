// UI Helper Functions
class UIHelper {
    static showLoading(element) {
        if (element) {
            element.innerHTML = '<div class="text-center"><div class="spinner-border text-primary" role="status"><span class="visually-hidden">Loading...</span></div></div>';
        }
    }

    static showError(message, container) {
        if (container) {
            container.innerHTML = `<div class="alert alert-danger" role="alert">${message}</div>`;
        } else {
            alert(message);
        }
    }

    static showSuccess(message, container) {
        if (container) {
            container.innerHTML = `<div class="alert alert-success" role="alert">${message}</div>`;
            setTimeout(() => {
                container.innerHTML = '';
            }, 3000);
        } else {
            alert(message);
        }
    }

    static formatDate(dateString) {
        if (!dateString) return '';
        const date = new Date(dateString);
        return date.toLocaleDateString('en-US', { year: 'numeric', month: 'short', day: 'numeric' });
    }

    static formatGrade(grade) {
        if (grade === null || grade === undefined) return '-';
        return grade.toFixed(2);
    }

    static formatPercentage(percentage) {
        if (percentage === null || percentage === undefined) return '-';
        return percentage.toFixed(1) + '%';
    }

    static getDepartmentMap() {
        return {
            'DEPT001': 'New & Renewable Energy',
            'DEPT002': 'Mechatronics',
            'DEPT003': 'Information Technology',
            'DEPT004': 'Autotronics',
            'DEPT005': 'Petroleum Technology',
            'DEPT006': 'Prosthetics'
        };
    }

    static getLevelMap() {
        return {
            'LEVEL1': 'Level 1',
            'LEVEL2': 'Level 2',
            'LEVEL3': 'Level 3',
            'LEVEL4': 'Level 4'
        };
    }

    static getLookupList(type) {
        if (typeof API !== 'undefined' && API.getLookups) {
            const cache = API.getLookups();
            if (cache && cache[type] && cache[type].length) {
                return cache[type];
            }
        }
        if (type === 'departments') {
            return Object.entries(this.getDepartmentMap()).map(([id, nameEn]) => ({ id, nameEn }));
        }
        if (type === 'levels') {
            return Object.entries(this.getLevelMap()).map(([id, nameEn]) => ({ id, nameEn }));
        }
        return [];
    }

    static formatDepartment(departmentId) {
        if (!departmentId) return '-';
        const departments = this.getLookupList('departments');
        const match = departments.find(d => d.id === departmentId || d.departmentId === departmentId);
        if (match) {
            return match.nameEn || match.name || departmentId;
        }
        const fallback = this.getDepartmentMap();
        return fallback[departmentId] || departmentId || '-';
    }

    static formatLevel(levelId) {
        if (!levelId) return '-';
        const levels = this.getLookupList('levels');
        const match = levels.find(l => l.id === levelId || l.levelId === levelId);
        if (match) {
            return match.nameEn || match.name || levelId;
        }
        const fallback = this.getLevelMap();
        return fallback[levelId] || levelId || '-';
    }

    static getGradeColor(letterGrade) {
        switch (letterGrade) {
            case 'A+':
            case 'A':
            case 'A-':
                return 'success';
            case 'B+':
            case 'B':
            case 'B-':
                return 'primary';
            case 'C+':
            case 'C':
            case 'C-':
                return 'warning';
            case 'D+':
            case 'D':
                return 'warning';
            case 'F': return 'danger';
            default: return 'secondary';
        }
    }

    static getAttendanceBadgeClass(status) {
        switch (status) {
            case 'present': return 'bg-success';
            case 'absent': return 'bg-danger';
            case 'late': return 'bg-warning';
            case 'excused': return 'bg-info';
            default: return 'bg-secondary';
        }
    }

    static createRoleBasedMenu() {
        const role = auth.getRole();
        const menu = document.getElementById('sidebar-menu');
        if (!menu) return;

        menu.innerHTML = '';
        const currentPage = window.location.pathname.split('/').pop();

        const menuItems = [
            { icon: 'home', text: 'Dashboard', href: 'dashboard.html', roles: ['ROLE_STUDENT', 'ROLE_PROFESSOR', 'ROLE_STUDENT_AFFAIRS', 'ROLE_SUPERADMIN'] },
            { icon: 'people', text: 'Students', href: 'students.html', roles: ['ROLE_STUDENT_AFFAIRS', 'ROLE_SUPERADMIN'] },
            { icon: 'person-badge', text: 'Professors', href: 'professors.html', roles: ['ROLE_SUPERADMIN'] },
            { icon: 'book', text: 'Courses', href: 'courses.html', roles: ['ROLE_SUPERADMIN'] },
            { icon: 'clipboard-check', text: 'Grades', href: 'grades.html', roles: ['ROLE_STUDENT', 'ROLE_PROFESSOR', 'ROLE_SUPERADMIN'] },
            { icon: 'calendar-check', text: 'Attendance', href: 'attendance.html', roles: ['ROLE_STUDENT', 'ROLE_PROFESSOR', 'ROLE_SUPERADMIN'] },
            { icon: 'building', text: 'Facilities', href: 'facilities.html', roles: ['ROLE_SUPERADMIN'] },
            { icon: 'sliders', text: 'Administration', href: 'admin.html', roles: ['ROLE_SUPERADMIN'] }
        ];

        menuItems.forEach(item => {
            if (item.roles.includes(role)) {
                const li = document.createElement('li');
                li.className = 'nav-item';
                const a = document.createElement('a');
                a.className = 'nav-link';
                a.href = item.href;
                a.innerHTML = `<i class="bi bi-${item.icon} me-2"></i>${item.text}`;
                if (item.href === currentPage) {
                    a.classList.add('active');
                }
                li.appendChild(a);
                menu.appendChild(li);
            }
        });
    }

    static updateUserInfo() {
        const user = auth.getUser();
        if (user) {
            const userNameEl = document.getElementById('user-name');
            if (userNameEl) {
                userNameEl.textContent = user.fullName;
            }

            const userRoleEl = document.getElementById('user-role');
            if (userRoleEl) {
                const roleNames = {
                    'ROLE_STUDENT': 'Student',
                    'ROLE_PROFESSOR': 'Professor',
                    'ROLE_STUDENT_AFFAIRS': 'Student Affairs',
                    'ROLE_SUPERADMIN': 'Super Admin'
                };
                userRoleEl.textContent = roleNames[user.roleId] || user.roleId;
            }
        }
    }

    static async initializePage() {
        if (!auth.isAuthenticated() && typeof API !== 'undefined' && API.syncSession) {
            const user = await API.syncSession();
            if (user && typeof auth.saveSession === 'function') {
                auth.saveSession(user);
            }
        }

        if (!auth.requireAuth()) {
            return false;
        }

        this.createRoleBasedMenu();
        this.updateUserInfo();
        if (typeof API !== 'undefined' && API.loadLookups) {
            API.loadLookups();
        }
        return true;
    }
}

// Table helper functions
class TableHelper {
    static createTable(headers, data, rowCallback = null) {
        const table = document.createElement('table');
        table.className = 'table table-striped table-hover';
        
        const thead = document.createElement('thead');
        const headerRow = document.createElement('tr');
        headers.forEach(header => {
            const th = document.createElement('th');
            th.textContent = header;
            headerRow.appendChild(th);
        });
        thead.appendChild(headerRow);
        table.appendChild(thead);

        const tbody = document.createElement('tbody');
        data.forEach(item => {
            const row = document.createElement('tr');
            if (rowCallback) {
                rowCallback(row, item);
            }
            tbody.appendChild(row);
        });
        table.appendChild(tbody);

        return table;
    }

    static createCard(title, content, footer = null) {
        const card = document.createElement('div');
        card.className = 'card mb-4';
        
        const cardHeader = document.createElement('div');
        cardHeader.className = 'card-header';
        cardHeader.textContent = title;
        card.appendChild(cardHeader);

        const cardBody = document.createElement('div');
        cardBody.className = 'card-body';
        if (typeof content === 'string') {
            cardBody.innerHTML = content;
        } else {
            cardBody.appendChild(content);
        }
        card.appendChild(cardBody);

        if (footer) {
            const cardFooter = document.createElement('div');
            cardFooter.className = 'card-footer';
            cardFooter.innerHTML = footer;
            card.appendChild(cardFooter);
        }

        return card;
    }
}


