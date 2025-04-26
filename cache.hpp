class Cache {
public:
    Cache(int size, int blockSize) : size(size), blockSize(blockSize) {
        numBlocks = size / blockSize;
        cacheData.resize(numBlocks);
        for (int i = 0; i < numBlocks; ++i) {
            cacheData[i].resize(blockSize);
        }
    }

    void read(int address) {
        // Implement read logic
    }

    void write(int address, int data) {
        // Implement write logic
    }
}