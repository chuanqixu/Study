public class Body {
    public double xxPos;
    public double yyPos;
    public double xxVel;
    public double yyVel;
    public double mass;
    public String imgFileName;
    private final double gravitationalConst = 6.67E-11;

    public Body(double xP, double yP, double xV, double yV, double m, String img) {
        xxPos = xP;
        yyPos = yP;
        xxVel = xV;
        yyVel = yV;
        mass = m;
        imgFileName = img;
    }

    public Body(Body b) {
        xxPos = b.xxPos;
        yyPos = b.yyPos;
        xxVel = b.xxVel;
        yyVel = b.yyVel;
        mass = b.mass;
        imgFileName = b.imgFileName;
    }

    public double calcDistance(Body body) {
        return Math.sqrt(Math.pow(xxPos - body.xxPos, 2) + Math.pow(yyPos - body.yyPos, 2));
    }

    public double calcForceExertedBy(Body body) {
        return gravitationalConst * mass * body.mass / Math.pow(calcDistance(body), 2);
    }

    public double calcForceExertedByX(Body body) {
        return calcForceExertedBy(body) * (body.xxPos - xxPos) / calcDistance(body);
    }

    public double calcForceExertedByY(Body body) {
        return calcForceExertedBy(body) * (body.yyPos - yyPos) / calcDistance(body);
    }

    public double calcNetForceExertedByX(Body[] allBodies) {
        double xxForce = 0;
        for (Body body : allBodies) {
            if (this.equals(body)) {
                continue;
            }
            xxForce += calcForceExertedByX(body);
        }
        return xxForce;
    }

    public double calcNetForceExertedByY(Body[] allBodies) {
        double yyForce = 0;
        for (Body body : allBodies) {
            if (this.equals(body)) {
                continue;
            }
            yyForce += calcForceExertedByY(body);
        }
        return yyForce;
    }

    public void update(double dt, double fX, double fY) {
        xxVel += dt * fX / mass;
        yyVel += dt * fY / mass;
        xxPos += dt * xxVel;
        yyPos += dt * yyVel;
    }

    public void draw() {
        StdDraw.picture(xxPos, yyPos, "images/" + imgFileName);
    }
}
