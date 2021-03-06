// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2008 Gael Guennebaud <gael.guennebaud@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <unordered_map>
#include <bits/unordered_map.h>
#include "icosphere.h"

using namespace calenhad::icosphere;

    Icosphere::Icosphere (const unsigned int& depth) : _depth (depth), _count (0), _initial (true) {

        //--------------------------------------------------------------------------------
        // icosahedron data
        //--------------------------------------------------------------------------------

        static constexpr double vdata[12][3] = {
                {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
                {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
                {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
        };

        static constexpr uint32_t tindices[20][3] = {
                {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
                {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
                {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
                {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };
        //--------------------------------------------------------------------------------

        _triangles.reserve (depth);
        uint32_t capacity = 10 * pow (4, depth - 1) + 2;
        _vertices.reserve (capacity);

        _gc = new GeographicLib::Geocentric (1, 0);


        // init with an icosahedron
        for (int i = 0; i < 12; i++) {
            Cartesian c;
            c.x = vdata [i][0];
            c.y = vdata [i][1];
            c.z = vdata [i][2];
            addVertex (c, 0);
        }
        _lastVisited = _vertices [0];
        _triangles.push_back (std::vector<Triangle*>());
        _triangles.back();
        for (int i = 0; i < 20; i++) {
            Triangle* t = new Triangle();
            for (int k = 0; k < 3; k++) {
                t -> vertices [k] = _vertices [tindices [i] [k]];
            }
            t -> level = 0;
            t -> parent = nullptr;
            _triangles.back().push_back (t);
        }

        for (int i = 0; i < 20; i++) {
            makeNeighbours (_triangles.back() [i] -> vertices [1], _triangles.back() [i] -> vertices [2]);
            makeNeighbours (_triangles.back() [i] -> vertices [2], _triangles.back() [i] -> vertices [0]);
            makeNeighbours (_triangles.back() [i] -> vertices [1], _triangles.back() [i] -> vertices [0]);

        }

        while(_triangles.size()<_depth) {
            subdivide (_triangles.size());
        }
        _initial = false;
    }

    Icosphere::~Icosphere() {
        delete _gc;
        std::for_each (_triangles.begin(), _triangles.end(), [] (std::vector<Triangle*> list) {
            std::for_each (list.begin(), list.end(), [] (Triangle* t) { delete t; }); });
        std::for_each (_vertices.begin(), _vertices.end(), [] (Vertex* p) { delete p; });
    }

    void Icosphere::subdivide (const unsigned int& level) {
        while (_triangles.size() <= level) {
            _triangles.push_back (std::vector<Triangle*> ());
            _triangles.reserve (_triangles [_triangles.size() - 2].size() * 4);
        }

        for (uint32_t i = 0; i < _triangles [level - 1].size(); i ++) {
            Triangle* t = _triangles [level - 1] [i];
            divideTriangle (t);
        }
    }

    void Icosphere::divideTriangle (Triangle* t) {
        unsigned level = t -> level + 1;
        for (k = 0; k < 3; ++k) {

            ids0[k] = t -> vertices [k];
            e1 = t -> vertices [(k + 1) % 3];

            edgeKey = makeKey (e1 -> id, ids0 [k] -> id);
            it = edgeMap.find (edgeKey);
            if (it == edgeMap.end()) {
                mid (ids0[k]-> cartesian, e1-> cartesian, c);
                ids1[k] = addVertex (c, level);;
                edgeMap[edgeKey] = ids1[k];
            } else {
                ids1[k] = it -> second;
            }
        }

        // The three members of ids0 are the triangles of the three vertices of the parent (outer) triangle
        // The three members of ids1 are the triangles of the three midpoints of the parent triangle's edges

        makeTriangle (ids0[0], ids1[0], ids1[2], t);
        makeTriangle (ids0[1], ids1[1], ids1[0], t);
        makeTriangle (ids0[2], ids1[2], ids1[1], t);
        makeTriangle (ids1[0], ids1[1], ids1[2], t);

        makeNeighbours (ids0[0], ids1[0]);
        makeNeighbours (ids0[0], ids1[2]);

        makeNeighbours (ids0[1], ids1[1]);
        makeNeighbours (ids0[1], ids1[0]);

        makeNeighbours (ids0[2], ids1[2]);
        makeNeighbours (ids0[2], ids1[1]);

        makeNeighbours (ids1[1], ids1[2]);
        makeNeighbours (ids1[0], ids1[2]);
        makeNeighbours (ids1[1], ids1[0]);
    }

    uint64_t Icosphere::makeKey (const uint32_t& v1, const uint32_t& v2) {
        if (v1 > v2) {
            return v1 | (uint64_t (v2) << 32);
        } else {
            return v2 | (uint64_t (v1) << 32);
        }

    }

    void Icosphere::removeTriangle (Triangle* t) {

        // remove child triangles
        if (! t -> children.empty()) {
            std::for_each (t->children.begin (), t->children.end (), [=] (Triangle* t) {
                removeTriangle (t);
            });
        }

        for (int i = 0; i < 3; i++) {

            // remove from edge list
            uint64_t key = makeKey (t -> vertices [i] -> id, t -> vertices [(i + 1) % 3] -> id);
            edgeMap.erase (key);

            // disconnect from neighbours
            t -> vertices [i] -> neighbours.remove (t -> vertices [i]);


            // remove vertex if it has no neighbours left
            if (t -> vertices [i] -> neighbours.empty()) {
                std::vector<Vertex*>::iterator v = std::find (_vertices.begin(), _vertices.end(), t -> vertices [i]);
                if (v != _vertices.end()) {
                    _vertices.erase (v);
                    delete *v;
                }
            }


        }

        // can't remove a triangle below minimum level
        if (t -> level > 3) {

            // remove the triangle we were given
            std::vector<Triangle*>::iterator v = std::find (_triangles[t->level].begin (), _triangles[t->level].end (), t);
            if (v != _triangles[t->level].end ()) {
                _triangles[t->level].erase (v);
                delete t;
            }
        }
    }



    void Icosphere::mid (const Cartesian& c1, const Cartesian& c2, Cartesian& c) {
        c.x = c1.x + c2.x;
        c.y = c1.y + c2.y;
        c.z = c1.z + c2.z;
        double mag = sqrt (c.x * c.x + c.y * c.y + c.z * c.z);
        c.x /= mag;
        c.y /= mag;
        c.z /= mag;
    }

    Vertex* Icosphere::addVertex (const Cartesian& c, int level) {
        Vertex* v = new Vertex();
        v -> id = ++ _count;
        v -> cartesian = c;
        v -> level = level;
        _vertices.push_back (v);
        return v;
    }

    void Icosphere::makeTriangle (Vertex* a, Vertex* b, Vertex* c, Triangle* parent) {

        Triangle* t = new Triangle();
        t -> vertices [0] = a;
        t -> vertices [1] = b;
        t -> vertices [2] = c;
        t -> parent = parent;
        if (parent) {
            parent->children.emplace_front (t);
        }
        t -> level = parent ? parent -> level + 1 : 0;;
        _triangles [t -> level].push_back (t);
    }

    void Icosphere::makeNeighbours (Vertex* p, Vertex* q) {
        if (_initial || std::find (p -> neighbours.begin (), p->neighbours.end(), q) == p -> neighbours.end()) {
            p -> neighbours.emplace_front (q);
            q -> neighbours.emplace_front (p);
        }
    }

    Vertex* Icosphere::operator [] (const uint32_t& id) {
        return _vertices [id];
    }

    Vertex* Icosphere::nearest (const Geolocation& target, const unsigned int& depth)  {
        double d, dist;
        Vertex* pV;
        _lastVisited = _vertices [0];
        for (int i = 1; i < 12; i++) {
            pV = _vertices [i];
            Cartesian c;
            toCartesian (target, c);
            d = distSquared (c, pV -> cartesian);
            if (i == 1 || d < dist) {
                _lastVisited = pV;
                dist = d;
            }
        }

        // walk over the Delaunay triangulation until a point is reached that is nearer the key than any connected point
        return walkTowards (target, depth);
    }

    void Icosphere::visit (Vertex* pV) {
        _lastVisited = pV;
    }

    Vertex* Icosphere::walkTowards (const Geolocation& target, const unsigned int& depth) {
        Cartesian c;
        toCartesian (target, c);
        return walkTowards (c, depth);
    }

    Vertex* Icosphere::walkTowards (const Cartesian& target, const unsigned int& depth) const {
        if (depth == 0 || depth > _depth - 1) { return walkTowards (target, _depth - 1); }
        double dist = distSquared (_lastVisited -> cartesian, target);

        std::forward_list<Vertex*>::const_iterator i  = _lastVisited -> neighbours.begin();
        Vertex* next;

        bool found = false;
        while (i != _lastVisited -> neighbours.end()) {
            if ((*i) -> level <= depth) {
                double d = distSquared ((*i) -> cartesian, target);
                if (d < dist && (*i) -> level >= (_lastVisited -> level)) {
                    next = *i;
                    dist = d;
                    found = true;
                }
            }
            i++;
        }

        if (found) {
            _lastVisited = next;
            return walkTowards (target, depth);
        } else {
            return _lastVisited;
        }
    }

    double Icosphere::distSquared (const Cartesian& a, const Cartesian& b) {
        double dx = fabs (a.x - b.x);
        double dy = fabs (a.y - b.y);
        double dz = fabs (a.z - b.z);
        double dist = dx * dx + dy * dy + dz * dz;
        return dist;
    }

    void Icosphere::toGeolocation (const Cartesian& c, Geolocation& g) {
        _gc -> Reverse (c.x, c.y, c.z, g.lat, g.lon, g.height);
    }

    Cartesian Icosphere::toCartesian (const Geolocation& g, Cartesian& c) {
        _gc -> Forward (g.lat, g.lon, 0, c.x, c.y, c.z);

    }

    std::pair<std::vector<Triangle*>::iterator, std::vector<Triangle*>::iterator> Icosphere::triangles (const unsigned& level) {
        return std::make_pair (_triangles [level].begin(), _triangles [level].end());
    }

    std::pair<std::vector<Vertex*>::iterator, std::vector<Vertex*>::iterator> Icosphere::vertices() {
        return std::make_pair (_vertices .begin(), _vertices.end());
    }

    unsigned long Icosphere::vertexCount () {
        return _vertices.size();
    }



