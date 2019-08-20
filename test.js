
module.exports.range = limit => {
    let a = 0;
    return {
        [Symbol.iterator]: () => ({
            next: () => {
                if (a >= limit) {
                    return {
                        value: a,
                        done: true
                    }
                } else {
                a += 1;
                    return {
                        done: false,
                        value: a
                    };
                }
            }
        })
    };
};

module.exports.range2 = function*(limit) {
    for (let i = 0; i < limit; i++) {
        yield i;
    }
};

const setTimeoutP = module.exports.setTimeoutP = (func, duration) => new Promise(resolve => {
    setTimeout(() => resolve(func()), duration);
});

module.exports.delay = async function() {
    const result = await setTimeoutP(() => {
        return 'this is a test';
    }, 2000);
};

const delayRange = module.exports.delayRange = async function*(limit, delay) {
    for (let i = 0; i < limit; i++) {
        await setTimeoutP(() => {}, delay);
        yield i;
    }
};

async function test() {
    for (const i of delayRange2(10, 500)) {
        console.log(i);
    }
}

test();

class DisjointSet {
    constructor() {
        this.sets = [];
        this.sizes = [];
    }
    
    find(i) {
        const child = this.sets[i];
        const parent = this.sets[child];
        if (child !== parent) {
            sets[i] = this.find(parent);
        }
        return sets[i];
    }

    union(i, j) {
        const iRoot = this.find(i);
        const jRoot = this.find(j);

        if (iRoot === jRoot) {
            return;
        }

        const iSize = this.sizes[iRoot] || 0;
        const jSize = this.sizes[jRoot] || 0;
        if (iSize >= jSize) {
            this.sizes[iRoot] = iSize + jSize;
            this.sets[jRoot] = iRoot;
        } else {
            this.sizes[jRoot] = iSize + jSize;
            this.sets[iRoot] = iRoot;
        }
    }
}
