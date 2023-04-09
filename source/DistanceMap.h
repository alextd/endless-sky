/* DistanceMap.h
Copyright (c) 2014 by Michael Zahniser

Endless Sky is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

Endless Sky is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef DISTANCE_MAP_H_
#define DISTANCE_MAP_H_

#include "RouteEdge.h"
#include "WormholeStrategy.h"

#include <map>
#include <queue>
#include <set>
#include <utility>
#include <vector>

class PlayerInfo;
class Ship;
class System;



// This is a map of shortest routes to all other systems from the given "center"
// system. It also tracks how many days and fuel it takes to get there. Ships with
// a hyperdrive travel using the "links" between systems. Ships with jump drives
// can make use of those links, but can also travel to any system that's closeby.
// Wormholes can also be used by players or ships.
class DistanceMap {
public:
	// Find paths branching out from the given system. The optional arguments put
	// a limit on how many systems will be returned (e.g. buying a local map) and
	// a limit on how many jumps away they can be (e.g. a valid mission location).
	explicit DistanceMap(const System *center, int maxSystems = -1, int maxDays = -1);
	// If a player is given, the map will start from the player's system.
	// Pathfinding will only use hyperspace paths known to the player; that is,
	// one end of the path has been visited. Also, if the ship has a jump drive
	// or wormhole access, the route will make use of it.
	explicit DistanceMap(const PlayerInfo &player);
	// Find paths to the given system, potentially using wormholes, a jump drive, or both.
	// Optional arguments are as above.
	explicit DistanceMap(const System *center, WormholeStrategy wormholeStrategy,
			bool useJumpDrive, int maxSystems = -1, int maxDays = -1);
	// Calculate the path for the given ship to get to the given system. The
	// ship will use a jump drive or hyperdrive depending on what it has. The
	// pathfinding will stop once a path to the destination is found.
	DistanceMap(const Ship &ship, const System &destination);

	// Find out if the given system is reachable.
	bool HasRoute(const System &system) const;
	// Find out how many days away the given system is.
	int Days(const System &system) const;
	// Get the planned route from center to this system.
	std::vector<const System *> Plan(const System &system) const;
	// Get a set containing all the systems.
	std::set<const System *> Systems() const;


private:
	// With the optional destination, the pathfinding will stop once it finds the
	// best path to it. You must use RoutePlan to set a destination.
	explicit DistanceMap(const System &center, const System &destination);
	// The center argument starts the path there instead of the player
	// (e.g. appending a route to the end of a planned route).
	explicit DistanceMap(const PlayerInfo &player, const System &center, const System &destination);

	// Depending on the capabilities of the given ship, use hyperspace paths,
	// jump drive paths, or both to find the shortest route. Bail out if the
	// destination system or the maximum count is reached.
	void Init(const Ship *ship = nullptr);
	// Add the given links to the map. Return false if an end condition is hit.
	bool Propagate(RouteEdge edge, bool useJump);
	// Check if we already have a better path to the given system.
	bool HasBetter(const System &to, const RouteEdge &edge);
	// Add the given path to the record.
	void Add(const System &to, RouteEdge edge);
	// Check whether the given link is travelable. If no player was given in the
	// constructor then this is always true; otherwise, the player must know
	// that the given link exists.
	bool CheckLink(const System &from, const System &to, bool useJump) const;


private:
	// Final route, each Edge pointing to the previous step along the route.
	std::map<const System *, RouteEdge> route;

	// Variables only used during construction:
	// 'edgesTodo' holds unfinished candidate Edges - Only the 'prev' value
	// is up-to-date. Other values are one step behind, awaiting an update.
	// The top() value is the best route among uncertain systems. Once
	// popped, that's the best route to that system - and then adjacent links
	// from that system are processed, which will build upon the popped Edge.
	std::priority_queue<RouteEdge> edgesTodo;
	const PlayerInfo *player = nullptr;
	const System *center = nullptr;
	int maxSystems = -1;
	int maxDays = -1;
	const System *destination = nullptr;
	WormholeStrategy wormholeStrategy = WormholeStrategy::ALL;
	// How much fuel is used for travel. If either value is zero, it means that
	// the ship does not have that type of drive.
	// Defaults are set for hyperlane usage only. Using a ship overrides these.
	int hyperspaceFuel = 100;
	int jumpFuel = 0;
	bool useWormholes = false;
	double jumpRange = 0.;

	friend class RoutePlan;
};



#endif
