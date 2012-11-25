#include "CS3 Framework\gtest.h"
#include "Graph.hpp"

TEST(Graph, 10Nodes)
{
  Graph* pGraph = new Graph("Graphs/graph10.txt");
  
  EXPECT_EQ(16, pGraph->shortestPath(1));
  EXPECT_EQ(29, pGraph->shortestPath(2));
  EXPECT_EQ(14, pGraph->shortestPath(3));
  EXPECT_EQ(10, pGraph->shortestPath(4));
  EXPECT_EQ(8, pGraph->shortestPath(5));
  
  delete pGraph;
}

// Remove DISABLED_ when you are ready for this test to run
// !!This test must pass!!
TEST(Graph, DISABLED_100Nodes)
{
  Graph* pGraph = new Graph("Graphs/graph100.txt");
  
  EXPECT_EQ(51, pGraph->shortestPath(1));
  EXPECT_EQ(25, pGraph->shortestPath(2));
  EXPECT_EQ(57, pGraph->shortestPath(3));
  EXPECT_EQ(59, pGraph->shortestPath(4));
  EXPECT_EQ(61, pGraph->shortestPath(5));

  delete pGraph;
}

// Remove DISABLED_ when you are ready for this test to run
// !!This test must pass!!
TEST(Graph, DISABLED_1000Nodes)
{
  Graph* pGraph = new Graph("Graphs/graph1000.txt");
  
  EXPECT_EQ(35, pGraph->shortestPath(1));
  EXPECT_EQ(42, pGraph->shortestPath(2));
  EXPECT_EQ(47, pGraph->shortestPath(3));
  EXPECT_EQ(55, pGraph->shortestPath(4));
  EXPECT_EQ(49, pGraph->shortestPath(5));

  delete pGraph;
}

// Remove DISABLED_ when you are ready for this test to run
// !!This test doesn't have to run!!
TEST(Graph, DISABLED_10000Nodes)
{
  Graph* pGraph = new Graph("Graphs/graph10000.txt");
  
  EXPECT_EQ(87, pGraph->shortestPath(1));
  EXPECT_EQ(133, pGraph->shortestPath(2));
  EXPECT_EQ(85, pGraph->shortestPath(3));
  EXPECT_EQ(36, pGraph->shortestPath(4));
  EXPECT_EQ(81, pGraph->shortestPath(5));

  delete pGraph;
}

// Remove DISABLED_ when you are ready for this test to run
// !!This test doesn't have to run!!
//  5 points extra credit if this test runs in less than 5 seconds
// 10 points extra credit if this test runs in less than 3 seconds
TEST(Graph, DISABLED_100000Nodes)
{
  Graph* pGraph = new Graph("Graphs/graph100000.txt");
  
  EXPECT_EQ(82, pGraph->shortestPath(1));
  EXPECT_EQ(124, pGraph->shortestPath(2));
  EXPECT_EQ(121, pGraph->shortestPath(3));
  EXPECT_EQ(96, pGraph->shortestPath(4));
  EXPECT_EQ(95, pGraph->shortestPath(5));

  delete pGraph;
}