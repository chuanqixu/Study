#include "RecursionToTheRescue.h"
#include "map.h"
#include <climits>
#include <iostream>
#include "Disasters.h"
using namespace std;

#include "sparsegrid.h"

/* * * * Doctors Without Orders * * * */


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


/**
 * Given a list of doctors and a list of patients, determines whether all the patients can
 * be seen. If so, this function fills in the schedule outparameter with a map from doctors
 * to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @return Whether or not a schedule was found.
 */
bool canAllPatientsBeSeen(const Vector<Doctor>& doctors,
                          const Vector<Patient>& patients,
                          Map<string, Set<string>>& schedule) {
////     [TODO: Delete these lines and implement this function!]
//    (void)(doctors, patients, schedule);

    Map<string, int> remain_time;
    return canAllPatientsBeSeenHelper(doctors, patients, schedule, remain_time);
}

/* * * * Disaster Planning * * * */


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

/**
 * Given a transportation grid for a country or region, along with the number of cities where disaster
 * supplies can be stockpiled, returns whether it's possible to stockpile disaster supplies in at most
 * the specified number of cities such that each city either has supplies or is connected to a city that
 * does.
 * <p>
 * This function can assume that every city is a key in the transportation map and that roads are
 * bidirectional: if there's a road from City A to City B, then there's a road from City B back to
 * City A as well.
 *
 * @param roadNetwork The underlying transportation network.
 * @param numCities   How many cities you can afford to put supplies in.
 * @param locations   An outparameter filled in with which cities to choose if a solution exists.
 * @return Whether a solution exists.
 */
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


/* * * * Winning the Election * * * */



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
