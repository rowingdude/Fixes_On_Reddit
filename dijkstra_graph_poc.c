int minimumInitialValue(int lin, int col, int grid[][MAX]) {
    // use the smaller dimension for the cost array
    int minDim = min(lin, col);
    int maxDim = max(lin, col);
    int cost[MAX];

    // calculate cost using smaller dimension as row size
    bool isRowSmaller = lin <= col;
    for (int i = 0; i < maxDim; i++) {
        for (int j = 0; j < minDim; j++) {
            int val = grid[isRowSmaller ? j : i][isRowSmaller ? i : j];
            if (i == 0 && j == 0) {
                cost[j] = val > 0 ? 1 : 1 - val;
            } else if (j == 0) {
                cost[j] = max(1, cost[j] - val);
            } else if (i == 0) {
                cost[j] = max(1, cost[j-1] - val);
            } else {
                cost[j] = min(max(1, cost[j] - val), max(1, cost[j-1] - val));
            }
        }
    }
    return cost[minDim - 1];
}
