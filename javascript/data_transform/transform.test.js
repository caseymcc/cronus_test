/**
 * Tests for data transform utilities.
 *
 * Run: node transform.test.js
 */

const { groupBy, flatten, unique, mapValues, filterMap, sum } = require("./transform");

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
        not: {
            toThrow()
            {
                try
                {
                    actual();
                }
                catch(e)
                {
                    throw new Error(`Expected no throw, but got: ${e.message}`);
                }
            }
        }
    };
}

// ── Tests ──

describe("groupBy", () =>
{
    it("should group items by key", () =>
    {
        const items = [
            { name: "Alice", dept: "eng" },
            { name: "Bob", dept: "sales" },
            { name: "Charlie", dept: "eng" }
        ];

        const result = groupBy(items, i => i.dept);
        expect(result.eng.length).toBe(2);
        expect(result.sales.length).toBe(1);
    });

    it("should handle null/undefined keys gracefully", () =>
    {
        const items = [
            { name: "Alice", dept: "eng" },
            { name: "Bob", dept: null },
            { name: "Charlie" }  // dept is undefined
        ];

        // Should not crash. Items with null/undefined key go to "unknown" group
        expect(() => groupBy(items, i => i.dept)).not.toThrow();

        const result = groupBy(items, i => i.dept);
        expect(result.eng.length).toBe(1);
    });
});

describe("flatten", () =>
{
    it("should flatten one level", () =>
    {
        expect(flatten([[1, 2], [3, 4]])).toEqual([1, 2, 3, 4]);
    });

    it("should flatten deeply nested arrays", () =>
    {
        expect(flatten([[[1]], [[2, [3]]]])).toEqual([1, 2, 3]);
    });

    it("should handle mixed nesting", () =>
    {
        expect(flatten([1, [2, [3, [4]]]])).toEqual([1, 2, 3, 4]);
    });

    it("should handle empty arrays", () =>
    {
        expect(flatten([])).toEqual([]);
        expect(flatten([[], []])).toEqual([]);
    });
});

describe("unique", () =>
{
    it("should deduplicate primitives", () =>
    {
        expect(unique([1, 2, 2, 3, 3, 3])).toEqual([1, 2, 3]);
    });

    it("should deduplicate strings", () =>
    {
        expect(unique(["a", "b", "a"])).toEqual(["a", "b"]);
    });

    it("should handle empty array", () =>
    {
        expect(unique([])).toEqual([]);
    });
});

describe("mapValues", () =>
{
    it("should transform object values", () =>
    {
        const result = mapValues({ a: 1, b: 2, c: 3 }, v => v * 2);
        expect(result).toEqual({ a: 2, b: 4, c: 6 });
    });

    it("should pass key as second argument", () =>
    {
        const result = mapValues({ x: 1 }, (v, k) => `${k}=${v}`);
        expect(result).toEqual({ x: "x=1" });
    });
});

describe("filterMap", () =>
{
    it("should filter and transform in one pass", () =>
    {
        const result = filterMap(
            [1, 2, 3, 4, 5],
            n => n % 2 === 0,
            n => n * 10
        );

        expect(result).toEqual([20, 40]);
    });
});

describe("sum", () =>
{
    it("should sum numbers", () =>
    {
        expect(sum([1, 2, 3, 4])).toBe(10);
    });

    it("should sum with selector", () =>
    {
        const items = [{ score: 10 }, { score: 20 }, { score: 30 }];
        expect(sum(items, i => i.score)).toBe(60);
    });

    it("should return 0 for empty array", () =>
    {
        expect(sum([])).toBe(0);
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
