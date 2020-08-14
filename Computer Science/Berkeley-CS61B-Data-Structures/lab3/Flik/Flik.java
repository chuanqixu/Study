/** An Integer tester created by Flik Enterprises. */
public class Flik {
    public static boolean isSameNumber(Integer a, Integer b) {
        /** Bug is here, Integer a and b is point to Integer object.
         * Usually they are not the same because they point to different address.
         * ex. Integer instances created using keyword "new" are always not the same.
         * However, if "int" between -128 and 127 is assigned to "Integer",
         * Java will cash it and a and b will point to the same object.
         */
        return a == b;
    }
}
