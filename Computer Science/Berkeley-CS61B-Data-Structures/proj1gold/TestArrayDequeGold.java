import static org.junit.Assert.*;

import org.junit.Test;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class TestArrayDequeGold {
    @Test
    public void testStudentArrayDeque() {
        StudentArrayDeque<Integer> sad = new StudentArrayDeque<>();
        ArrayDequeSolution<Integer> right = new ArrayDequeSolution<>();
        int numberOfOperations = 100;
        List<Integer> operations = new LinkedList<>();
        List<Integer> numbers = new ArrayList<>();

        for (int i = 0; i < numberOfOperations; i += 1) {
            int operationNumber = StdRandom.uniform(4);
            int addNumber = StdRandom.uniform(10);

            if (operationNumber == 0) {
                operations.add(0);
                numbers.add(addNumber);
                sad.addLast(addNumber);
                right.addLast(addNumber);
            } else if (operationNumber == 1) {
                operations.add(1);
                numbers.add(addNumber);
                sad.addFirst(addNumber);
                right.addFirst(addNumber);
            } else if (operationNumber == 2 && sad.size() > 0) {
                operations.add(2);
                assertEquals(errorMessage(operations, numbers), right.removeLast(), sad.removeLast());
            } else if (operationNumber == 3 && sad.size() > 0) {
                operations.add(3);
                assertEquals(errorMessage(operations, numbers), right.removeFirst(), sad.removeFirst());
            }
        }
    }

    private String errorMessage(List<Integer> operations, List<Integer> numbers) {
        StringBuilder message = new StringBuilder("");
        int j = 0;
        for (Integer i : operations) {
            if (i == 0) {
                message.append("addLast(" + numbers.get(j) + ")\n");
                j++;
            } else if (i == 1) {
                message.append("addFirst(" + numbers.get(j) + ")\n");
                j++;
            } else if (i == 2) {
                message.append("removeLast()\n");
            } else if (i == 3) {
                message.append("removeFirst()\n");
            }
        }
        return message.toString();
    }
}
