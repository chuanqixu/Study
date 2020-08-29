public class ArrayDeque<T> implements Deque<T> {
    private int nextFirst;
    private int nextLast;
    private int size;
    private T[] items;

    public ArrayDeque() {
        nextFirst = 0;
        nextLast = 1;
        size = 0;
        items = (T[]) new Object[8];
    }

    public ArrayDeque(ArrayDeque other) {
        items = (T[]) new Object[other.size];
        nextFirst = other.nextFirst;
        nextLast = other.nextLast;
        size = other.size;
        System.arraycopy(other.items, 0, items, 0, other.size);
    }

    private void resize(int capacity) {
        if (capacity >= size) {
            T[] previousItems = items;
            items = (T[]) new Object[capacity];
            for (int i = 0; i < size; i++) {
                nextFirst = (nextFirst + 1) % previousItems.length;
                items[i] = previousItems[nextFirst];
            }
            nextFirst = capacity - 1;
            nextLast = size;
        }
    }

    @Override
    public void addFirst(T item) {
        if (size + 1 > items.length) {
            resize(items.length * 2);
        }
        items[nextFirst] = item;
        nextFirst = (nextFirst + items.length - 1) % items.length;
        size++;
    }

    @Override
    public void addLast(T item) {
        if (size + 1 > items.length) {
            resize(items.length * 2);
        }
        items[nextLast] = item;
        nextLast = (nextLast + 1) % items.length;
        size++;
    }

    @Override
    public int size() {
        return size;
    }

    @Override
    public void printDeque() {
        for (int i = (nextFirst + 1) % items.length;
             (i - nextFirst + items.length - 1) % items.length < size;
             i = (i + 1) % items.length) {
            System.out.print(items[i] + " ");
        }
        System.out.printf("\n");
    }

    @Override
    public T removeFirst() {
        if (isEmpty()) {
            return null;
        }
        if (size - 1 < items.length / 4 && size >= 16) {
            resize(items.length / 2);
        }
        nextFirst = (nextFirst + 1) % items.length;
        T temp = items[nextFirst];
        items[nextFirst] = null;
        size--;
        return temp;
    }

    @Override
    public T removeLast() {
        if (isEmpty()) {
            return null;
        }
        if (size - 1 < items.length / 4) {
            resize(items.length / 2);
        }
        nextLast = (nextLast + items.length - 1) % items.length;
        T temp = items[nextLast];
        items[nextLast] = null;
        size--;
        return temp;
    }

    @Override
    public T get(int index) {
        if (index >= size) {
            return null;
        }
        return items[(index + nextFirst + 1) % items.length];
    }
}
