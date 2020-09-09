#include <iostream>
#include "TransformUtils.hpp"
#include "IOutils.hpp"
#include "generater.hpp"
#include "planner.hpp"
using namespace std;

int main() {
  ProjectionDataset Map;
  cout << readGeoRaster("dem.tif", Map) << endl;
  cout << Map.step << endl;
  cout << Map.adfLRCoord[0] << " " << Map.adfLRCoord[1] << endl;
  for (int i = 0; i < 1; i++) {
    for (int j = 0; j < 1; j++) {
      GeoPoint p = Map.RasterData[i][j];
      cout << setprecision(15) << p.Xgeo << " " << p.Ygeo << " " << p.elevation
           << endl;
    }
  }
  cout << readGeoRaster("DH-DSM-84BL.TIF", Map) << endl;
  cout << Map.step << endl;
  cout << Map.adfLRCoord[0] << " " << Map.adfLRCoord[1] << endl;
  for (int i = 0; i < 1; i++) {
    for (int j = 0; j < 1; j++) {
      GeoPoint p = Map.RasterData[i][j];
      cout << setprecision(15) << p.Xgeo << " " << p.Ygeo << " " << p.elevation
           << endl;
    }
  }
  double x, y;
  Polygon_2 pgn;
  while (cin >> x >> y && x && y) {
    pgn.push_back(Point_2(x, y));
  }
  // Point_2 points[] = {Point_2(0, 8), Point_2(0, 0), Point_2(8, 0),
  //                     Point_2(8, 8), Point_2(5, 5)};
  // cout << pointInPolygon(pgn, Point_2(0,0))<<endl;
  InputNode in(pgn, 1, 0.5, *pgn.vertices_begin(), *pgn.vertices_begin());
  PolygonNode node(pgn);
  TaskNode output = CoveragePathPlan(in);
  for (auto task : output.TaskSequence) {
    for (auto point : task.waypoints) {
      cout << point.x() << ", " << point.y() << endl;
    }
  }
  cout << output.pathlength << endl;
  return 0;
}
