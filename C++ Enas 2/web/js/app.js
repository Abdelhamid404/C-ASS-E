/*
 * ============================================
 * APP.JS - C++ WebView Bridge
 * ============================================
 * Communicates with C++ backend via webview bindings
 */

// Check if we're running in WebView with C++ bindings
const isWebView = typeof cpp_login !== 'undefined';

// Current user session
let currentUser = null;
const lookupCache = {
    loaded: false,
    departments: [],
    colleges: [],
    levels: [],
    semesters: [],
    courseTypes: []
};

// ============================================
// API Functions - Call C++ Backend
// ============================================
const API = {
    // Login with username and password
    login: async (username, password) => {
        try {
            let response;
            if (typeof cpp_login !== 'undefined') {
                // Use webview binding
                const result = await cpp_login(username, password);
                response = typeof result === 'string' ? JSON.parse(result) : result;
            } else {
                // Fallback - simulate login for testing
                console.log('Simulating login for:', username);
                response = {
                    success: true,
                    data: {
                        userId: 1,
                        username: username,
                        fullName: 'Test User',
                        roleId: 'ROLE_SUPERADMIN',
                        studentId: '',
                        professorId: ''
                    }
                };
            }
            
            if (response.success) {
                currentUser = response.data;
                localStorage.setItem('currentUser', JSON.stringify(currentUser));
                localStorage.setItem('nctu_sis_user', JSON.stringify(currentUser));
                return currentUser;
            } else {
                throw new Error(response.error || 'Login failed');
            }
        } catch (error) {
            console.error('Login error:', error);
            throw error;
        }
    },

    // Logout
    logout: async () => {
        try {
            if (typeof cpp_logout !== 'undefined') {
                await cpp_logout();
            }
            currentUser = null;
            localStorage.removeItem('currentUser');
            localStorage.removeItem('nctu_sis_user');
        } catch (error) {
            console.error('Logout error:', error);
        }
    },

    // Get current user
    getCurrentUser: () => {
        if (currentUser) return currentUser;
        const stored = localStorage.getItem('currentUser') || localStorage.getItem('nctu_sis_user');
        if (stored) {
            currentUser = JSON.parse(stored);
            return currentUser;
        }
        return null;
    },

    // Check if logged in
    isLoggedIn: () => {
        return API.getCurrentUser() !== null;
    },

    // Sync session with backend
    syncSession: async () => {
        if (typeof cpp_sendMessage === 'undefined') {
            return API.getCurrentUser();
        }
        try {
            const response = await API.sendMessage('getCurrentUser', {});
            if (response.success && response.data) {
                currentUser = response.data;
                localStorage.setItem('currentUser', JSON.stringify(currentUser));
                localStorage.setItem('nctu_sis_user', JSON.stringify(currentUser));
                return currentUser;
            }
        } catch (error) {
            console.warn('Session sync failed:', error);
        }
        currentUser = null;
        localStorage.removeItem('currentUser');
        localStorage.removeItem('nctu_sis_user');
        return null;
    },

    // Get student dashboard
    getStudentDashboard: async (studentId = '') => {
        try {
            const response = await API.sendMessage('getStudentDashboard', { studentId });
            if (response.success) return response.data;
            throw new Error(response.error || 'Failed to get student dashboard');
        } catch (error) {
            console.error('Dashboard error:', error);
            throw error;
        }
    },

    // Get student grades
    getStudentGrades: async (studentId = '', semesterId = '') => {
        try {
            const response = await API.sendMessage('getStudentGrades', { studentId, semesterId });
            if (response.success) {
                // MessageHandler returns {grades: [...]}
                return response.data && response.data.grades ? response.data.grades : [];
            }
            throw new Error(response.error || 'Failed to get student grades');
        } catch (error) {
            console.error('Grades error:', error);
            throw error;
        }
    },

    // Get student attendance
    getAttendance: async (studentId = '', courseId = '', semesterId = '') => {
        try {
            const response = await API.sendMessage('getStudentAttendance', { studentId, courseId, semesterId });
            if (response.success) {
                return response.data || { attendance: [] };
            }
            throw new Error(response.error || 'Failed to get attendance');
        } catch (error) {
            console.error('Attendance error:', error);
            throw error;
        }
    },

    // Get student courses
    getStudentCourses: async (studentId = '', semesterId = '') => {
        try {
            const response = await API.sendMessage('getStudentCourses', { studentId, semesterId });
            if (response.success) {
                return response.data && response.data.courses ? response.data.courses : [];
            }
            throw new Error(response.error || 'Failed to get student courses');
        } catch (error) {
            console.error('Student courses error:', error);
            throw error;
        }
    },

    // Get all students
    getStudents: async () => {
        try {
            const response = await API.sendMessage('getStudents', {});
            if (response.success) {
                // MessageHandler returns {students: [...]}
                return response.data && response.data.students ? response.data.students : [];
            }
            throw new Error(response.error || 'Failed to get students');
        } catch (error) {
            console.error('Students error:', error);
            throw error;
        }
    },

    // Create student
    createStudent: async (payload) => {
        try {
            const response = await API.sendMessage('createStudent', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to create student');
        } catch (error) {
            console.error('Create student error:', error);
            throw error;
        }
    },

    // Update student
    updateStudent: async (payload) => {
        try {
            const response = await API.sendMessage('updateStudent', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to update student');
        } catch (error) {
            console.error('Update student error:', error);
            throw error;
        }
    },

    // Delete student
    deleteStudent: async (studentId) => {
        try {
            const response = await API.sendMessage('deleteStudent', { studentId });
            if (response.success) return true;
            throw new Error(response.error || 'Failed to delete student');
        } catch (error) {
            console.error('Delete student error:', error);
            throw error;
        }
    },

    // Get all courses
    getCourses: async (departmentId = '') => {
        try {
            const response = await API.sendMessage('getCourses', { departmentId });
            if (response.success) {
                // MessageHandler returns {courses: [...]}
                return response.data && response.data.courses ? response.data.courses : [];
            }
            throw new Error(response.error || 'Failed to get courses');
        } catch (error) {
            console.error('Courses error:', error);
            throw error;
        }
    },

    // Create course
    createCourse: async (payload) => {
        try {
            const response = await API.sendMessage('createCourse', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to create course');
        } catch (error) {
            console.error('Create course error:', error);
            throw error;
        }
    },

    // Update course
    updateCourse: async (payload) => {
        try {
            const response = await API.sendMessage('updateCourse', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to update course');
        } catch (error) {
            console.error('Update course error:', error);
            throw error;
        }
    },

    // Delete course
    deleteCourse: async (courseId) => {
        try {
            const response = await API.sendMessage('deleteCourse', { courseId });
            if (response.success) return true;
            throw new Error(response.error || 'Failed to delete course');
        } catch (error) {
            console.error('Delete course error:', error);
            throw error;
        }
    },

    // Get professor courses
    getProfessorCourses: async (professorId = '') => {
        try {
            const response = await API.sendMessage('getProfessorCourses', { professorId });
            if (response.success) {
                // MessageHandler returns {courses: [...]}
                return response.data || { courses: [] };
            }
            throw new Error(response.error || 'Failed to get professor courses');
        } catch (error) {
            console.error('Professor courses error:', error);
            throw error;
        }
    },

    // Get course assignments
    getCourseAssignments: async () => {
        try {
            const response = await API.sendMessage('getCourseAssignments', {});
            if (response.success) {
                return response.data && response.data.assignments ? response.data.assignments : [];
            }
            throw new Error(response.error || 'Failed to get course assignments');
        } catch (error) {
            console.error('Course assignments error:', error);
            throw error;
        }
    },

    // Create course assignment
    createCourseAssignment: async (payload) => {
        try {
            const response = await API.sendMessage('createCourseAssignment', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to assign course');
        } catch (error) {
            console.error('Create assignment error:', error);
            throw error;
        }
    },

    // Delete course assignment
    deleteCourseAssignment: async (payload) => {
        try {
            const response = await API.sendMessage('deleteCourseAssignment', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to delete assignment');
        } catch (error) {
            console.error('Delete assignment error:', error);
            throw error;
        }
    },

    // Get course students (with grades)
    getCourseStudents: async (courseId = '', semesterId = '') => {
        try {
            const response = await API.sendMessage('getCourseStudents', { courseId, semesterId });
            if (response.success) {
                return response.data || { students: [] };
            }
            throw new Error(response.error || 'Failed to get course students');
        } catch (error) {
            console.error('Course students error:', error);
            throw error;
        }
    },

    // Get course grades
    getCourseGrades: async (courseId = '', semesterId = '') => {
        try {
            const response = await API.sendMessage('getCourseGrades', { courseId, semesterId });
            if (response.success) {
                return response.data && response.data.grades ? response.data.grades : [];
            }
            throw new Error(response.error || 'Failed to get course grades');
        } catch (error) {
            console.error('Course grades error:', error);
            throw error;
        }
    },

    // Delete grade
    deleteGrade: async (payload) => {
        try {
            const response = await API.sendMessage('deleteGrade', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to delete grade');
        } catch (error) {
            console.error('Delete grade error:', error);
            throw error;
        }
    },

    // Get course attendance
    getCourseAttendance: async (courseId = '', semesterId = '', date = '') => {
        try {
            const response = await API.sendMessage('getCourseAttendance', { courseId, semesterId, date });
            if (response.success) {
                return response.data || { attendance: [] };
            }
            throw new Error(response.error || 'Failed to get course attendance');
        } catch (error) {
            console.error('Course attendance error:', error);
            throw error;
        }
    },

    // Update grade for a student/course
    updateGrade: async (payload, assignment1, assignment2, yearWork, finalExam, midterm = 0) => {
        try {
            let data = {};
            if (typeof payload === 'object') {
                data = payload;
            } else {
                data = {
                    registrationId: payload,
                    assignment1,
                    assignment2,
                    yearWork,
                    finalExam,
                    midterm
                };
            }
            const response = await API.sendMessage('updateGrade', data);
            if (response.success) return true;
            throw new Error(response.error || 'Failed to update grade');
        } catch (error) {
            console.error('Update grade error:', error);
            throw error;
        }
    },

    // Record attendance (batch supported)
    recordAttendance: async (courseId, semesterId, sessionDate, sessionType, students = []) => {
        try {
            const response = await API.sendMessage('recordAttendance', {
                courseId,
                semesterId,
                date: sessionDate,
                sessionType,
                students
            });
            if (response.success) return response.data || {};
            throw new Error(response.error || 'Failed to record attendance');
        } catch (error) {
            console.error('Record attendance error:', error);
            throw error;
        }
    },

    // Update attendance record
    updateAttendanceRecord: async (payload) => {
        try {
            const response = await API.sendMessage('updateAttendanceRecord', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to update attendance');
        } catch (error) {
            console.error('Update attendance error:', error);
            throw error;
        }
    },

    // Delete attendance record
    deleteAttendance: async (attendanceId) => {
        try {
            const response = await API.sendMessage('deleteAttendance', { attendanceId });
            if (response.success) return true;
            throw new Error(response.error || 'Failed to delete attendance');
        } catch (error) {
            console.error('Delete attendance error:', error);
            throw error;
        }
    },

    // Get professors
    getProfessors: async (departmentId = '') => {
        try {
            const response = await API.sendMessage('getProfessors', { departmentId });
            if (response.success) {
                return response.data && response.data.professors ? response.data.professors : [];
            }
            throw new Error(response.error || 'Failed to get professors');
        } catch (error) {
            console.error('Professors error:', error);
            throw error;
        }
    },

    // Create professor
    createProfessor: async (payload) => {
        try {
            const response = await API.sendMessage('createProfessor', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to create professor');
        } catch (error) {
            console.error('Create professor error:', error);
            throw error;
        }
    },

    // Update professor
    updateProfessor: async (payload) => {
        try {
            const response = await API.sendMessage('updateProfessor', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to update professor');
        } catch (error) {
            console.error('Update professor error:', error);
            throw error;
        }
    },

    // Delete professor
    deleteProfessor: async (professorId) => {
        try {
            const response = await API.sendMessage('deleteProfessor', { professorId });
            if (response.success) return true;
            throw new Error(response.error || 'Failed to delete professor');
        } catch (error) {
            console.error('Delete professor error:', error);
            throw error;
        }
    },

    // Get facilities (lecture halls and labs)
    getFacilities: async () => {
        try {
            const response = await API.sendMessage('getFacilities', {});
            if (response.success) {
                return response.data || { lectureHalls: [], laboratories: [] };
            }
            throw new Error(response.error || 'Failed to get facilities');
        } catch (error) {
            console.error('Facilities error:', error);
            throw error;
        }
    },

    // Create lecture hall
    createLectureHall: async (payload) => {
        try {
            const response = await API.sendMessage('createLectureHall', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to create lecture hall');
        } catch (error) {
            console.error('Create lecture hall error:', error);
            throw error;
        }
    },

    // Update lecture hall
    updateLectureHall: async (payload) => {
        try {
            const response = await API.sendMessage('updateLectureHall', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to update lecture hall');
        } catch (error) {
            console.error('Update lecture hall error:', error);
            throw error;
        }
    },

    // Delete lecture hall
    deleteLectureHall: async (hallId) => {
        try {
            const response = await API.sendMessage('deleteLectureHall', { id: hallId });
            if (response.success) return true;
            throw new Error(response.error || 'Failed to delete lecture hall');
        } catch (error) {
            console.error('Delete lecture hall error:', error);
            throw error;
        }
    },

    // Create laboratory
    createLaboratory: async (payload) => {
        try {
            const response = await API.sendMessage('createLaboratory', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to create laboratory');
        } catch (error) {
            console.error('Create laboratory error:', error);
            throw error;
        }
    },

    // Update laboratory
    updateLaboratory: async (payload) => {
        try {
            const response = await API.sendMessage('updateLaboratory', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to update laboratory');
        } catch (error) {
            console.error('Update laboratory error:', error);
            throw error;
        }
    },

    // Delete laboratory
    deleteLaboratory: async (labId) => {
        try {
            const response = await API.sendMessage('deleteLaboratory', { id: labId });
            if (response.success) return true;
            throw new Error(response.error || 'Failed to delete laboratory');
        } catch (error) {
            console.error('Delete laboratory error:', error);
            throw error;
        }
    },

    // Get registrations
    getRegistrations: async (payload = {}) => {
        try {
            const response = await API.sendMessage('getRegistrations', payload);
            if (response.success) {
                return response.data && response.data.registrations ? response.data.registrations : [];
            }
            throw new Error(response.error || 'Failed to get registrations');
        } catch (error) {
            console.error('Registrations error:', error);
            throw error;
        }
    },

    // Register student to course
    registerStudentToCourse: async (payload) => {
        try {
            const response = await API.sendMessage('registerStudentToCourse', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to register student');
        } catch (error) {
            console.error('Register student error:', error);
            throw error;
        }
    },

    // Update registration
    updateRegistration: async (payload) => {
        try {
            const response = await API.sendMessage('updateRegistration', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to update registration');
        } catch (error) {
            console.error('Update registration error:', error);
            throw error;
        }
    },

    // Delete registration
    deleteRegistration: async (registrationId) => {
        try {
            const response = await API.sendMessage('deleteRegistration', { registrationId });
            if (response.success) return true;
            throw new Error(response.error || 'Failed to delete registration');
        } catch (error) {
            console.error('Delete registration error:', error);
            throw error;
        }
    },

    // Departments
    getDepartments: async () => {
        try {
            const response = await API.sendMessage('getDepartments', {});
            if (response.success) {
                return response.data && response.data.departments ? response.data.departments : [];
            }
            throw new Error(response.error || 'Failed to get departments');
        } catch (error) {
            console.error('Departments error:', error);
            throw error;
        }
    },
    createDepartment: async (payload) => {
        try {
            const response = await API.sendMessage('createDepartment', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to create department');
        } catch (error) {
            console.error('Create department error:', error);
            throw error;
        }
    },
    updateDepartment: async (payload) => {
        try {
            const response = await API.sendMessage('updateDepartment', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to update department');
        } catch (error) {
            console.error('Update department error:', error);
            throw error;
        }
    },
    deleteDepartment: async (departmentId) => {
        try {
            const response = await API.sendMessage('deleteDepartment', { departmentId });
            if (response.success) return true;
            throw new Error(response.error || 'Failed to delete department');
        } catch (error) {
            console.error('Delete department error:', error);
            throw error;
        }
    },

    // Colleges
    getColleges: async () => {
        try {
            const response = await API.sendMessage('getColleges', {});
            if (response.success) {
                return response.data && response.data.colleges ? response.data.colleges : [];
            }
            throw new Error(response.error || 'Failed to get colleges');
        } catch (error) {
            console.error('Colleges error:', error);
            throw error;
        }
    },
    createCollege: async (payload) => {
        try {
            const response = await API.sendMessage('createCollege', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to create college');
        } catch (error) {
            console.error('Create college error:', error);
            throw error;
        }
    },
    updateCollege: async (payload) => {
        try {
            const response = await API.sendMessage('updateCollege', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to update college');
        } catch (error) {
            console.error('Update college error:', error);
            throw error;
        }
    },
    deleteCollege: async (collegeId) => {
        try {
            const response = await API.sendMessage('deleteCollege', { collegeId });
            if (response.success) return true;
            throw new Error(response.error || 'Failed to delete college');
        } catch (error) {
            console.error('Delete college error:', error);
            throw error;
        }
    },

    // Academic levels
    getAcademicLevels: async () => {
        try {
            const response = await API.sendMessage('getAcademicLevels', {});
            if (response.success) {
                return response.data && response.data.levels ? response.data.levels : [];
            }
            throw new Error(response.error || 'Failed to get academic levels');
        } catch (error) {
            console.error('Academic levels error:', error);
            throw error;
        }
    },
    createAcademicLevel: async (payload) => {
        try {
            const response = await API.sendMessage('createAcademicLevel', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to create academic level');
        } catch (error) {
            console.error('Create academic level error:', error);
            throw error;
        }
    },
    updateAcademicLevel: async (payload) => {
        try {
            const response = await API.sendMessage('updateAcademicLevel', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to update academic level');
        } catch (error) {
            console.error('Update academic level error:', error);
            throw error;
        }
    },
    deleteAcademicLevel: async (levelId) => {
        try {
            const response = await API.sendMessage('deleteAcademicLevel', { levelId });
            if (response.success) return true;
            throw new Error(response.error || 'Failed to delete academic level');
        } catch (error) {
            console.error('Delete academic level error:', error);
            throw error;
        }
    },

    // Semesters
    getSemesters: async () => {
        try {
            const response = await API.sendMessage('getSemesters', {});
            if (response.success) {
                return response.data && response.data.semesters ? response.data.semesters : [];
            }
            throw new Error(response.error || 'Failed to get semesters');
        } catch (error) {
            console.error('Semesters error:', error);
            throw error;
        }
    },
    createSemester: async (payload) => {
        try {
            const response = await API.sendMessage('createSemester', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to create semester');
        } catch (error) {
            console.error('Create semester error:', error);
            throw error;
        }
    },
    updateSemester: async (payload) => {
        try {
            const response = await API.sendMessage('updateSemester', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to update semester');
        } catch (error) {
            console.error('Update semester error:', error);
            throw error;
        }
    },
    deleteSemester: async (semesterId) => {
        try {
            const response = await API.sendMessage('deleteSemester', { semesterId });
            if (response.success) return true;
            throw new Error(response.error || 'Failed to delete semester');
        } catch (error) {
            console.error('Delete semester error:', error);
            throw error;
        }
    },

    // Course types
    getCourseTypes: async () => {
        try {
            const response = await API.sendMessage('getCourseTypes', {});
            if (response.success) {
                return response.data && response.data.courseTypes ? response.data.courseTypes : [];
            }
            throw new Error(response.error || 'Failed to get course types');
        } catch (error) {
            console.error('Course types error:', error);
            throw error;
        }
    },
    createCourseType: async (payload) => {
        try {
            const response = await API.sendMessage('createCourseType', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to create course type');
        } catch (error) {
            console.error('Create course type error:', error);
            throw error;
        }
    },
    updateCourseType: async (payload) => {
        try {
            const response = await API.sendMessage('updateCourseType', payload || {});
            if (response.success) return true;
            throw new Error(response.error || 'Failed to update course type');
        } catch (error) {
            console.error('Update course type error:', error);
            throw error;
        }
    },
    deleteCourseType: async (courseTypeId) => {
        try {
            const response = await API.sendMessage('deleteCourseType', { courseTypeId });
            if (response.success) return true;
            throw new Error(response.error || 'Failed to delete course type');
        } catch (error) {
            console.error('Delete course type error:', error);
            throw error;
        }
    },

    // Load lookup tables with caching
    loadLookups: async (force = false) => {
        if (lookupCache.loaded && !force) {
            return lookupCache;
        }
        const safeLoad = async (fn) => {
            try {
                return await fn();
            } catch (error) {
                return [];
            }
        };
        const [departments, colleges, levels, semesters, courseTypes] = await Promise.all([
            safeLoad(() => API.getDepartments()),
            safeLoad(() => API.getColleges()),
            safeLoad(() => API.getAcademicLevels()),
            safeLoad(() => API.getSemesters()),
            safeLoad(() => API.getCourseTypes())
        ]);
        lookupCache.departments = departments;
        lookupCache.colleges = colleges;
        lookupCache.levels = levels;
        lookupCache.semesters = semesters;
        lookupCache.courseTypes = courseTypes;
        lookupCache.loaded = true;
        return lookupCache;
    },

    // Get lookup cache
    getLookups: () => lookupCache,

    // Send generic message to C++ backend
    sendMessage: async (action, data = {}) => {
        try {
            if (typeof cpp_sendMessage !== 'undefined') {
                const message = JSON.stringify({ action, ...data });
                const result = await cpp_sendMessage(message);
                const response = typeof result === 'string' ? JSON.parse(result) : result;
                if (response && response.success === false && response.error) {
                    if (response.error.toLowerCase().includes('not logged in')) {
                        await API.logout();
                        const currentPage = window.location.pathname.split('/').pop();
                        if (currentPage !== 'index.html') {
                            UI.navigateTo('index.html');
                        }
                    }
                }
                return response;
            }
            console.warn('cpp_sendMessage not available, action:', action);
            return { success: false, error: 'Backend not available' };
        } catch (error) {
            console.error('Send message error:', error);
            throw error;
        }
    }
};

// ============================================
// UI Helper Functions
// ============================================
const UI = {
    // Show loading spinner
    showLoading: (containerId) => {
        const container = document.getElementById(containerId);
        if (container) {
            container.innerHTML = `
                <div class="text-center py-5">
                    <div class="spinner-border text-primary" role="status">
                        <span class="visually-hidden">Loading...</span>
                    </div>
                    <p class="mt-2 text-muted">Loading...</p>
                </div>
            `;
        }
    },

    // Show error message
    showError: (containerId, message) => {
        const container = document.getElementById(containerId);
        if (container) {
            container.innerHTML = `
                <div class="alert alert-danger" role="alert">
                    <i class="bi bi-exclamation-triangle me-2"></i>
                    ${message}
                </div>
            `;
        }
    },

    // Show success message
    showSuccess: (message) => {
        // Create toast notification
        const toast = document.createElement('div');
        toast.className = 'toast-notification success';
        toast.innerHTML = `<i class="bi bi-check-circle me-2"></i>${message}`;
        document.body.appendChild(toast);
        
        setTimeout(() => toast.classList.add('show'), 100);
        setTimeout(() => {
            toast.classList.remove('show');
            setTimeout(() => toast.remove(), 300);
        }, 3000);
    },

    // Navigate to page
    navigateTo: (page) => {
        window.location.href = page;
    },

    // Format date
    formatDate: (dateStr) => {
        if (!dateStr) return '-';
        const date = new Date(dateStr);
        return date.toLocaleDateString('en-US', { 
            year: 'numeric', 
            month: 'short', 
            day: 'numeric' 
        });
    },

    // Get grade color class
    getGradeColor: (percentage) => {
        if (percentage >= 85) return 'success';
        if (percentage >= 70) return 'primary';
        if (percentage >= 50) return 'warning';
        return 'danger';
    }
};

// ============================================
// Page Initialization
// ============================================
document.addEventListener('DOMContentLoaded', () => {
    console.log('App loaded. WebView mode:', isWebView);

    (async () => {
        // Check authentication on protected pages
        const publicPages = ['index.html', 'login.html', ''];
        const currentPage = window.location.pathname.split('/').pop();

        let user = API.getCurrentUser();
        if (!publicPages.includes(currentPage)) {
            user = await API.syncSession();
            if (!user) {
                console.log('Not logged in, redirecting to login...');
                UI.navigateTo('index.html');
                return;
            }
        }

        // Update UI based on user role
        if (user) {
            updateNavigation(user);
        }
    })();
});

// Update navigation based on user role
function updateNavigation(user) {
    const navUser = document.getElementById('nav-user-name');
    const navRole = document.getElementById('nav-user-role');
    
    if (navUser) navUser.textContent = user.fullName;
    if (navRole) navRole.textContent = getRoleName(user.roleId);
}

// Get human-readable role name
function getRoleName(roleId) {
    const roles = {
        'ROLE_SUPERADMIN': 'Super Admin',
        'ROLE_STUDENT_AFFAIRS': 'Student Affairs',
        'ROLE_PROFESSOR': 'Professor',
        'ROLE_STUDENT': 'Student'
    };
    return roles[roleId] || roleId;
}

// ============================================
// Login Page Handler
// ============================================
async function handleLogin(event) {
    event.preventDefault();
    
    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;
    const errorDiv = document.getElementById('login-error');
    const submitBtn = document.getElementById('login-btn');
    
    // Disable button during login
    submitBtn.disabled = true;
    submitBtn.innerHTML = '<span class="spinner-border spinner-border-sm me-2"></span>Logging in...';
    errorDiv.classList.add('d-none');
    
    try {
        const user = await API.login(username, password);
        console.log('Login successful:', user);
        
        // Redirect based on role
        switch (user.roleId) {
            case 'ROLE_STUDENT':
                UI.navigateTo('dashboard.html');
                break;
            case 'ROLE_PROFESSOR':
                UI.navigateTo('grades.html');
                break;
            case 'ROLE_STUDENT_AFFAIRS':
            case 'ROLE_SUPERADMIN':
                UI.navigateTo('students.html');
                break;
            default:
                UI.navigateTo('dashboard.html');
        }
    } catch (error) {
        console.error('Login failed:', error);
        errorDiv.textContent = error.message || 'Invalid username or password';
        errorDiv.classList.remove('d-none');
        
        submitBtn.disabled = false;
        submitBtn.innerHTML = 'Login';
    }
}

// Logout handler
async function handleLogout() {
    await API.logout();
    UI.navigateTo('index.html');
}

// Export for global access
window.API = API;
window.UI = UI;
window.handleLogin = handleLogin;
window.handleLogout = handleLogout;
