public class LinkedListDeque<T> {

    private class Node {
        private T item;
        private Node prev;
        private Node next;

        public Node(T i, Node p, Node n) {
            item = i;
            prev = p;
            next = n;
        }
    }

    private Node sentinel;
    private int size;

    public LinkedListDeque() {
        sentinel = new Node(null, null, null);
        sentinel.prev = sentinel;
        sentinel.next = sentinel;
        size = 0;
    }

    public LinkedListDeque(LinkedListDeque other) {
        sentinel = new Node(null, null, null);
        sentinel.prev = sentinel;
        sentinel.next = sentinel;
        size = 0;

        Node p = other.sentinel.next;
        while (p != sentinel) {
            addLast(p.item);
            p = p.next;
        }
    }

    public void addFirst(T item) {
        sentinel.next = new Node(item, sentinel, sentinel.next);
        sentinel.next.next.prev = sentinel.next;
        size++;
    }

    public void addLast(T item) {
        sentinel.prev = new Node(item, sentinel.prev, sentinel);
        sentinel.prev.prev.next = sentinel.prev;
        size++;
    }

    public boolean isEmpty() {
        return size == 0;
    }

    public int size() {
        return size;
    }

    public void printDeque() {
        Node p = sentinel.next;
        while (p != sentinel) {
            System.out.print(p.item + " ");
            p = p.next;
        }
        System.out.printf("\n");
    }

    public T removeFirst() {
        if (isEmpty()) {
            return null;
        }
        Node temp = sentinel.next;
        sentinel.next = temp.next;
        sentinel.next.prev = sentinel;
        size--;
        return temp.item;
    }

    public T removeLast() {
        if (isEmpty()) {
            return null;
        }
        Node temp = sentinel.prev;
        sentinel.prev = temp.prev;
        sentinel.prev.next = sentinel;
        size--;
        return temp.item;
    }

    public T get(int index) {
        if (index >= size) {
            return null;
        }
        Node p = sentinel.next;
        while (p != sentinel) {
            if (index == 0) {
                return p.item;
            }
            p = p.next;
            index--;
        }
        return null;
    }

    private T getRecursive(int index, Node p) {
        if (index >= size) {
            return null;
        }
        if (index == 0) {
            return p.item;
        } else {
            return getRecursive(index - 1, p.next);
        }
    }

    public T getRecursive(int index) {
        return getRecursive(index, sentinel.next);
    }

}
