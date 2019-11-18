#include "HexaGraph.h"


HexaGraph::HexaGraph(std::map<ICube, int> * wghtmap) : WeightedCoords(wghtmap)
{
	auto coord = wghtmap->begin();
	while (coord != wghtmap->end())
	{
		std::set<ICube> neighb;
		//debug << "\Getting neighbors for " << coord.key().toStr();
		for (int i = 0; i < 6; ++i)
		{
			ICube neigCoord = coord->first.neighbor((HexCoords::CubeDirections)i);
			//debug <<"\n\t"<< neigCoord.toStr();
			if (wghtmap->find(neigCoord)!= wghtmap->end())
			{
				neighb.emplace(neigCoord);
			}
		}
		heights.emplace(coord->first);
		edges.emplace(coord->first, neighb);
		++coord;
	}
}
std::string HexaGraph::getDescription() const
{
	std::ostringstream sout;
	sout << "\nGraph contains " << heights.size() << " heights";
	auto hght = heights.begin();
	while (hght != heights.end())
	{
		sout << "\nHeight " << hght->toStr();
		auto edgeset = edges.at(*hght).begin();
		auto edgesetend = edges.at(*hght).end();
		while (edgeset != edgesetend)
		{
			sout << "\n\tHas edge: (" << hght->toStr() << ") -> ("
				<< edgeset->toStr() << ") and weight: " << (*WeightedCoords)[*hght];
			++edgeset;
		}
		++hght;
	}
	sout << "\n<Graph end>\n";
	return sout.str();
}
std::set<ICube> HexaGraph::neighbours(const ICube & coord) const
{
	if (heights.find(coord)!= heights.end())
	{
		return edges.at(coord);
	}
	return std::set<ICube>();
}
std::vector<ICube> HexaGraph::BreadthFirstSearch(const ICube & start, const ICube & finish) const
{
	if ((heights.find(start) == heights.end())  && (heights.find(finish) == heights.end()))
	{
		return std::vector<ICube>();
	}
	std::queue<ICube> frontier;
	frontier.push(start);
	std::map<ICube, ICube> came_from;
	came_from.emplace(start, ICube(-300,150,150));
	ICube current;
	while (!frontier.empty())
	{
		current = frontier.front();
		frontier.pop();

		if (current == finish)
			break;
		for each(ICube next in neighbours(current))
		{
			if (came_from.find(next) == came_from.end())
			{
				frontier.push(next);
				came_from[next] = current;
			}
		}
	}
	current = finish;
	std::vector<ICube> path;
	while (!(current == start))
	{
		path.push_back(current);
		current = came_from[current];
	}
	path.push_back(start);
	std::reverse(path.begin(), path.end());
	return path;
}
std::vector<ICube> HexaGraph::DijkstraSearch(const ICube & start, const ICube & finish, const int max_cost)
{
	std::priority_queue<WeightedCoord, std::vector<WeightedCoord>, std::greater<WeightedCoord>> frontier;
	frontier.push(WeightedCoord({ start, 0 }));
	std::map<ICube, ICube> came_from;
	std::map<ICube, int> cost_so_far;
	came_from.emplace(start, ICube(-150, -150, 300));
	cost_so_far[start] = 0;
	WeightedCoord current;
	//debug << "Dijkstra started. Stats: S " << start.toStr() << " F " << finish.toStr();
	while (!frontier.empty())
	{
		current = frontier.top();
		frontier.pop();
		//debug << "\nCurrent changed: " << current.coord.toStr() << " w " << current.weight;
		if (current.coord == finish)
			break;
		for each(ICube next in neighbours(current.coord))
		{
			//debug << "\nNext changed: " << next.toStr();
			int new_cost = cost_so_far[current.coord] + (*WeightedCoords)[current.coord];
			//debug << " new cost " << new_cost;
			if ((cost_so_far.find(next) == cost_so_far.end()))
			{
				cost_so_far[next] =  new_cost;
				int priority = new_cost;
				frontier.push(WeightedCoord({ next, priority }));
				came_from.emplace(next, current.coord);
			}
			else if (new_cost < cost_so_far[next])
			{
				cost_so_far[next] = new_cost;
				int priority = new_cost;
				frontier.push(WeightedCoord({ next, priority }));
				came_from.emplace(next, current.coord);
			}
		}
	}
	ICube curr = finish;
	//debug << "\nFin : " << finish.toStr() << " St: " << start.toStr() << " Curr: " << curr.toStr();
	std::vector<ICube> path;
	while (!(curr == start))
	{
		path.push_back(curr);
		//debug << curr.toStr();
		curr = came_from[curr];
	}
	path.push_back(start);
	std::reverse(path.begin(), path.end());
	return path;
}
bool HexaGraph::insertHeight(WeightedCoord  wc)
{
	if (WeightedCoords->find(wc.coord) == WeightedCoords->end())
	{
		WeightedCoords->emplace(wc.coord, wc.weight);
		heights.emplace(wc.coord);
		std::set<ICube> temp;
		for (int i = 0; i < 6; ++i)
		{
			ICube neigCoord = wc.coord.neighbor((HexCoords::CubeDirections)i);
			if (WeightedCoords->find(neigCoord) != WeightedCoords->end())
			{
				temp.emplace(neigCoord);
				edges[neigCoord].emplace(wc.coord);
			}
		}
		edges.emplace(wc.coord, temp);
		return true;
	}
	return false;
}
bool HexaGraph::removeHeight(const ICube  key)
{
	if (WeightedCoords->find(key) != WeightedCoords->end())
	{
		WeightedCoords->erase(key);
		auto neighbours = edges[key];
		auto begin = neighbours.begin();
		while (begin != neighbours.end())
		{
			edges[*begin].erase(key);
			++begin;
		}
		heights.erase(key);
		edges.erase(key);
		return true;
	}
	return false;
}
std::unique_ptr<UniformPathfindResult> HexaGraph::UniformBreadthFirst(const ICube & start, const ICube & finish) const
{
	std::unique_ptr<UniformPathfindResult> uniresult(new UniformPathfindResult);
	uniresult->success = true;
	if (!(heights.find(start) == heights.end()) && !(heights.find(finish) == heights.end()))
	{
		uniresult->success = false;
		return uniresult;
	}
	std::queue<ICube> frontier;
	frontier.emplace(start);
	uniresult->came_from.emplace(start, ICube(-300, 150, 150));
	ICube current;
	while (!frontier.empty())
	{
		current = frontier.front();
		frontier.pop();
		if (current == finish)
			break;
		for each(ICube next in neighbours(current))
		{
			if (uniresult->came_from.find(next) == uniresult->came_from.end())
			{
				frontier.emplace(next);
				uniresult->came_from.emplace(next, current);
			}
		}
	}
	current = finish;
	while (!(current == start))
	{
		uniresult->path.push_back(current);
		if (uniresult->came_from.find(current)!= uniresult->came_from.end())
			current = uniresult->came_from[current];
		else
		{
			uniresult->success = false;
			return uniresult;
		}
	}
	uniresult->path.push_back(start);
	std::reverse(uniresult->path.begin(), uniresult->path.end());
	return uniresult;
}
std::unique_ptr<UniformPathfindResult>  HexaGraph::UniformDijkstra(const ICube & start, const ICube & finish,
	const int max_cost) const
{
	std::unique_ptr<UniformPathfindResult> uniresult(new UniformPathfindResult);
	uniresult->success=true;
	if ((heights.find(start) == heights.end() && heights.find(finish) == heights.end()))
	{
		uniresult->success = false;
		return uniresult;
	}
	std::priority_queue<WeightedCoord, std::vector<WeightedCoord>,
		std::greater<WeightedCoord>> frontier;
	frontier.push(WeightedCoord({ start, 0 }));
	uniresult->came_from.emplace(start, ICube(-150, -150, 300));
	uniresult->cost_so_far.emplace(start, 0);
	WeightedCoord current;
	//debug << "Dijkstra started. Stats: S " << start.toStr() << " F " << finish.toStr();
	while (!frontier.empty())
	{
		current = frontier.top();
		frontier.pop();
		//debug << "\nCurrent changed: " << current.coord.toStr() << " w " << current.weight;
		if (current.coord == finish)
			break;
		for each(ICube next in neighbours(current.coord))
		{
			//debug << "\nNext changed: " << next.toStr();
			int new_cost = uniresult->cost_so_far[current.coord] +
				(*WeightedCoords)[next];
			//debug << " new cost " << new_cost;
			if (((uniresult->cost_so_far.find(next) == uniresult->cost_so_far.end()) || 
				(new_cost < uniresult->cost_so_far[next])) && new_cost < max_cost)
			{
				uniresult->cost_so_far.emplace(next, new_cost);
				int priority = new_cost;
				frontier.push(WeightedCoord({ next, priority }));
				uniresult->came_from.emplace(next, current.coord);
			}
		}
	}
	ICube curr = finish;
	//debug << "\nFin : " << finish.toStr() << " St: " << start.toStr() << " Curr: " << curr.toStr();
	while (!(curr == start))
	{
		uniresult->path.push_back(curr);
		//debug << curr.toStr();
		if (uniresult->came_from.find(curr)!= uniresult->came_from.end())
			curr = uniresult->came_from[curr];
		else
		{
			uniresult->success = false;
			return uniresult;
		}
	}
	uniresult->path.push_back(start);
	std::reverse(uniresult->path.begin(), uniresult->path.end());
	return uniresult;
}
UniPFResult HexaGraph::UniformBestFirst(const ICube & start, const ICube & finish) const
{
	std::priority_queue<WeightedCoord, std::vector<WeightedCoord>,
		std::greater<WeightedCoord>> frontier;
	frontier.push(WeightedCoord({ start, 0 }));
	UniPFResult uniresult(new UniformPathfindResult);
	uniresult->success = true;
	if (!(heights.find(start) != heights.end() && heights.find (finish)!= heights.end()))
	{
		uniresult->success = false;
		return uniresult;
	}
	uniresult->came_from[start] = ICube(-300, 150, 150);
	while (!frontier.empty())
	{
		WeightedCoord current = frontier.top();
		frontier.pop();
		if (current.coord == finish)
		{
			break;
		}
		for each(ICube next in neighbours(current.coord))
		{
			if ((uniresult->came_from.find(next) == uniresult->came_from.end()))
			{
				int priority = finish.distanceTo(next);
				frontier.push(WeightedCoord({ next, priority }));
				uniresult->came_from.emplace(next, current.coord);
			}
		}
	}
	auto current = finish;
	while (!(current == start))
	{
		uniresult->path.push_back(current);
		if (uniresult->came_from.find(current)!= uniresult->came_from.end())
			current = uniresult->came_from[current];
		else
		{
			uniresult->success = false;
			return uniresult;
		}
	}
	uniresult->path.push_back(start);
	std::reverse(uniresult->path.begin(), uniresult->path.end());
	return uniresult;
}
UniPFResult HexaGraph::UniformAStar(const ICube & start, const ICube & finish,
	const int max_cost)
{
	std::unique_ptr<UniformPathfindResult> uniresult(new UniformPathfindResult);
	uniresult->success = true;
	if (!(heights.find(start) != heights.end() && heights.find(finish) != heights.end()))
	{
		uniresult->success = false;
		return uniresult;
	}
	std::priority_queue<WeightedCoord, std::vector<WeightedCoord>,
		std::greater<WeightedCoord>> frontier;
	frontier.push(WeightedCoord({ start, 0 }));
	uniresult->came_from.emplace(start, ICube(-150, -150, 300));
	uniresult->cost_so_far.emplace(start, 0);
	WeightedCoord current;
	//debug << "Dijkstra started. Stats: S " << start.toStr() << " F " << finish.toStr();
	while (!frontier.empty())
	{
		current = frontier.top();
		frontier.pop();
		//debug << "\nCurrent changed: " << current.coord.toStr() << " w " << current.weight;
		if (current.coord == finish)
			break;
		for each(ICube next in neighbours(current.coord))
		{
			//debug << "\nNext changed: " << next.toStr();
			int new_cost = uniresult->cost_so_far[current.coord] +
				(*WeightedCoords)[next];
			//debug << " new cost " << new_cost;
			if (((uniresult->cost_so_far.find(next) == uniresult->cost_so_far.end()) ||
				(new_cost < uniresult->cost_so_far[next])) && new_cost < max_cost)
			{
				uniresult->cost_so_far.emplace(next, new_cost);
				int priority = new_cost + finish.distanceTo(next)*3;
				frontier.push(WeightedCoord({ next, priority }));
				uniresult->came_from.emplace(next, current.coord);
			}
		}
	}
	ICube curr = finish;
	//debug << "\nFin : " << finish.toStr() << " St: " << start.toStr() << " Curr: " << curr.toStr();
	while (!(curr == start))
	{
		uniresult->path.push_back(curr);
		//debug << curr.toStr();
		if (uniresult->came_from.find(curr) != uniresult->came_from.end())
			curr = uniresult->came_from[curr];
		else
		{
			uniresult->success = false;
			return uniresult;
		}
	}
	uniresult->path.push_back(start);
	std::reverse(uniresult->path.begin(), uniresult->path.end());
	return uniresult;
}

std::string testGraph()
{
	std::map<ICube, int> coordlist;
	std::vector<ICube> clist;
	coordlist.emplace(ICube(1, 1, -2), 1);
	coordlist.emplace(ICube(1, 0, -1), 1);
	coordlist.emplace(ICube(0, 1, -1), 1);
	
	coordlist.emplace(ICube(2, 0, -2), 1);
	coordlist.emplace(ICube(2, 1, -3), 1);

	coordlist.emplace(ICube(0, 2, -2), 1);
	coordlist.emplace(ICube(1, 2, -3), 1);
	auto b = coordlist.begin();
	while (b != coordlist.end())
	{
		clist.push_back(b++->first);
	}
	HexaGraph hg(&coordlist);
	return hg.getDescription() + testBreadth(clist, ICube(1,0,-1), ICube(1,2,-3)) + testDijkstra(clist, ICube(1, 0, -1), ICube(1, 2, -3));
}

std::string testGraph(std::vector<ICube>& clist)
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

std::string testBreadth(std::vector<ICube>& clist, ICube start, ICube finish)
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
	for each (ICube coor in hg.BreadthFirstSearch(start, finish))
	{
		sout << coor.toStr() << "\n";
	}
	return sout.str();
}

std::string testDijkstra(std::vector<ICube>& clist, ICube start, ICube finish)
{
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
		for each (ICube coor in hg.DijkstraSearch(start, finish, 3))
		{
			sout << coor.toStr() << "\n";
		}
		return sout.str();
	}
}
