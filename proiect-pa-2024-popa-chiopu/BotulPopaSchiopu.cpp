#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <set>
#include <fstream>
#include <queue>
#include <vector>
#include <climits>

#include "hlt.hpp"
#include "networking.hpp"

#define OUTER_PRODUCTION_SCALE_CONSTANT 5.656
#define OUTER_PRODUCTION_SCALE_SCALE 0.002
#define INNER_PRODUCTION_SCALE 2
#define TURNS_BEFORE_LEAVING 4
#define ACCEPTED_STRENGTH_LOSS 50

// Inversează direcția primită (ex. NORTH => SOUTH)
unsigned char reverseDirection(unsigned char direction) {
    switch (direction) {
    case NORTH:
        return SOUTH;
    case EAST:
        return WEST;
    case SOUTH:
        return NORTH;
    case WEST:
        return EAST;
    }
    return STILL;
}

int main() {
    srand(time(NULL));

    std::cout.sync_with_stdio(0);

    unsigned char myID;
    hlt::GameMap presentMap;
    getInit(myID, presentMap);
    sendInit("BotulPopaSchiopu");

    const double outerProductionScale = OUTER_PRODUCTION_SCALE_CONSTANT + presentMap.width * presentMap.height * OUTER_PRODUCTION_SCALE_SCALE;
    const double innerProductionScale = INNER_PRODUCTION_SCALE;
    const double turnsBeforeLeaving = TURNS_BEFORE_LEAVING;
    const double acceptedStrengthLoss = ACCEPTED_STRENGTH_LOSS;

    while (true) {
        getFrame(presentMap);

        std::set<hlt::Move> moves;
        std::vector<std::vector<unsigned char>> proposedMoves(presentMap.width, std::vector<unsigned char>(presentMap.height, UCHAR_MAX));
        std::vector<std::vector<int>> forecastStrength(presentMap.width, std::vector<int>(presentMap.height, 0));
        std::priority_queue<std::pair<double, hlt::Location>> locationsQueue;

        for (unsigned short x = 0; x < presentMap.width; x++) {
            for (unsigned short y = 0; y < presentMap.height; y++) {
                auto currentSite = presentMap.getSite({x, y});
                if (currentSite.owner != myID && (presentMap.getSite({x, y}, NORTH).owner == myID || presentMap.getSite({x, y}, EAST).owner == myID || presentMap.getSite({x, y}, SOUTH).owner == myID || presentMap.getSite({x, y}, WEST).owner == myID)) {
                    locationsQueue.push({currentSite.production * outerProductionScale - currentSite.strength, {x, y}});
                }

                if (currentSite.owner == myID) {
                    forecastStrength[x][y] = currentSite.strength;
                }
            }
        }

        while (!locationsQueue.empty()) {
            auto [pointWeight, pointLocation] = locationsQueue.top();
            locationsQueue.pop();

            for (unsigned short dir = 1; dir <= 4; dir++) {
                auto neighborLocation = presentMap.getLocation(pointLocation, dir);
                auto neighborSite = presentMap.getSite(neighborLocation);

                if (neighborSite.owner == myID) {
                    if (proposedMoves[neighborLocation.x][neighborLocation.y] == UCHAR_MAX) {
                        locationsQueue.push({pointWeight - neighborSite.production * innerProductionScale, neighborLocation});
                    }

                    if (proposedMoves[neighborLocation.x][neighborLocation.y] != UCHAR_MAX && proposedMoves[neighborLocation.x][neighborLocation.y] != STILL) {
                        continue;
                    }

                    if ((int)forecastStrength[pointLocation.x][pointLocation.y] + (int)neighborSite.strength < UCHAR_MAX + acceptedStrengthLoss) {
                        proposedMoves[neighborLocation.x][neighborLocation.y] = reverseDirection(dir);

                        forecastStrength[pointLocation.x][pointLocation.y] += neighborSite.strength;
                        forecastStrength[neighborLocation.x][neighborLocation.y] -= neighborSite.strength;
                    } else {
                        proposedMoves[neighborLocation.x][neighborLocation.y] = STILL;
                    }
                }
            }
        }

        for (unsigned short x = 0; x < presentMap.width; x++) {
            for (unsigned short y = 0; y < presentMap.height; y++) {
                auto currentSite = presentMap.getSite({x, y});
                if (currentSite.owner == myID) {
                    if (currentSite.strength < currentSite.production * turnsBeforeLeaving) {
                        proposedMoves[x][y] = STILL;
                    }

                    for (unsigned short dir = 1; dir <= 4; dir++) {
                        auto neighborSite = presentMap.getSite({x, y}, dir);
                        if (neighborSite.owner != myID) {
                            if (neighborSite.strength < currentSite.strength) {
                                proposedMoves[x][y] = dir;
                                break;
                            } else if (dir == proposedMoves[x][y]) {
                                proposedMoves[x][y] = STILL;
                            }
                        }
                    }

                    moves.insert({{x, y}, proposedMoves[x][y]});
                }
            }
        }

        sendFrame(moves);
    }

    return 0;
}
