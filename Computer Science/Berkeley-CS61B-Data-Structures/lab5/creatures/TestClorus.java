package creatures;

import huglife.*;
import huglife.Action;
import org.junit.Test;

import javax.management.DescriptorRead;
import javax.swing.*;
import java.util.HashMap;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;

public class TestClorus {

    @Test
    public void testChoose() {

        // No empty adjacent spaces; stay.
        Clorus clorus = new Clorus(1.2);
        HashMap<Direction, Occupant> surrounded = new HashMap<Direction, Occupant>();
        surrounded.put(Direction.TOP, new Impassible());
        surrounded.put(Direction.BOTTOM, new Impassible());
        surrounded.put(Direction.LEFT, new Impassible());
        surrounded.put(Direction.RIGHT, new Impassible());

        Action actual = clorus.chooseAction(surrounded);
        Action expected = new Action(Action.ActionType.STAY);

        assertEquals(expected, actual);

        // Plip exists; attack it
        clorus = new Clorus(1.2);
        HashMap<Direction, Occupant> topPlip = new HashMap<Direction, Occupant>();
        topPlip.put(Direction.TOP, new Plip(1.2));
        topPlip.put(Direction.BOTTOM, new Impassible());
        topPlip.put(Direction.LEFT, new Impassible());
        topPlip.put(Direction.RIGHT, new Impassible());

        actual = clorus.chooseAction(topPlip);
        expected = new Action(Action.ActionType.ATTACK, Direction.TOP);

        // Plip exists; attack it
        clorus = new Clorus(1.2);
        HashMap<Direction, Occupant> twoPlips = new HashMap<Direction, Occupant>();
        twoPlips.put(Direction.TOP, new Plip(1.2));
        twoPlips.put(Direction.BOTTOM, new Plip(1.2));
        twoPlips.put(Direction.LEFT, new Empty());
        twoPlips.put(Direction.RIGHT, new Empty());

        actual = clorus.chooseAction(twoPlips);
        expected = new Action(Action.ActionType.ATTACK, Direction.TOP);

        // Neighbors are all plips; stay
        clorus = new Clorus(1.2);
        HashMap<Direction, Occupant> allPlip = new HashMap<Direction, Occupant>();
        allPlip.put(Direction.TOP, new Plip(1.2));
        allPlip.put(Direction.BOTTOM, new Plip(1.2));
        allPlip.put(Direction.LEFT, new Plip(1.2));
        allPlip.put(Direction.RIGHT, new Plip(1.2));

        actual = clorus.chooseAction(allPlip);
        expected = new Action(Action.ActionType.STAY);

        assertEquals(expected, actual);

        // Energy >= 1; replicate towards an empty space.
        clorus = new Clorus(1.2);
        HashMap<Direction, Occupant> topEmpty = new HashMap<Direction, Occupant>();
        topEmpty.put(Direction.TOP, new Empty());
        topEmpty.put(Direction.BOTTOM, new Impassible());
        topEmpty.put(Direction.LEFT, new Impassible());
        topEmpty.put(Direction.RIGHT, new Impassible());

        actual = clorus.chooseAction(topEmpty);
        expected = new Action(Action.ActionType.REPLICATE, Direction.TOP);

        assertEquals(expected, actual);


        // Energy >= 1; replicate towards an empty space.
        clorus = new Clorus(1.2);
        HashMap<Direction, Occupant> allEmpty = new HashMap<Direction, Occupant>();
        allEmpty.put(Direction.TOP, new Empty());
        allEmpty.put(Direction.BOTTOM, new Empty());
        allEmpty.put(Direction.LEFT, new Empty());
        allEmpty.put(Direction.RIGHT, new Empty());

        actual = clorus.chooseAction(allEmpty);
        Action unexpected = new Action(Action.ActionType.STAY);

        assertNotEquals(unexpected, actual);


        // Energy < 1; stay.
        clorus = new Clorus(.99);

        actual = clorus.chooseAction(allEmpty);

        assertEquals(Action.ActionType.MOVE, actual.type);


        // Energy < 1; stay.
        clorus = new Clorus(.99);

        actual = clorus.chooseAction(topEmpty);
        expected = new Action(Action.ActionType.MOVE, Direction.TOP);

        assertEquals(expected, actual);
    }
}
