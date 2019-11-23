// GridBasedRouter.h
#ifndef PCBROUTER_GRID_BASED_ROUTER_H
#define PCBROUTER_GRID_BASED_ROUTER_H

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "BoardGrid.h"
#include "globalParam.h"
#include "kicadPcbDataBase.h"
#include "util.h"

class GridBasedRouter {
   public:
    // ctor
    GridBasedRouter(kicadPcbDataBase &db) : mDb(db) {}
    // dtor
    ~GridBasedRouter() {}

    void test_router();
    void testRouterWithPinAndKeepoutAvoidance();
    void testRouterWithAvoidanceAndVariousPadType();
    void testRouterWithRipUpAndReroute();
    void testRouterWithPinShape();
    bool outputResults2KiCadFile(std::vector<MultipinRoute> &nets, bool mergeSegments = false, std::string fileNameStamp = "");

   private:
    bool writeNets(std::vector<MultipinRoute> &multipinNets, std::ofstream &ofs);
    bool writeNetsFromGridPaths(std::vector<MultipinRoute> &multipinNets, std::ofstream &ofs);
    void writeSolutionBackToDbAndSaveOutput(std::vector<MultipinRoute> &multipinNets);

    // Helpers
    void setupBoardAndMappingStructure();
    void setupGridNetsAndGridPins();
    void addAllPinCostToGrid(const int);
    // void addAllPinInflationCostToGrid(const int);
    void addPinAvoidingCostToGrid(const Pin &, const float, const bool, const bool, const bool, const int inflate = 0);
    void addPinAvoidingCostToGrid(const padstack &, const instance &, const float, const bool, const bool, const bool, const int inflate = 0);
    void addPinAvoidingCostToGrid(const GridPin &gridPin, const float value, const bool toViaCost, const bool toViaForbidden, const bool toBaseCost, const int inflate = 0);
    // PadShape version
    void addPinShapeAvoidingCostToGrid(const GridPin &gridPin, const float value, const bool toViaCost, const bool toViaForbidden, const bool toBaseCost);

    // Rasterize circle
    void getRasterizedCircle(const int radius, const double radiusFloating, std::vector<Point_2D<int>> &grids);

    // Pin Layers on Grid
    bool getGridLayers(const Pin &, std::vector<int> &layers);
    bool getGridLayers(const padstack &, const instance &, std::vector<int> &layers);

    int getNextRipUpNetId();

    // Utilities
    int dbLengthToGridLengthCeil(const double dbLength) {
        return (int)ceil(dbLength * GlobalParam::inputScale);
    }
    int dbLengthToGridLengthFloor(const double dbLength) {
        return (int)floor(dbLength * GlobalParam::inputScale);
    }
    double dbLengthToGridLength(const double dbLength) {
        return dbLength * GlobalParam::inputScale;
    }

    bool dbPointToGridPoint(const point_2d &dbPt, point_2d &gridPt);
    bool dbPointToGridPointCeil(const Point_2D<double> &dbPt, Point_2D<int> &gridPt);
    bool dbPointToGridPointFloor(const Point_2D<double> &dbPt, Point_2D<int> &gridPt);
    bool dbPointToGridPointRound(const Point_2D<double> &dbPt, Point_2D<int> &gridPt);
    bool gridPointToDbPoint(const point_2d &gridPt, point_2d &dbPt);

   private:
    BoardGrid mBg;
    kicadPcbDataBase &mDb;

    // Layer mapping
    std::vector<std::string> mGridLayerToName;
    std::unordered_map<std::string, int> mLayerNameToGrid;
    std::unordered_map<int, int> mDbLayerIdToGridLayer;

    // Put below in the BoardGrid?
    // Global GridPins including the pins aren't connected by nets
    std::vector<GridPin> mGridPins;
    // TODO:: Improve the below......
    std::vector<MultipinRoute> gridNets;
    std::vector<MultipinRoute> bestSolution;

    // Board Boundary
    double mMinX = std::numeric_limits<double>::max();
    double mMaxX = std::numeric_limits<double>::min();
    double mMinY = std::numeric_limits<double>::max();
    double mMaxY = std::numeric_limits<double>::min();
};

#endif