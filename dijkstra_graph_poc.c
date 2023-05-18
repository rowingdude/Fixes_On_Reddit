int minimumInitialValue(int lin, int col, int grid[][MAX]) {
    int cost[MAX][MAX];

    // Initialize the first cell
    cost[0][0] = grid[0][0] > 0 ? 1 : 1 - grid[0][0];

    // Calculate the cost for the first row
    for (int j = 1; j < col; j++) {
        cost[0][j] = max(1, cost[0][j-1] - grid[0][j]);
    }

    // Calculate the cost for the first column
    for (int i = 1; i < lin; i++) {
        cost[i][0] = max(1, cost[i-1][0] - grid[i][0]);
    }

    // Calculate the cost for the remaining cells
    for (int i = 1; i < lin; i++) {
        for (int j = 1; j < col; j++) {
            int costFromLeft = max(1, cost[i][j-1] - grid[i][j]);
            int costFromUp = max(1, cost[i-1][j] - grid[i][j]);
            cost[i][j] = min(costFromLeft, costFromUp);
        }
    }

    return cost[lin-1][col-1];
}
