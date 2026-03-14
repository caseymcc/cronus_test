/**
 * Tests for TaskManager.
 *
 * Run: node taskManager.test.js
 *
 * Uses a minimal test runner (no Jest dependency required).
 */

const { TaskManager } = require("./taskManager");

let testsRun = 0;
let testsPassed = 0;
let testsFailed = 0;

function describe(name, fn)
{
    console.log(`\n  ${name}`);
    fn();
}

function it(name, fn)
{
    testsRun++;

    try
    {
        fn();
        testsPassed++;
        console.log(`    ✓ ${name}`);
    }
    catch(e)
    {
        testsFailed++;
        console.log(`    ✗ ${name}: ${e.message}`);
    }
}

function expect(actual)
{
    return {
        toBe(expected)
        {
            if(actual !== expected)
            {
                throw new Error(`Expected ${JSON.stringify(expected)}, got ${JSON.stringify(actual)}`);
            }
        },
        toEqual(expected)
        {
            if(JSON.stringify(actual) !== JSON.stringify(expected))
            {
                throw new Error(`Expected ${JSON.stringify(expected)}, got ${JSON.stringify(actual)}`);
            }
        },
        toBeNull()
        {
            if(actual !== null)
            {
                throw new Error(`Expected null, got ${JSON.stringify(actual)}`);
            }
        },
        toBeTrue()
        {
            if(actual !== true)
            {
                throw new Error(`Expected true, got ${JSON.stringify(actual)}`);
            }
        },
        toBeFalse()
        {
            if(actual !== false)
            {
                throw new Error(`Expected false, got ${JSON.stringify(actual)}`);
            }
        },
        toThrow()
        {
            // actual should be a function
            let threw = false;

            try
            {
                actual();
            }
            catch(e)
            {
                threw = true;
            }

            if(!threw)
            {
                throw new Error("Expected function to throw");
            }
        },
        not: {
            toThrow()
            {
                try
                {
                    actual();
                }
                catch(e)
                {
                    throw new Error(`Expected function not to throw, but got: ${e.message}`);
                }
            }
        }
    };
}

// ── Tests ──

describe("TaskManager - addTask", () =>
{
    it("should create a task with correct fields", () =>
    {
        const tm = new TaskManager();
        const task = tm.addTask("Write tests", "Cover edge cases", "high");
        expect(task.title).toBe("Write tests");
        expect(task.description).toBe("Cover edge cases");
        expect(task.priority).toBe("high");
        expect(task.status).toBe("pending");
        expect(task.completedAt).toBeNull();
    });

    it("should assign incrementing IDs", () =>
    {
        const tm = new TaskManager();
        const t1 = tm.addTask("First");
        const t2 = tm.addTask("Second");
        expect(t2.id).toBe(t1.id + 1);
    });
});

describe("TaskManager - getTask", () =>
{
    it("should return task by ID", () =>
    {
        const tm = new TaskManager();
        const task = tm.addTask("Test");
        expect(tm.getTask(task.id).title).toBe("Test");
    });

    it("should return null for nonexistent ID", () =>
    {
        const tm = new TaskManager();
        expect(tm.getTask(999)).toBeNull();
    });
});

describe("TaskManager - completeTask", () =>
{
    it("should mark task as done", () =>
    {
        const tm = new TaskManager();
        const task = tm.addTask("Do something");
        tm.completeTask(task.id);
        expect(tm.getTask(task.id).status).toBe("done");
    });

    it("should not throw when completing nonexistent task", () =>
    {
        const tm = new TaskManager();
        // Should return null or handle gracefully, not throw TypeError
        expect(() => tm.completeTask(999)).not.toThrow();
    });
});

describe("TaskManager - removeTask", () =>
{
    it("should remove a task", () =>
    {
        const tm = new TaskManager();
        const task = tm.addTask("Remove me");
        expect(tm.removeTask(task.id)).toBeTrue();
        expect(tm.getTask(task.id)).toBeNull();
    });

    it("should return false for nonexistent task", () =>
    {
        const tm = new TaskManager();
        expect(tm.removeTask(999)).toBeFalse();
    });

    it("should clean up status index after removal", () =>
    {
        const tm = new TaskManager();
        const task = tm.addTask("Test");
        tm.removeTask(task.id);
        // After removal, getTasksByStatus should not include the removed task
        const pending = tm.getTasksByStatus("pending");
        expect(pending.length).toBe(0);
    });
});

describe("TaskManager - getTasksByStatus", () =>
{
    it("should return tasks filtered by status", () =>
    {
        const tm = new TaskManager();
        tm.addTask("Pending 1");
        tm.addTask("Pending 2");
        const t3 = tm.addTask("Will complete");
        tm.completeTask(t3.id);

        expect(tm.getTasksByStatus("pending").length).toBe(2);
        expect(tm.getTasksByStatus("done").length).toBe(1);
    });

    it("should return a copy, not the internal array", () =>
    {
        const tm = new TaskManager();
        tm.addTask("Test");
        const result = tm.getTasksByStatus("pending");
        // Mutating the result should not affect internal state
        result.length = 0;
        expect(tm.getTasksByStatus("pending").length).toBe(1);
    });
});

describe("TaskManager - getStats", () =>
{
    it("should return correct counts", () =>
    {
        const tm = new TaskManager();
        tm.addTask("A");
        tm.addTask("B");
        const c = tm.addTask("C");
        tm.completeTask(c.id);

        const stats = tm.getStats();
        expect(stats.total).toBe(3);
        expect(stats.pending).toBe(2);
        expect(stats.done).toBe(1);
    });
});

// ── Results ──

console.log(`\n  ${testsPassed}/${testsRun} tests passed.`);

if(testsFailed > 0)
{
    console.log(`  ${testsFailed} tests FAILED.\n`);
    process.exit(1);
}
else
{
    console.log("");
    process.exit(0);
}
