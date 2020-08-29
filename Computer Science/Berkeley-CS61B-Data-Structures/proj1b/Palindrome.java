public class Palindrome {
    public Deque<Character> wordToDeque(String word) {
        Deque<Character> deque = new LinkedListDeque<>();
        for (int i = 0; i < word.length(); i++) {
            deque.addLast(word.charAt(i));
        }
        return deque;
    }

    private boolean isPalindrome(Deque<Character> chars) {
        if (chars.size() == 0 || chars.size() == 1) {
            return true;
        }
        if (chars.removeFirst().equals(chars.removeLast())) {
            return isPalindrome(chars);
        } else {
            return false;
        }
    }

    public boolean isPalindrome(String word) {
        return isPalindrome(wordToDeque(word));
    }

    private boolean isPalindrome(Deque<Character> chars, CharacterComparator cc) {
        if (chars.size() == 0 || chars.size() == 1) {
            return true;
        }
        if (cc.equalChars(chars.removeFirst(), chars.removeLast())) {
            return isPalindrome(chars, cc);
        } else {
            return false;
        }
    }

    public boolean isPalindrome(String word, CharacterComparator cc) {
        return isPalindrome(wordToDeque(word), cc);
    }
}
