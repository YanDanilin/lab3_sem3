#ifndef GRAPH_H_LAB3_SEM3
#define GRAPH_H_LAB3_SEM3

#include <limits>
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <unordered_map>
#include "Pair.h"
#include "sup.h"

template <class TV, class TE>
class Graph {
    friend class MainWindow;
protected:
	struct Vertex {
		TV value = TV();
		size_t id = 0;
		Vertex() {}
		Vertex(size_t _id, const TV& _value) {
			value = _value;
			id = _id;
		}
		Vertex(const Vertex& another) {
			value = another.value;
			id = another.id;
		}
		//Vertex(Vertex&& another) {
		//	value = another.value;
		//	id = another.id;
		//	another.value = TV();
		//	another.id = 0;
		//}
		Vertex& operator=(const Vertex& another) {
			value = another.value;
			id = another.id;
			return *this;
		}
		~Vertex() = default;
		bool operator==(const Vertex& v) {
			if (id == v.id && value == v.value) {
				return true;
			}
			return false;
		}
		bool operator!=(const Vertex& v) {
			return !(*this == v);
		}
	};
	struct Edge {
		TE value = TE();
		size_t id_from = 0;
		size_t id_to = 0;
		Edge() {}
		Edge(size_t from, size_t to, const TE& _value) {
			value = _value;
			id_from = from;
			id_to = to;
		}
		Edge(const Edge& another) {
			value = another.value;
			id_from = another.id_from;
			id_to = another.id_to;
		}
		Edge& operator=(const Edge& another) {
			value = another.value;
			id_from = another.id_from;
			id_to = another.id_to;
			return *this;
		}
		~Edge() = default;
		bool operator==(const Edge& e) {
			if (value == e.value && id_from == e.id_from && id_to == e.id_to) {
				return true;
			}
			return false;
		}
		bool operator!=(const Edge& e) {
			return !(*this == e);
		}
	};
	struct VertexHash {
		size_t operator()(const Vertex& vertex) const {
			std::hash<size_t> hasher;
			return hasher(vertex.id);
		}
	};
	struct VertexEqual {
		bool operator()(const Vertex& v1, const Vertex& v2) const {
            if (v1.id == v2.id) {
                return true;
            }
			return false;
		}
	};

	std::unordered_map<size_t, std::vector<Edge>> incidence_list;
	std::unordered_map<size_t, Vertex> vertexes;
	size_t vertex_count = 0;
	size_t edge_count = 0;
	bool weighed = false;
public:
	class Path {
	private:
		std::list<Edge> seq;
	public:
		Path() = default;
		Path(const Path& another) {
			seq = another.seq;
		}
		~Path() = default;
		Path& operator=(const Path& another) {
			seq = another.seq;
			return *this;
		}
		void push_front(const Edge& edge) {
			seq.push_front(edge);
		}
		void push_front(size_t from, size_t to, TE _value = TE()) {
			Edge edge(from, to, _value);
			seq.push_front(edge);
		}	
		void push_back(const Edge& edge) {
			seq.push_back(edge);
		}
		void push_back(size_t from, size_t to, TE _value = TE()) {
			Edge edge(from, to, _value);
			seq.push_back(edge);
		}
		void erase(const Edge& edge) {
			for (auto it = seq.begin(); it != seq.end(); ) {
				if ((*it) == edge) {
					it = seq.erase(it);
					break;
				}
				else {
					it++;
				}
			}
		}
        void clear() {
            seq.clear();
        }
        Edge operator[](size_t index) {
            size_t i = 0;
            auto it = seq.cbegin();
            while (i < index) {
                it++;
                i++;
            }
            return *it;
        }
        bool empty() {
            if (seq.size() == 0) {
                return true;
            }
            return false;
        }
        size_t size() {
            return seq.size();
        }
		bool operator==(const Path& path) {
			if (seq == path.seq) {
				return true;
			}
			return false;
		}
		bool operator!=(const Path& path) {
            return !(*this == path);
		}

		
	};
	Graph() = default;
	~Graph() = default;
	void add_vertex(size_t _id, TV _value = TV());
	void add_edge(size_t from, size_t to, TE _value = TE());
	TV& vertex_weight(size_t _id);

	void erase_vertex(size_t _id);

    void clear();

	size_t get_vertex_count() const;
	size_t get_edge_count() const;
	bool is_weighed() const;

};

template <class TV, class TE>
void Graph<TV, TE>::add_vertex(size_t _id, TV _value) {
	if (incidence_list.empty()) {
		incidence_list.reserve(10);
		vertexes.reserve(10);
	}
	if (_id <= 0) {
        //throw std::exception("Inappropriate id");
        return;
	}
	if (incidence_list.find(_id) != incidence_list.end()) {
        //throw std::exception("Such vertex exists");
        return;
	}
	Vertex vertex(_id, _value);
	incidence_list[_id] = std::vector<Edge>();
	vertexes[_id] = vertex;
	vertex_count++;
}
template <class TV, class TE>
void Graph<TV, TE>::add_edge(size_t from, size_t to, TE _value) {
	if (incidence_list.empty()) {
		incidence_list.reserve(10);
		vertexes.reserve(10);
	}
	Vertex vf(from, TV());
	Vertex vt(to, TV());
	if (from <= 0) {
        //throw std::exception("Inappropriate id");
	}
	if (to <= 0) {
        //throw std::exception("Inappropriate id");
	}
	if (vertexes.find(from) == vertexes.end()) {
		vertexes[from] = vf;
		vertex_count++;
 	}
	if (vertexes.find(to) == vertexes.end()) {
		vertexes[to] = vt;
		vertex_count++;
	}
	incidence_list[from].push_back(Edge(from, to, _value));
	if (from != to) {
		incidence_list[to].push_back(Edge(from, to, _value));
	}
	if (_value != TE() && !weighed) {
		weighed = true;
	}
	edge_count++;
}
template <class TV, class TE>
TV& Graph<TV, TE>::vertex_weight(size_t _id) {
	if (_id <= 0) {
        //throw std::exception("Inappropriate id");
	}
	Vertex v(_id, TV());
	if (vertexes.find(_id) == vertexes.end()) {
		vertex_count++;
		incidence_list[_id] = std::vector<Edge>();
		vertexes[_id] = v;
	}
	return vertexes[_id].value;
}
template <class TV, class TE>
void Graph<TV, TE>::erase_vertex(size_t _id) {
	if (vertexes.find(_id) == vertexes.end()) {
		return;
	}
	edge_count -= incidence_list[_id].size();
	vertex_count--;
	incidence_list.erase(incidence_list.find(_id));
	vertexes.erase(vertexes.find(_id));
	for (auto elem = incidence_list.begin(); elem != incidence_list.end(); elem++) {
		for (auto itv = elem->second.begin(); itv != elem->second.end(); ) {
			if (itv->id_from == _id || itv->id_to == _id) {
				itv = elem->second.erase(itv);
			}
			else {
				itv++;
			}
		}
	}
}

template <class TV, class TE>
void Graph<TV, TE>::clear() {
    for (auto it = incidence_list.begin(); it != incidence_list.end(); it++) {
        it->second.clear();
    }
    incidence_list.clear();
    vertexes.clear();
    vertex_count = 0;
    edge_count = 0;
    weighed = false;
}
template <class TV, class TE>
size_t Graph<TV, TE>::get_vertex_count() const {
	return vertex_count;
}
template <class TV, class TE>
size_t Graph<TV, TE>::get_edge_count() const {
	return edge_count;
}
template <class TV, class TE>
bool Graph<TV, TE>::is_weighed() const {
	return weighed;
}

template <class TV, class TE>
class DirectedGraph : public Graph<TV, TE> {
private:
    using Path = typename Graph<TV, TE>::Path;
	Path bfs(size_t from, size_t to) {
		Path path;
		std::queue<size_t> q;
		std::unordered_map<size_t, bool> checked(Graph<TV, TE>::vertex_count);
		std::unordered_map<size_t, size_t> parents(Graph<TV, TE>::vertex_count);
		q.push(from);
		parents[from] = 0;
		checked[from] = true;
		while (!q.empty()) {
			size_t vert = q.front();
			if (vert == to) {
				break;
			}
			q.pop();
			for (auto it = Graph<TV, TE>::incidence_list[vert].begin(); it != Graph<TV, TE>::incidence_list[vert].end(); it++) {
				if (checked.find(it->id_to) == checked.end()) {
					checked[it->id_to] = true;
					q.push(it->id_to);
					parents[it->id_to] = vert;
				}
			}
		}
		if (checked.find(to) == checked.end()) {
			return path;
		}
		size_t child = to;
		size_t cur_parent = parents[child];
		while (cur_parent != 0) {
			for (auto it = Graph<TV, TE>::incidence_list[child].begin(); it != Graph<TV, TE>::incidence_list[child].end(); it++) {
				if (cur_parent == it->id_from) {
					path.push_front(*it);
					break;
				}
			}
			child = cur_parent;
			cur_parent = parents[cur_parent];
		}
		return path;
	}
	Path dijkstra(size_t from, size_t to) {
        Path res;
        sup<TE> inf;
		std::unordered_map<size_t, bool> checked;
		std::unordered_map<size_t, TE> dist;
		std::unordered_map<size_t, size_t> parents;
		std::unordered_map<Pair<size_t, size_t>, TE, pair_hash<size_t>, pair_ord_equal<size_t>> edges;
		checked.reserve(Graph<TV, TE>::vertex_count);
		dist.reserve(Graph<TV, TE>::vertex_count);
		parents.reserve(Graph<TV, TE>::vertex_count);
		edges.reserve(Graph<TV, TE>::edge_count);
		for (auto el = Graph<TV, TE>::vertexes.begin(); el != Graph<TV, TE>::vertexes.end(); el++) {
			checked[el->second.id] = false;
            dist[el->second.id] = inf();
			parents[el->second.id] = 0;
		}
		dist[from] = TE();
		parents[from] = from;
        for (size_t i = 0; i < Graph<TV, TE>::vertex_count; ++i) {
			size_t v = 0;
			for (auto it = Graph<TV, TE>::vertexes.cbegin(); it != Graph<TV, TE>::vertexes.cend(); it++) {
				if (!checked[it->second.id] && (v == 0 || dist[it->second.id] < dist[v])) {
					v = it->second.id;
				}
			}
            if (dist[v] == inf()) {
				break;
			}
			checked[v] = true;
			for (auto it = Graph<TV, TE>::incidence_list[v].cbegin(); it != Graph<TV, TE>::incidence_list[v].cend(); it++) {
				size_t v1 = it->id_to;
				if (v != v1) {
					TE len = it->value;
					if (dist[v] + len < dist[v1]) {
						dist[v1] = dist[v] + len;
						parents[v1] = v;
					}
					edges[Pair<size_t, size_t>{v, v1}] = len;
				}
			}
		}
        if (dist[to] == inf()) {
			return res;
		}
		size_t i = to;
		size_t p = parents[i];
		while (p != from) {
			res.push_front(p, i, edges[Pair<size_t, size_t>{p, i}]);
			i = p;
			p = parents[i];
		}
		res.push_front(p, i, edges[Pair<size_t, size_t>{p, i}]);
		return res;
	}
public:
    void erase_edge(size_t from, size_t to, TE _value = TE());
	Path min_path(size_t from, size_t to) {
		if (Graph<TV, TE>::weighed) {
			return dijkstra(from, to);
		}
		return bfs(from, to);
	}
};

template <class TV, class TE>
class UndirectedGraph : public Graph<TV, TE> {
private:
    using Path = typename Graph<TV, TE>::Path;
	Path bfs(size_t from, size_t to) {
		Path path;
		std::queue<size_t> q;
		std::unordered_map<size_t, bool> checked(Graph<TV, TE>::vertex_count);
		std::unordered_map<size_t, size_t> parents(Graph<TV, TE>::vertex_count);
		q.push(from);
		parents[from] = 0;
		checked[from] = true;
		while (!q.empty()) {
			size_t vert = q.front();
			if (vert == to) {
				break;
			}
			q.pop();
			for (auto it = Graph<TV, TE>::incidence_list[vert].begin(); it != Graph<TV, TE>::incidence_list[vert].end(); it++) {
				if (checked.find(it->id_from) == checked.end()) {
					checked[it->id_from] = true;
					q.push(it->id_from);
					parents[it->id_from] = vert;
				}
				else if (checked.find(it->id_to) == checked.end()) {
					checked[it->id_to] = true;
					q.push(it->id_to);
					parents[it->id_to] = vert;
				}
			}
		}
		if (checked.find(to) == checked.end()) {
			return path;
		}
		size_t child = to;
		size_t cur_parent = parents[child];
		while (cur_parent != 0) {
			for (auto it = Graph<TV, TE>::incidence_list[child].begin(); it != Graph<TV, TE>::incidence_list[child].end(); it++) {
				if (cur_parent == it->id_from || cur_parent == it->id_to) {
					path.push_front(*it);
					break;
				}
			}
			child = cur_parent;
			cur_parent = parents[cur_parent];
		}
		return path;
	}
	Path dijkstra(size_t from, size_t to) {
		Path res;
        sup<TE> inf;
		std::unordered_map<size_t, bool> checked;
		std::unordered_map<size_t, TE> dist;
		std::unordered_map<size_t, size_t> parents;
		std::unordered_map<Pair<size_t, size_t>, TE, pair_hash<size_t>, pair_unord_equal<size_t>> edges;
		checked.reserve(Graph<TV, TE>::vertex_count);
		dist.reserve(Graph<TV, TE>::vertex_count);
		parents.reserve(Graph<TV, TE>::vertex_count);
		edges.reserve(Graph<TV, TE>::edge_count);
		for (auto el = Graph<TV, TE>::vertexes.begin(); el != Graph<TV, TE>::vertexes.end(); el++) {
			checked[el->second.id] = false;
            dist[el->second.id] = inf();
			parents[el->second.id] = 0;
		}
		dist[from] = TE();
		parents[from] = from;
        for (size_t i = 0; i < Graph<TV, TE>::vertex_count; ++i) {
			size_t v = 0;
			for (auto it = Graph<TV, TE>::vertexes.cbegin(); it != Graph<TV, TE>::vertexes.cend(); it++) {
				if (!checked[it->second.id] && (v == 0 || dist[it->second.id] < dist[v])) {
					v = it->second.id;
				}
			}
            if (dist[v] == inf()) {
				break;
			}
			checked[v] = true;
			for (auto it = Graph<TV, TE>::incidence_list[v].cbegin(); it != Graph<TV, TE>::incidence_list[v].cend(); it++) {
				size_t v1 = 0;
				if (v == it->id_from) {
					v1 = it->id_to;
				}
				else {
					v1 = it->id_from;
				}
				TE len = it->value;
				if (dist[v] + len < dist[v1]) {
					dist[v1] = dist[v] + len;
					parents[v1] = v;
				}
				edges[Pair<size_t, size_t>{v, v1}] = len;
			}
		}
        if (dist[to] == inf()) {
			return res;
		}
		size_t i = to;
		size_t p = parents[i];
		while (p != from) {
			res.push_front(p, i, edges[Pair<size_t, size_t>{p, i}]);
			i = p;
			p = parents[i];
		}
		res.push_front(p, i, edges[Pair<size_t, size_t>{p, i}]);
		return res;
	}
public:
    void erase_edge(size_t from, size_t to, TE _value = TE());
	Path min_path(size_t from, size_t to) {
		if (Graph<TV, TE>::weighed) {
			return dijkstra(from, to);
		}
		return bfs(from, to);
	}
	Path euler_path() {
		Path path;
        std::unordered_map<size_t, std::vector<typename Graph<TV, TE>::Edge>> copy_list = Graph<TV, TE>::incidence_list;
		std::unordered_map<size_t, size_t> degree(Graph<TV, TE>::vertex_count);
		std::vector<size_t> uneven;
        size_t edges = Graph<TV, TE>::edge_count;
        uneven.reserve(3);
		for (auto it = Graph<TV, TE>::incidence_list.cbegin(); it != Graph<TV, TE>::incidence_list.cend(); it++) {
			degree[it->first] = it->second.size();
			if (it->second.size() % 2 != 0) {
                if (uneven.size() == 3) return path;
				uneven.push_back(it->first);
			}
		}
        if (uneven.size() == 2 ) {
           copy_list[uneven[0]].push_back(typename Graph<TV, TE>::Edge(uneven[0], uneven[1], 0));
           copy_list[uneven[1]].push_back(typename Graph<TV, TE>::Edge(uneven[0], uneven[1], 0));
           edges++;
		}
		std::vector<size_t> res;

        res.reserve(edges * 2);
		std::stack<size_t> st;
		st.push(Graph<TV, TE>::vertexes.cbegin()->first);
		while (!st.empty()) {
			size_t v = st.top();
			size_t i = 0;
            for (auto it = copy_list[v].begin(); it != copy_list[v].end(); it++) {
                if (it->id_from == v) {
                    i = it->id_to;
                }
                else {
                    i = it->id_from;
                }
                break;
			}
			if (i == 0) {
				res.push_back(v);
				st.pop();
			}
			else {
                for (auto it = copy_list[v].begin(); it != copy_list[v].end(); ) {
                    if ((it->id_from == i && it->id_to == v) || (it->id_from == v && it->id_to == i)) {
                        it = copy_list[v].erase(it);
                        edges--;
                        break;
                    }
                    else {
                        it++;
                    }
                }
                for (auto it = copy_list[i].begin(); it != copy_list[i].end(); ) {
                    if ((it->id_from == v && it->id_to == i) || (it->id_from == i && it->id_to == v)) {
                        it = copy_list[i].erase(it);
                        break;
                    }
                    else {
                        it++;
                    }
                }
                st.push(i);
			}
		}
        if (uneven.size() == 2) {
            for (size_t i = 0; i + 1 < res.size(); ++i) {
                if ((res[i] == uneven[0] && res[i + 1] == uneven[1]) || (res[i] == uneven[1] && res[i + 1] == uneven[0])) {
					std::vector<size_t> res2;
                    for (size_t j = i + 1; j < res.size(); ++j) {
						res2.push_back(res[j]);
					}
                    for (size_t j = 1; j < i + 1; ++j) {
						res2.push_back(res[j]);
					} 
					res = res2;
					break;
				}
			}
 		}
        if (edges) {
            return path;
		}
        for (size_t i = 0; i + 1 < res.size(); ++i) {
			for (auto it = Graph<TV, TE>::incidence_list[res[i]].cbegin(); it != Graph<TV, TE>::incidence_list[res[i]].cend(); it++) {
                if ((it->id_from == res[i] && it->id_to == res[i + 1]) || (it->id_to == res[i] && it->id_from == res[i + 1])) {
                    path.push_back(res[i], res[i + 1], it->value);
					break;
                }
			}
		}
		return path;
	}
};
template <class TV, class TE>
void UndirectedGraph<TV, TE>::erase_edge(size_t from, size_t to, TE _value) {
    if (Graph<TV, TE>::vertexes.find(from) == Graph<TV, TE>::vertexes.end()) {
        return;
    }
    if (Graph<TV, TE>::vertexes.find(to) == Graph<TV, TE>::vertexes.end()) {
        return;
    }
    for (auto it = Graph<TV, TE>::incidence_list[from].begin(); it != Graph<TV, TE>::incidence_list[from].end(); ) {
        if (((it->id_from == from && it->id_to == to) || (it->id_from == to && it->id_to == from)) && it->value == _value) {
            it = Graph<TV, TE>::incidence_list[from].erase(it);
            Graph<TV, TE>::edge_count--;
            break;
        }
        else {
            it++;
        }
    }
    for (auto it = Graph<TV, TE>::incidence_list[to].begin(); it != Graph<TV, TE>::incidence_list[to].end(); ) {
        if (((it->id_from == from && it->id_to == to) || (it->id_from == to && it->id_to == from)) && it->value == _value) {
            it = Graph<TV, TE>::incidence_list[to].erase(it);
            break;
        }
        else {
            it++;
        }
    }
}
template <class TV, class TE>
void DirectedGraph<TV, TE>::erase_edge(size_t from, size_t to, TE _value) {
    if (Graph<TV, TE>::vertexes.find(from) == Graph<TV, TE>::vertexes.end()) {
        return;
    }
    if (Graph<TV, TE>::vertexes.find(to) == Graph<TV, TE>::vertexes.end()) {
        return;
    }
    for (auto it = Graph<TV, TE>::incidence_list[from].begin(); it != Graph<TV, TE>::incidence_list[from].end(); ) {
        if (it->id_from == from && it->id_to == to && it->value == _value) {
            it = Graph<TV, TE>::incidence_list[from].erase(it);
            Graph<TV, TE>::edge_count--;
            break;
        }
        else {
            it++;
        }
    }
    for (auto it = Graph<TV, TE>::incidence_list[to].begin(); it != Graph<TV, TE>::incidence_list[to].end(); ) {
        if (it->id_from == from && it->id_to == to && it->value == _value) {
            it = Graph<TV, TE>::incidence_list[to].erase(it);
            break;
        }
        else {
            it++;
        }
    }
}
#endif //GRAPH_H_LAB3_SEM3
