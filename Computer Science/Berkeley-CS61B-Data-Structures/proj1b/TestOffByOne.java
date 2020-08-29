import org.junit.Test;

import static org.junit.Assert.*;

public class TestOffByOne {
    // You must use this CharacterComparator and not instantiate
    // new ones, or the autograder might be upset.
    static CharacterComparator offByOne = new OffByOne();

    // Your tests go here.
    @Test
    public void testEqualChairs() {
        assertTrue(offByOne.equalChars('a', 'b'));
        assertTrue(offByOne.equalChars('r', 'q'));
        assertTrue(offByOne.equalChars('A', 'B'));
        assertTrue(offByOne.equalChars('R', 'Q'));
        assertTrue(offByOne.equalChars('&', '%'));
        assertTrue(offByOne.equalChars('%', '&'));
        assertTrue(offByOne.equalChars('1', '2'));
        assertTrue(offByOne.equalChars('2', '1'));
        assertTrue(offByOne.equalChars('a', '`'));
        assertTrue(offByOne.equalChars('`', 'a'));
        assertTrue(offByOne.equalChars('z', '{'));
        assertTrue(offByOne.equalChars('{', 'z'));
        assertTrue(offByOne.equalChars('A', '@'));
        assertTrue(offByOne.equalChars('@', 'A'));
        assertTrue(offByOne.equalChars('Z', '['));
        assertTrue(offByOne.equalChars('[', 'Z'));

        assertFalse(offByOne.equalChars('a', 'e'));
        assertFalse(offByOne.equalChars('z', 'a'));
        assertFalse(offByOne.equalChars('a', 'a'));
        assertFalse(offByOne.equalChars('a', 'A'));
        assertFalse(offByOne.equalChars('A', 'C'));
        assertFalse(offByOne.equalChars('a', 'B'));
        assertFalse(offByOne.equalChars('B', 'a'));
    }
}
