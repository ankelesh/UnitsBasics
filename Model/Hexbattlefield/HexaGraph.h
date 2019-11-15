#pragma once
#include <set>
#include <map>
#include <Model/CubeCoords.h>
#include <vector>
#include <sstream>
#include <queue>
#include <memory>
#include <limits>
#include <locale>


using namespace HexCoords;

struct WeightedCoord
{
	ICube coord;
	int weight;
};
struct UniformPathfindResult
{
	bool success;
	std::vector<ICube> path;
	std::map<ICube, ICube> came_from;
	std::map<ICube, int> cost_so_far;
};
typedef std::unique_ptr<UniformPathfindResult> UniPFResult;
static std::wstring showResults(UniPFResult & res)
{
	std::wostringstream sout;
	sout << ((res->success) ? L"Success" : L"Fail") <<
		L" Path contains " << res->path.size();
	return sout.str();
}
static bool operator<(const WeightedCoord & f, const WeightedCoord & s) { return f.weight < s.weight; };
static bool operator>(const WeightedCoord & f, const WeightedCoord & s) { return f.weight > s.weight; };
static std::string serialize(WeightedCoord & wcoord)
{
		std::ostringstream sout;
		sout << (wcoord.coord.toStr()) << wcoord.weight << '\n';
		return sout.str();
}
static WeightedCoord deserialize(std::string & str)
	{
		std::istringstream sin(str);
		int x, z, y, w;
		sin >> x >> z >> y >> w;
		return WeightedCoord({ ICube(x,z,y), w });
	}
class HexaGraph
{
private:
	std::map<ICube, int> * WeightedCoords;
	std::set<ICube> heights;
	std::map<ICube, std::set<ICube>> edges;
public:
	HexaGraph(std::map<ICube, int> * wghtmap);
	std::string getDescription() const;
	bool insertHeight(WeightedCoord wc);
	bool removeHeight(const ICube key);
	std::set<ICube> neighbours(const ICube &) const;
	std::vector<ICube> BreadthFirstSearch(const ICube & start, const ICube & finish) const;
	std::vector<ICube> DijkstraSearch(const ICube & start, const ICube & finish, const int max_cost=0);
	UniPFResult UniformBreadthFirst(const ICube & start, const ICube & finish) const;
	UniPFResult UniformDijkstra(const ICube & start, const ICube & finish,
		const int max_cost =std::numeric_limits<int>().max()) const;
	UniPFResult UniformBestFirst(const ICube & start, const ICube & finish) const;
	UniPFResult UniformAStar(const ICube & start, const ICube & finish,
		const int max_cost = std::numeric_limits<int>().max());
};


std::string testGraph()
{
	std::map<ICube, int> coordlist;
	coordlist.emplace(ICube(0, 0, 0), 1);
	coordlist.emplace(ICube(-1, 0, 1), 1);
	coordlist.emplace(ICube(1, 0, -1), 1);
	coordlist.emplace(ICube(0, 1, -1), 1);
	coordlist.emplace(ICube(0, -1, 1), 1);
	coordlist.emplace(ICube(1, -1, 0), 1);
	coordlist.emplace(ICube(-1, 1, 0), 1);
	HexaGraph hg(&coordlist);
	return hg.getDescription();
}
std::string  testGraph(std::vector<ICube> & clist)
{
	auto coord = clist.begin();
	std::map<ICube, int> temp;
	while (coord != clist.end())
	{
		temp.emplace(*coord, 1);
		++coord;
	}
	HexaGraph hg(&temp);
	return hg.getDescription();
}
std::string testBreadth(std::vector<ICube> & clist, ICube start, ICube finish)
{
	auto coord = clist.begin();
	std::map<ICube, int> temp;
	while (coord != clist.end())
	{
		temp.emplace(*coord, 1);
		++coord;
	}
	HexaGraph hg(&temp);
	std::ostringstream sout;
	sout << "\nPath from " << start.toStr() << " to " << finish.toStr() << '\n';
	for each(ICube coor in hg.BreadthFirstSearch(start, finish))
	{
		sout << coor.toStr() << "\n";
	}
	return sout.str();
}
std::string testDijkstra(std::vector<ICube> & clist, ICube start, ICube finish)
{
	auto coord = clist.begin();
	std::map<ICube, int> temp;
	while (coord != clist.end())
	{
		temp.emplace(*coord, 1);
		++coord;
	}
	HexaGraph hg(&temp);
	std::ostringstream sout;
	sout << "\nPath from " << start.toStr() << " to " << finish.toStr() << '\n';
	for each(ICube coor in hg.DijkstraSearch(start, finish, 3))
	{
		sout << coor.toStr() << "\n";
	}
	return sout.str();
}
