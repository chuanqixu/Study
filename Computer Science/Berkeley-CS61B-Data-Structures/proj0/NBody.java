public class NBody {

    public static double readRadius(String file) {
        In in = new In(file);
        in.readInt();
        return in.readDouble();
    }

    public static Body[] readBodies(String file) {
        In in = new In(file);
        Body[] nBodies = new Body[in.readInt()];
        in.readDouble();
        for (int i = 0; i < nBodies.length; i++) {
            nBodies[i] = new Body(in.readDouble(), in.readDouble(), in.readDouble(), in.readDouble(), in.readDouble(), in.readString());
        }
        return nBodies;
    }

    public static void main(String[] args) {
        double T = Double.parseDouble(args[0]);
        double dt = Double.parseDouble(args[1]);
        String filename = args[2];

        double radius = readRadius(filename);
        Body[] nBodies = readBodies(filename);

        String bgFile = "images/starfield.jpg";
        StdDraw.enableDoubleBuffering();
        StdDraw.setScale(-3E11, 3E11);
        StdDraw.picture(0, 0, bgFile);

        for (Body body : nBodies) {
            body.draw();
        }

        double time = 0;
        double[] xForces = new double[nBodies.length];
        double[] yForces = new double[nBodies.length];
        while (time < T) {
            StdDraw.clear();
            StdDraw.picture(0, 0, bgFile);
            for (int i = 0; i < xForces.length; i++) {
                xForces[i] = nBodies[i].calcNetForceExertedByX(nBodies);
                yForces[i] = nBodies[i].calcNetForceExertedByY(nBodies);
            }
            for (int i = 0; i < nBodies.length; i++) {
                nBodies[i].update(dt, xForces[i], yForces[i]);
                nBodies[i].draw();
            }
            StdDraw.show();
            StdDraw.pause(10);
            time += dt;
        }
        StdOut.printf("%d\n", nBodies.length);
        StdOut.printf("%.2e\n", radius);
        for (int i = 0; i < nBodies.length; i++) {
            StdOut.printf("%11.4e %11.4e %11.4e %11.4e %11.4e %12s\n",
                    nBodies[i].xxPos, nBodies[i].yyPos, nBodies[i].xxVel,
                    nBodies[i].yyVel, nBodies[i].mass, nBodies[i].imgFileName);
        }
    }
}
