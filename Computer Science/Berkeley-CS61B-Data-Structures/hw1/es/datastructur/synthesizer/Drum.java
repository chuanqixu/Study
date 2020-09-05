package es.datastructur.synthesizer;

public class Drum {
    /**
     * Constants. Do not change. In case you're curious, the keyword final
     * means the values cannot be changed at runtime.
     */
    private static final int SR = 44100;      // Sampling Rate
    private static final double DECAY = 1; // energy decay factor

    /* Buffer for storing sound data. */
    private BoundedQueue<Double> buffer;

    /* Create a guitar string of the given frequency.  */
    public Drum(double frequency) {
        buffer = new ArrayRingBuffer<>((int) Math.round(SR / frequency));
        for (int i = 0; i < buffer.capacity(); i++) {
            buffer.enqueue(0.0);
        }
    }


    /* Pluck the guitar string by replacing the buffer with white noise. */
    public void pluck() {
        //       Make sure that your random numbers are different from each
        //       other.
        for (int i = 0; i < buffer.capacity(); i++) {
            buffer.dequeue();
            buffer.enqueue(Math.random() - 0.5);
        }
    }

    /* Advance the simulation one time step by performing one iteration of
     * the Karplus-Strong algorithm.
     */
    public void tic() {
        if (Math.random() < 0.5) {
            buffer.enqueue((buffer.dequeue() + buffer.peek()) * DECAY / 2);
        } else {
            buffer.enqueue(-(buffer.dequeue() + buffer.peek()) * DECAY / 2);
        }

    }

    /* Return the double at the front of the buffer. */
    public double sample() {
        return buffer.peek();
    }
}
