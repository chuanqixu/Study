
# Introduction

[Assignment description](./18-Assignment-4-ADTs-and-Recursion.pdf).

This assignment implements three recursive problems:

1. Doctors Without Orders: assign patients to doctors. Each patient has the required hours, and each doctor works for a number of hours. This problem is similar to assigning a series of short line segments to a series of long line segments.
2. Disaster Preparation: find a set of nodes in a graph where they and their adjacent nodes can cover the whole graph.
3. Winning the Presidency: dynamic programming but uses recursions instead of loops. Similar to [knapsack problem](https://en.wikipedia.org/wiki/Knapsack_problem).



# Note

Because of the path issue, in `Main.cpp`, I added one local variable `string relative_path  = "../"`. For each test code in `Main.cpp`, if test files are needed, I added this to specify the relative path.

For the assignment, change line 32 to:

```cpp
string relativePath = relative_path + directory + "/" + filename;
```


# Doctors Without Orders

Easy example with the exhaustive search using recursion.

Add another helper function to implement the recursion, though this helper function is not needed. The main addition of the helper function is the parameter `Map<string, int> &remain_time` that stores each `Doctor` remaining time, but this can also be done every time to sum up the time in `schedule`.

The recursion function transverses all the patients. In every recursion step, the function removes this patient from the list, assigns the last patient in `patients` to one doctor in `doctors`, and checks if the remaining patients can be assigned. The strategy will be shown in the following. Remember for backtracking, need to reverse the changes made in chosen possible solutions. The base case is the size of `patients` goes to 0, which means the work has been done, i.e., all patients are assigned, so return `true`. Otherwise, if the current assignment is not possible, the size of `patients` cannot be 0 because the strategy does not go into such recursion.

The strategy is to transverse all doctors into `doctors`. The possible assignment is:

1. If the chosen patient can be assigned to the current transversed doctor, i.e., the remaining time for the doctor is larger or equal to the needed time of the chosen patient, assign the current chosen patient to the current transversed doctor by adding it in `schedule`, and call the recursion function with `remain_time` changes and the current chosen patient removed from `patients`.
2. If the chosen patient cannot be assigned to the current transversed doctor, then nothing will happen.

Because I do not want to show the empty set for the doctor with no patient, I also check when the doctor is not shown in `schedule` and `remain_time`.

The code for the helper function is:

```cpp
bool canAllPatientsBeSeenHelper(const Vector<Doctor>& doctors,
                                const Vector<Patient>& patients,
                                Map<string, Set<string>>& schedule,
                                Map<string, int> &remain_time) {
    if (patients.size() == 0) {
        return true;
    }

    bool canSchedule = false;
    Vector<Patient> remain_patients(patients);
    auto patient = remain_patients.pop_back();
    for (const auto &doctor : doctors) {
        if (!schedule.containsKey(doctor.name)) {
            if (patient.hoursNeeded <= doctor.hoursFree) {
                schedule[doctor.name] = Set<string> {patient.name};
                remain_time[doctor.name] = doctor.hoursFree - patient.hoursNeeded;
                canSchedule = canAllPatientsBeSeenHelper(doctors, remain_patients, schedule, remain_time);
                if (canSchedule) {
                    break;
                }
                schedule.remove(doctor.name);
                remain_time.remove(doctor.name);
            }
        } else if (patient.hoursNeeded <= remain_time[doctor.name]) {
            schedule[doctor.name].add(patient.name);
            remain_time[doctor.name] -= patient.hoursNeeded;
            canSchedule = canAllPatientsBeSeenHelper(doctors, remain_patients, schedule, remain_time);
            if (canSchedule) {
                break;
            }
            schedule[doctor.name].remove(patient.name);
            remain_time[doctor.name] += patient.hoursNeeded;
        }
    }

    return canSchedule;
}
```

The entry function to call this helper function is easy, just initialize `remain_time`:

```cpp
bool canAllPatientsBeSeen(const Vector<Doctor>& doctors,
                          const Vector<Patient>& patients,
                          Map<string, Set<string>>& schedule) {
////     [TODO: Delete these lines and implement this function!]
//    (void)(doctors, patients, schedule);

    Map<string, int> remain_time;
    return canAllPatientsBeSeenHelper(doctors, patients, schedule, remain_time);
}
```




# Disaster Preparation

Create a helper function that takes another `Set<string> &remain_cities` as the parameter, which contains all remaining cities to cover. In addition, change `roadNetwork` to another parameter `Map<string, Set<string>> &remain_network` which contains only cities in `remain_cities`.

The strategy is:

1. Choose one city in `remain_cities`. This city is considered to be the city to cover in this recursive step.
2. Iterate all cities in `remain_network`. For each iteration, choose one city in `remain_network`, and name it `chosen_city`, which means to be chosen to be added into `locations`. Because we need to cover the city chosen in step 1, `chosen_city` must be either the city in step 1 itself or its adjacent city.
3. If `chosen_city` satisfies the above condition, remove related cities in `remain_cities` and `remain_network`. Then recursively call the helper function to search for another city. `numCities` needs to be decreased by 1 because we have chosen one city.
4. If the recursive call returns true, directly return without checking other possibilities.
5. If not, remember to restore all changes after the call.

The helper function is:

```cpp
int canBeMadeDisasterReadyHelper(Map<string, Set<string>> &remain_network,
                                  int numCities,
                                  Set<string>& locations,
                                  Set<string> &remain_cities) {
    if (remain_cities.isEmpty()) {
        return true;
    }
    if (numCities < 1) {
        return false;
    }

    auto prev_remain_network = remain_network;
    auto prev_remain_cities = remain_cities;

    // city is the city to cover in this recursive step
    auto city = remain_cities.front();

    // chosen city is the city to choose in the exaustive search path
    for (const auto &chosen_city : prev_remain_network.keys()) {
        // the city can be covered either the chosen city is itself or the chosen city is adjacent to this city
        if (chosen_city == city || remain_network[chosen_city].contains(city)) {
            locations.add(chosen_city);

            // update remain_cities and remain_network
            remain_cities.remove(chosen_city);
            for (const auto &adj_city : prev_remain_network[chosen_city]) {
                remain_cities.remove(adj_city);
                for (const auto &remain_city : remain_network.keys()) {
                    remain_network[remain_city].remove(adj_city);
                }
            }
            remain_network.remove(chosen_city);
            for (const auto &remain_city : remain_network.keys()) {
            if (remain_network[remain_city].isEmpty()) {
                remain_network.remove(remain_city);
            }
        }

        bool disasterReady = canBeMadeDisasterReadyHelper(remain_network, numCities - 1, locations, remain_cities);
        if (disasterReady) {
            return true;
        }

        // restore previous state
        locations.remove(chosen_city);
        remain_cities = prev_remain_cities;
        remain_network = prev_remain_network;
        }
    }

    return false;
}

```

The entry function is straightforward:

```cpp
bool canBeMadeDisasterReady(const Map<string, Set<string>>& roadNetwork,
                            int numCities,
                            Set<string>& locations) {
//    // [TODO: Delete these lines and implement this function!]
//    (void)(roadNetwork, numCities, locations);

    auto remain_network = roadNetwork;
    Set<string> remain_cities;
    for (const auto &city : roadNetwork.keys()) {
        remain_cities.add(city);
    }
    return canBeMadeDisasterReadyHelper(remain_network, numCities, locations, remain_cities);
}
```




# Winning the Presidency

Simple assignment of recursion and dynamic programming.

The strategy is simple. In the recursive step, consider whether to include the state at `minStateIndex` by checking the number of votes and choosing the smaller one.

Without memoization, the code is below:

```cpp
/**
 * Given a list of the states in the election, including their popular and electoral vote
 * totals, and the number of electoral votes needed, as well as the index of the lowest-indexed
 * state to consider, returns information about how few popular votes you'd need in order to
 * win that at least that many electoral votes.
 *
 * @param electoralVotesNeeded the minimum number of electoral votes needed
 * @param states All the states in the election (plus DC, if appropriate)
 * @param minStateIndex the lowest index in the states Vector that should be considered
 */
MinInfo minPopularVoteToGetAtLeast(int electoralVotesNeeded, const Vector<State>& states, int minStateIndex) {
//    // [TODO: Delete these lines and implement this function!]
//    (void)(electoralVotesNeeded);
//    (void)(states);
//    (void)(minStateIndex);

    // Base case
    if (minStateIndex == states.size() - 1) {
        if (electoralVotesNeeded <= 0) {
            return {0, {}};
        } else if (electoralVotesNeeded <= states[minStateIndex].electoralVotes) {
            return {states[minStateIndex].popularVotes / 2 + 1, {states[minStateIndex]}};
        } else {
            return {INT_MAX, {}};
        }
    }

    int min_pop_votes_curr = states[minStateIndex].popularVotes / 2 + 1;

    // Choose the state at minStateIndex
    MinInfo choose = minPopularVoteToGetAtLeast(electoralVotesNeeded - states[minStateIndex].electoralVotes, states, minStateIndex + 1);

    // Do not choose the state at minStateIndex
    MinInfo not_choose = minPopularVoteToGetAtLeast(electoralVotesNeeded, states, minStateIndex + 1);

    // Check whether any of them is not possible
    if (choose.popularVotesNeeded == INT_MAX) {
        if (not_choose.popularVotesNeeded == INT_MAX) {
            return {INT_MAX, {}};
        } else {
            return not_choose;
        }
    } else if (not_choose.popularVotesNeeded == INT_MAX) {
        return {choose.popularVotesNeeded + min_pop_votes_curr, choose.statesUsed + Vector<State> {states[minStateIndex]}};
    } else {
        if (choose.popularVotesNeeded + min_pop_votes_curr < not_choose.popularVotesNeeded) {
            return {choose.popularVotesNeeded + min_pop_votes_curr, choose.statesUsed + Vector<State> {states[minStateIndex]}};
        } else {
            return not_choose;
        }
    }
}
```

With memoization, need to check whether the data has been acquired before. If it has already existed, directly read the data. Otherwise, acquire that data first.

Note that because `electoralVotesNeeded` can be a negative number, need to change it to 0. Otherwise, the index of `Grid` cannot be accessed.

Dynamic programming usually uses loops instead of recursion. But given that the assignment requires recursion, I still used it.

The code is below:

```cpp
/**
 * Given a list of the states in the election, including their popular and electoral vote
 * totals, and the number of electoral votes needed, as well as the index of the lowest-indexed
 * state to consider, returns information about how few popular votes you'd need in order to
 * win that at least that many electoral votes.
 *
 * @param electoralVotesNeeded the minimum number of electoral votes needed
 * @param states All the states in the election (plus DC, if appropriate)
 * @param minStateIndex the lowest index in the states Vector that should be considered
 * @param mem the SparseGrid used for memoization
 */
void minPopularVoteToGetAtLeast(int electoralVotesNeeded, const Vector<State>& states, int minStateIndex, SparseGrid<MinInfo> &mem) {
//    // [TODO: Delete these lines and implement this function!]
//    (void)(electoralVotesNeeded);
//    (void)(states);
//    (void)(minStateIndex);

    MinInfo mininfo;

    // Base case
    if (minStateIndex == states.size() - 1) {
        if (electoralVotesNeeded <= 0) {
            mininfo = {0, {}};
        } else if (electoralVotesNeeded <= states[minStateIndex].electoralVotes) {
            mininfo = {states[minStateIndex].popularVotes / 2 + 1, {states[minStateIndex]}};
        } else {
            mininfo = {INT_MAX, {}};
        }
        mem[electoralVotesNeeded][minStateIndex] = mininfo;
        return;
    }

    int min_pop_votes_curr = states[minStateIndex].popularVotes / 2 + 1;

    // Choose the state at minStateIndex
    MinInfo choose;
    int next_electoralVotesNeeded = electoralVotesNeeded - states[minStateIndex].electoralVotes;
    next_electoralVotesNeeded = next_electoralVotesNeeded < 0 ? 0 : next_electoralVotesNeeded;
    if (mem[next_electoralVotesNeeded][minStateIndex + 1].popularVotesNeeded == -1) {
        minPopularVoteToGetAtLeast(next_electoralVotesNeeded, states, minStateIndex + 1, mem);
    }
    choose = mem[next_electoralVotesNeeded][minStateIndex + 1];
    if (choose.popularVotesNeeded != INT_MAX) {
        choose.popularVotesNeeded += min_pop_votes_curr;
        choose.statesUsed += Vector<State> {states[minStateIndex]};
    }

    // Do not choose the state at minStateIndex
    MinInfo not_choose;
    if (mem[electoralVotesNeeded][minStateIndex + 1].popularVotesNeeded == -1) {
        minPopularVoteToGetAtLeast(electoralVotesNeeded, states, minStateIndex + 1, mem);
    }
    not_choose = mem[electoralVotesNeeded][minStateIndex + 1];

    // Check whether any of them is not possible
    if (choose.popularVotesNeeded == INT_MAX) {
        if (not_choose.popularVotesNeeded == INT_MAX) {
            mininfo = {INT_MAX, {}};
        } else {
            mininfo = not_choose;
        }
    } else if (not_choose.popularVotesNeeded == INT_MAX) {
        mininfo = choose;
    } else {
        if (choose.popularVotesNeeded < not_choose.popularVotesNeeded) {
            mininfo = choose;
        } else {
            mininfo = not_choose;
        }
    }
    mem[electoralVotesNeeded][minStateIndex] = mininfo;
}

```


The entry function is straightforward. One thing that needs to note is to create one more row for memoization:

```cpp
/**
 * Given a list of all the states in an election, including their popular and electoral vote
 * totals, returns information about how few popular votes you'd need to win in order to win
 * the presidency.
 *
 * @param states All the states in the election (plus DC, if appropriate)
 * @return Information about how few votes you'd need to win the election.
 */
MinInfo minPopularVoteToWin(const Vector<State>& states) {
//    // [TODO: Delete these lines and implement this function!]
//    (void)(states);

    int electoral_votes_needed = 0;
    for (const auto &state : states) {
        electoral_votes_needed += state.electoralVotes;
    }
    electoral_votes_needed = electoral_votes_needed / 2 + 1;

    // One more row
    SparseGrid<MinInfo> mem(electoral_votes_needed + 1, states.size(), {-1, {}});

    minPopularVoteToGetAtLeast(electoral_votes_needed, states, 0, mem);

    return mem[electoral_votes_needed][0];
}
```
