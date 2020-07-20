/* *****************************************************************************
 *  Name:              Chuanqi Xu
 *  Coursera User ID:  466018122@qq.com
 *  Last modified:     7/20/2020
 **************************************************************************** */

import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdStats;

public class PercolationStats {
    private double[] threshold;
    private int trials;
    private final double confidence = 1.96;

    public PercolationStats(int n, int trials) {
        if (n < 1 || trials < 1)
            throw new IllegalArgumentException("size or trials can not be smaller than 1!");

        threshold = new double[trials];
        this.trials = trials;
        for (int i = 0; i < trials; i++) {
            Percolation trial = new Percolation(n);
            while (!trial.percolates())
                trial.open(StdRandom.uniform(n) + 1, StdRandom.uniform(n) + 1);
            threshold[i] = (double) trial.numberOfOpenSites() / (n * n);
        }
    }

    public double mean() {
        return StdStats.mean(threshold);
    }

    public double stddev() {
        return StdStats.stddev(threshold);
    }

    public double confidenceLo() {
        return mean() - confidence * stddev() / Math.sqrt(trials);
    }

    public double confidenceHi() {
        return mean() + confidence * stddev() / Math.sqrt(trials);
    }

    public static void main(String[] args) {
        int n = Integer.parseInt(args[0]);
        int trials = Integer.parseInt(args[1]);
        PercolationStats test = new PercolationStats(n, trials);
        System.out.println("mean\t\t\t= " + test.mean());
        System.out.println("stddev\t\t\t= " + test.stddev());
        System.out.println(
                "95%% confidence interval\t= [" + test.confidenceLo() + ", " + test.confidenceHi()
                        + "]");
    }
}
