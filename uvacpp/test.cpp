#include <iostream>

#include "generater.hpp"
#include "planner.hpp"
using namespace std;
int main() {
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
