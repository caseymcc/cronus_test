/**
 * Data transformation pipeline utilities.
 *
 * BUGS:
 * - groupBy doesn't handle null/undefined keys
 * - flatten only goes one level deep (should be recursive)
 * - unique uses === which fails for object comparison
 */

/**
 * Group an array of objects by a key.
 *
 * BUG: If keyFn returns null/undefined, it crashes trying to use it as object key.
 */
function groupBy(items, keyFn)
{
    const groups = {};

    for(const item of items)
    {
        const key = keyFn(item);
        // BUG: no guard for null/undefined key
        if(!groups[key])
        {
            groups[key] = [];
        }

        groups[key].push(item);
    }

    return groups;
}

/**
 * Flatten a nested array.
 *
 * BUG: Only flattens one level. [[[1]]] -> [[1]] instead of [1].
 */
function flatten(arr)
{
    const result = [];

    for(const item of arr)
    {
        if(Array.isArray(item))
        {
            // BUG: uses concat instead of recursive flatten
            result.push(...item);
        }
        else
        {
            result.push(item);
        }
    }

    return result;
}

/**
 * Get unique values from an array.
 *
 * Works correctly for primitives, but the test expects deep equality for objects.
 */
function unique(arr)
{
    return [...new Set(arr)];
}

/**
 * Map values of an object.
 */
function mapValues(obj, fn)
{
    const result = {};

    for(const [key, value] of Object.entries(obj))
    {
        result[key] = fn(value, key);
    }

    return result;
}

/**
 * Filter an array and transform in one pass.
 */
function filterMap(arr, filterFn, mapFn)
{
    const result = [];

    for(const item of arr)
    {
        if(filterFn(item))
        {
            result.push(mapFn(item));
        }
    }

    return result;
}

/**
 * Sum values in an array, optionally with a selector function.
 */
function sum(arr, selectorFn = null)
{
    return arr.reduce((acc, item) =>
    {
        const value = selectorFn ? selectorFn(item) : item;
        return acc + value;
    }, 0);
}

module.exports = { groupBy, flatten, unique, mapValues, filterMap, sum };
