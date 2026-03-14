/**
 * Simple async task manager.
 *
 * BUGS:
 * - completeTask doesn't check if task exists before modifying
 * - getTasksByStatus returns reference to internal array (mutation risk)
 * - removeTask doesn't clean up from status index
 */

class TaskManager
{
    constructor()
    {
        this.m_tasks = new Map();
        this.m_nextId = 1;
        this.m_statusIndex = {};  // status -> [taskId, ...]
    }

    addTask(title, description = "", priority = "medium")
    {
        const id = this.m_nextId++;
        const task = {
            id,
            title,
            description,
            priority,
            status: "pending",
            createdAt: new Date().toISOString(),
            completedAt: null
        };

        this.m_tasks.set(id, task);
        this._addToIndex("pending", id);

        return task;
    }

    getTask(id)
    {
        return this.m_tasks.get(id) || null;
    }

    completeTask(id)
    {
        // BUG: doesn't check if task exists — will throw on undefined.status
        const task = this.m_tasks.get(id);
        const oldStatus = task.status;
        task.status = "done";
        task.completedAt = new Date().toISOString();

        this._removeFromIndex(oldStatus, id);
        this._addToIndex("done", id);

        return task;
    }

    removeTask(id)
    {
        const task = this.m_tasks.get(id);

        if(!task)
        {
            return false;
        }

        // BUG: deletes from map but doesn't remove from status index
        this.m_tasks.delete(id);
        return true;
    }

    getTasksByStatus(status)
    {
        const ids = this.m_statusIndex[status] || [];

        // BUG: returns the internal index array directly — caller can mutate it
        return ids.map(id => this.m_tasks.get(id)).filter(Boolean);
    }

    getAllTasks()
    {
        return Array.from(this.m_tasks.values());
    }

    getStats()
    {
        const tasks = this.getAllTasks();
        return {
            total: tasks.length,
            pending: tasks.filter(t => t.status === "pending").length,
            done: tasks.filter(t => t.status === "done").length
        };
    }

    _addToIndex(status, id)
    {
        if(!this.m_statusIndex[status])
        {
            this.m_statusIndex[status] = [];
        }

        this.m_statusIndex[status].push(id);
    }

    _removeFromIndex(status, id)
    {
        if(!this.m_statusIndex[status])
        {
            return;
        }

        this.m_statusIndex[status] = this.m_statusIndex[status].filter(i => i !== id);
    }
}

module.exports = { TaskManager };
