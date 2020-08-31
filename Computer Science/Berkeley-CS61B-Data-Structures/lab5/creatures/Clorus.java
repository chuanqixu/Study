package creatures;

import edu.princeton.cs.algs4.StdRandom;
import huglife.Action;
import huglife.Creature;
import huglife.Direction;
import huglife.Occupant;

import static huglife.HugLifeUtils.randomEntry;

import java.awt.Color;
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.Map;

public class Clorus extends Creature {

    public Clorus(double e) {
        super("clorus");
        energy = e;
    }

    public Color color() {
        return color(34, 0, 231);
    }

    public void attack(Creature c) {
        energy += c.energy();
    }

    public void move() {
        energy = (energy > 0.03) ? (energy - 0.03) : 0;
    }

    public void stay() {
        energy = (energy > 0.01) ? (energy - 0.01) : 0;
    }

    public Clorus replicate() {
        energy *= 0.5;
        return new Clorus(energy);
    }

    public Action chooseAction(Map<Direction, Occupant> neighbors) {
        // Rule 1
        Deque<Direction> emptyNeighbors = new ArrayDeque<>();
        Deque<Direction> plipNeighbors = new ArrayDeque<>();
        // TODO
        // (Google: Enhanced for-loop over keys of NEIGHBORS?)
        for (Map.Entry<Direction, Occupant> entry : neighbors.entrySet()) {
            if (entry.getValue().name() == "empty") {
                emptyNeighbors.add(entry.getKey());
            } else if (entry.getValue().name() == "plip") {
                plipNeighbors.add(entry.getKey());
            }
        }

        if (emptyNeighbors.size() == 0) { // FIXME
            // TODO
            return new Action(Action.ActionType.STAY);
        }

        // Rule 2
        // HINT: randomEntry(emptyNeighbors)
        if (plipNeighbors.size() != 0) {
            return new Action(Action.ActionType.ATTACK, randomEntry(plipNeighbors));
        }

        // Rule 3
        if (energy >= 1) {
            return new Action(Action.ActionType.REPLICATE, randomEntry(emptyNeighbors));
        }

        // Rule 4
        return new Action(Action.ActionType.MOVE, randomEntry(emptyNeighbors));
    }
}
