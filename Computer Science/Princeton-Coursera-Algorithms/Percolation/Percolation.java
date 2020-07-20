/* *****************************************************************************
 *  Name:              Chuanqi Xu
 *  Coursera User ID:  466018122@qq.com
 *  Last modified:     7/20/2020
 **************************************************************************** */

import edu.princeton.cs.algs4.WeightedQuickUnionUF;

public class Percolation {
    private WeightedQuickUnionUF grid;
    private WeightedQuickUnionUF gridForFull;
    private boolean[] opensite;
    private int n;
    private int opennum;

    public Percolation(int n) {
        if (n < 1)
            throw new IllegalArgumentException("size can not be smaller than 1!");

        grid = new WeightedQuickUnionUF(n * n + 2);
        gridForFull = new WeightedQuickUnionUF(n * n + 1);
        opensite = new boolean[n * n];
        this.n = n;
        opennum = 0;
    }

    private int getID(int row, int col) {
        if ((row < 1 || row > n) || (col < 1 || col > n))
            throw new IllegalArgumentException("Argument is outside its prescribed range!");

        return ((row - 1) * n + (col - 1));
    }

    public void open(int row, int col) {
        int id = getID(row, col);
        if (!opensite[id]) {
            opensite[id] = true;
            opennum++;
            if (row == 1) {
                grid.union(id, n * n);
                gridForFull.union(id, n * n);
            }
            if (row == n)
                grid.union(id, n * n + 1);
            if (col != 1 && isOpen(row, col - 1)) {
                grid.union(id, getID(row, col - 1));
                gridForFull.union(id, getID(row, col - 1));
            }
            if (col != n && isOpen(row, col + 1)) {
                grid.union(id, getID(row, col + 1));
                gridForFull.union(id, getID(row, col + 1));
            }
            if (row != 1 && isOpen(row - 1, col)) {
                grid.union(id, getID(row - 1, col));
                gridForFull.union(id, getID(row - 1, col));
            }
            if (row != n && isOpen(row + 1, col)) {
                grid.union(id, getID(row + 1, col));
                gridForFull.union(id, getID(row + 1, col));
            }
        }
    }

    public boolean isOpen(int row, int col) {
        return opensite[getID(row, col)];
    }

    public boolean isFull(int row, int col) {
        return gridForFull.find(getID(row, col)) == gridForFull.find(n * n);
    }

    public int numberOfOpenSites() {
        return opennum;
    }

    public boolean percolates() {
        return grid.find(n * n) == grid.find(n * n + 1);
    }
}
