//---------------------------------------------------------------------------//
//                        Position2DVector.cpp -
//  A list of 2D-positions
//                           -------------------
//  project              : SUMO - Simulation of Urban MObility
//  begin                : Sept 2002
//  copyright            : (C) 2002 by Daniel Krajzewicz
//  organisation         : IVF/DLR http://ivf.dlr.de
//  email                : Daniel.Krajzewicz@dlr.de
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//---------------------------------------------------------------------------//
namespace
{
    const char rcsid[] =
    "$Id$";
}
// $Log$
// Revision 1.24  2004/04/02 11:29:02  dkrajzew
// computation of moving a line strip to the side patched (is still not correct to 100%)
//
// Revision 1.23  2004/03/19 13:01:11  dkrajzew
// methods needed for the new selection within the gui added; some style adaptions
//
// Revision 1.22  2004/02/16 14:00:00  dkrajzew
// some further work on edge geometry
//
// Revision 1.21  2004/01/12 15:57:21  dkrajzew
// catching the case of not crossing lines on concatanation
//
// Revision 1.20  2003/12/09 11:33:49  dkrajzew
// made the assignment operator and copy constructor explicite in the wish to save memory
//
// Revision 1.19  2003/11/18 14:21:20  dkrajzew
// computation of junction-inlanes geometry added
//
// Revision 1.18  2003/11/11 08:01:24  dkrajzew
// some further methods implemented
//
// Revision 1.17  2003/10/21 14:39:11  dkrajzew
// the rotation information now returns the last valid value if the length is exceeded
//
// Revision 1.16  2003/10/17 06:50:02  dkrajzew
// patched the false usage of a reference
//
// Revision 1.15  2003/10/15 11:56:30  dkrajzew
// further work on vissim-import
//
// Revision 1.14  2003/10/02 14:55:56  dkrajzew
// visualisation of E2-detectors implemented
//
// Revision 1.13  2003/09/25 09:03:53  dkrajzew
// some methods added, needed for the computation of line rotation
//
// Revision 1.12  2003/09/05 15:27:38  dkrajzew
// changes from adding internal lanes and further work on node geometry
//
// Revision 1.11  2003/08/20 11:47:38  dkrajzew
// bug in sorting the values by x, then y patched
//
// Revision 1.10  2003/08/14 14:05:50  dkrajzew
// functions to process a nodes geometry added
//
// Revision 1.9  2003/07/16 15:38:04  dkrajzew
// some work on computation and handling of geometry information
//
// Revision 1.8  2003/07/07 08:48:35  dkrajzew
// remved an unneeded debug-output
//
// Revision 1.7  2003/06/05 14:33:45  dkrajzew
// class templates applied; documentation added
//
//
/* =========================================================================
 * included modules
 * ======================================================================= */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <queue>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <cassert>
#include "AbstractPoly.h"
#include "Position2D.h"
#include "Position2DVector.h"
#include "GeomHelper.h"
#include "Line2D.h"


/* =========================================================================
 * used namespaces
 * ======================================================================= */
using namespace std;


/* =========================================================================
 * method definitions
 * ======================================================================= */
Position2DVector::Position2DVector()
{
}


Position2DVector::Position2DVector(size_t fieldSize)
    : myCont(fieldSize)
{
}


Position2DVector::~Position2DVector()
{
}


void
Position2DVector::push_back(const Position2D &p)
{
    myCont.push_back(p);
}


void
Position2DVector::push_back(const Position2DVector &p)
{
    copy(p.myCont.begin(), p.myCont.end(), back_inserter(myCont));
}


void
Position2DVector::push_front(const Position2D &p)
{
    myCont.push_front(p);
}


bool
Position2DVector::around(const Position2D &p, double offset) const
{
    if(offset!=0) {
        //throw 1; // !!! not yet implemented
    }
    double angle=0;
    for (ContType::const_iterator i=myCont.begin(); i!=myCont.end()-1; i++) {
       Position2D p1(
           (*i).x() - p.x(),
           (*i).y() - p.y());
       Position2D p2(
           (*(i+1)).x() - p.x(),
           (*(i+1)).y() - p.y());
       angle += GeomHelper::Angle2D(p1.x(), p1.y(), p2.x(), p2.y());
    }
    Position2D p1(
        (*(myCont.end()-1)).x() - p.x(),
        (*(myCont.end()-1)).y() - p.y());
    Position2D p2(
        (*(myCont.begin())).x() - p.x(),
        (*(myCont.begin())).y() - p.y());
    angle += GeomHelper::Angle2D(p1.x(), p1.y(), p2.x(), p2.y());
    return (!(fabs(angle) < 3.1415926535897932384626433832795));
}


bool
Position2DVector::overlapsWith(const AbstractPoly &poly, double offset) const
{
    for(ContType::const_iterator i=myCont.begin(); i!=myCont.end()-1; i++) {
        if(poly.around(*i, offset)) {
            return true;
        }
    }
    return false;
}


bool
Position2DVector::intersects(const Position2D &p1, const Position2D &p2) const
{
    if(size()<2) {
        return false;
    }
    for(ContType::const_iterator i=myCont.begin(); i!=myCont.end()-1; i++) {
        if(GeomHelper::intersects(*i, *(i+1), p1, p2)) {
            return true;
        }
    }
    //return GeomHelper::intersects(*(myCont.end()-1), *(myCont.begin()), p1, p2);
    return false;
}


bool
Position2DVector::intersects(const Position2DVector &v1) const
{
    for(ContType::const_iterator i=myCont.begin(); i!=myCont.end()-1; i++) {
        if(v1.intersects(*i, *(i+1))) {
            return true;
        }
    }
    //return v1.intersects(*(myCont.end()-1), *(myCont.begin()));
    return false;
}


Position2D
Position2DVector::intersectsAtPoint(const Position2D &p1,
                                    const Position2D &p2) const
{
    for(ContType::const_iterator i=myCont.begin(); i!=myCont.end()-1; i++) {
        if(GeomHelper::intersects(*i, *(i+1), p1, p2)) {
            return GeomHelper::intersection_position(*i, *(i+1), p1, p2);
        }
    }
    /*
    if(GeomHelper::intersects(*(myCont.end()-1), *(myCont.begin()), p1, p2)) {
        return GeomHelper::intersection_position(
            *(myCont.end()-1), *(myCont.begin()), p1, p2);
    }
    */
    return Position2D(-1, -1);
}


Position2D
Position2DVector::intersectsAtPoint(const Position2DVector &v1) const
{
    for(ContType::const_iterator i=myCont.begin(); i!=myCont.end()-1; i++) {
        if(v1.intersects(*i, *(i+1))) {
            return v1.intersectsAtPoint(*i, *(i+1));
        }
    }
    /*
    if(v1.intersects(*(myCont.end()-1), *(myCont.begin()))) {
        return v1.intersectsAtPoint(*(myCont.end()-1), *(myCont.begin()));
    }
    */
    return Position2D(-1, -1);
}


void
Position2DVector::clear()
{
    myCont.clear();
}


const Position2D &
Position2DVector::at(size_t i) const
{
    return myCont[i];
}


size_t
Position2DVector::size() const
{
    return myCont.size();
}



Position2D
Position2DVector::positionAtLengthPosition(double pos) const
{
    ContType::const_iterator i=myCont.begin();
    double seenLength = 0;
    do {
        double nextLength = GeomHelper::distance(*i, *(i+1));
        if(seenLength+nextLength>pos) {
            return positionAtLengthPosition(*i, *(i+1), pos-seenLength);
        }
        seenLength += nextLength;
    } while(++i!=myCont.end()-1);
    return myCont[myCont.size()-1];
    assert(false);
//    return positionAtLengthPosition(*(myCont.end()-1),
//        *(myCont.begin()), pos-seenLength);
}


double
Position2DVector::rotationDegreeAtLengthPosition(double pos) const
{
    ContType::const_iterator i=myCont.begin();
    double seenLength = 0;
    do {
        double nextLength = GeomHelper::distance(*i, *(i+1));
        if(seenLength+nextLength>pos) {
            Line2D l(*i, *(i+1));
            return l.atan2DegreeAngle();
        }
        seenLength += nextLength;
    } while(++i!=myCont.end()-1);
    Line2D l(*(myCont.end()-2), *(myCont.end()-1));
    return l.atan2DegreeAngle();
//    Line2D l(*(myCont.end()-1), *(myCont.begin()));
//    return l.atan2DegreeAngle();
}


Position2D
Position2DVector::positionAtLengthPosition(const Position2D &p1,
                                           const Position2D &p2,
                                           double pos)
{
    double dist = GeomHelper::distance(p1, p2);
    if(dist<pos) {
        return Position2D(-1, -1);
    }
    double x = p1.x() + (p2.x() - p1.x()) / dist * pos;
    double y = p1.y() + (p2.y() - p1.y()) / dist * pos;
    return Position2D(x, y);
}


Boundery
Position2DVector::getBoxBoundery() const
{
    Boundery ret;
    for(ContType::const_iterator i=myCont.begin(); i!=myCont.end(); i++) {
        ret.add(*i);
    }
    return ret;
}


Position2D
Position2DVector::center() const
{
    double x = 0;
    double y = 0;
    for(ContType::const_iterator i=myCont.begin(); i!=myCont.end(); i++) {
        x += (*i).x();
        y += (*i).y();
    }
    return Position2D(x/(double) myCont.size(), y/(double) myCont.size());
}


double
Position2DVector::length() const
{
    double len = 0;
    for(ContType::const_iterator i=myCont.begin(); i!=myCont.end()-1; i++) {
        len += GeomHelper::distance(*i, *(i+1));
    }
    return len;
}


bool
Position2DVector::partialWithin(const AbstractPoly &poly, double offset) const
{
    for(ContType::const_iterator i=myCont.begin(); i!=myCont.end()-1; i++) {
        if(poly.around(*i, offset)) {
            return true;
        }
    }
    return false;
}



bool
Position2DVector::crosses(const Position2D &p1, const Position2D &p2) const
{
    return intersects(p1, p2);
}



const Position2D &
Position2DVector::getBegin() const
{
    return myCont[0];
}


const Position2D &
Position2DVector::getEnd() const
{
    return myCont[myCont.size()-1];
}


std::pair<Position2DVector, Position2DVector>
Position2DVector::splitAt(double where) const
{
    Position2DVector one;
    double tmp = 0;
    ContType::const_iterator i=myCont.begin();
    Position2D last = myCont[0];
    for(; i!=myCont.end()&&tmp<where; i++) {
        Position2D curr = (*i);
        if(i!=myCont.begin()) {
            tmp += GeomHelper::distance(last, curr);
        }
        if(tmp<where) {
            one.push_back(*i);
        }
        last = curr;
    }
    Position2DVector two;
    two.push_back(last);
    for(; i!=myCont.end(); i++) {
        two.push_back(*i);
    }
    return std::pair<Position2DVector, Position2DVector>(one, two);
}



std::ostream &
operator<<(std::ostream &os, const Position2DVector &geom)
{
    for(Position2DVector::ContType::const_iterator i=geom.myCont.begin(); i!=geom.myCont.end(); i++) {
        if(i!=geom.myCont.begin()) {
            os << " ";
        }
        os << (*i).x() << "," << (*i).y();
    }
    return os;
}


Position2DVector
Position2DVector::resettedBy(double x, double y) const
{
    Position2DVector ret;
    for(size_t i=0; i<size(); i++) {
        Position2D pos = at(i);
        pos.add(x, y);
        ret.push_back(pos);
    }
    return ret;
}


void
Position2DVector::resetBy(double x, double y)
{
    for(size_t i=0; i<size(); i++) {
        myCont[i].add(x, y);
    }
}


void
Position2DVector::resetBy(const Position2D &by)
{
    for(size_t i=0; i<size(); i++) {
        myCont[i].add(by.x(), by.y());
    }
}


void
Position2DVector::sortAsPolyCWByAngle()
{
    Position2D c = center();
    std::sort(myCont.begin(), myCont.end(), as_poly_cw_sorter(c));
}


void
Position2DVector::reshiftRotate(double xoff, double yoff, double rot)
{
    for(size_t i=0; i<size(); i++) {
        myCont[i].reshiftRotate(xoff, yoff, rot);
    }
}


Position2DVector::as_poly_cw_sorter::as_poly_cw_sorter(Position2D center)
    : _center(center)
{
}



int
Position2DVector::as_poly_cw_sorter::operator() (const Position2D &p1,
                                                 const Position2D &p2) const
{
    return atan2(p1.x(), p1.y()) < atan2(p2.x(), p2.y());
}



void
Position2DVector::sortByIncreasingXY()
{
    std::sort(myCont.begin(), myCont.end(), increasing_x_y_sorter());
}



Position2DVector::increasing_x_y_sorter::increasing_x_y_sorter()
{
}



int
Position2DVector::increasing_x_y_sorter::operator() (const Position2D &p1,
                                                 const Position2D &p2) const
{
    if(p1.x()!=p2.x()) {
        return p1.x()<p2.x();
    }
    return p1.y()<p2.y();
}



float
Position2DVector::isLeft(const Position2D &P0, const Position2D &P1,
                         const Position2D &P2 ) const
{
    return (P1.x() - P0.x())*(P2.y() - P0.y()) - (P2.x() - P0.x())*(P1.y() - P0.y());
}


// Copyright 2001, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.
// Assume that a class is already given for the object:
// Point with coordinates {float x, y;}
//===================================================================
// isLeft(): tests if a point is Left|On|Right of an infinite line.
// Input: three points P0, P1, and P2
// Return: >0 for P2 left of the line through P0 and P1
// =0 for P2 on the line
// <0 for P2 right of the line
// See: the January 2001 Algorithm on Area of Triangles
Position2DVector
Position2DVector::convexHull() const
{
    if(size()==0) {
        return Position2DVector();
    }
    // build the sorted input point-list
    // copy
    Position2DVector inp;
    inp.myCont = myCont;
    // sort by increasing x, then y
    inp.sortByIncreasingXY();
    int n = inp.size();

    // convex-hull algo
    Position2DVector ret(n);
    // the output ret will be used as the stack
    int bot=0, top=(-1); // indices for bottom and top of the stack
    int i; // array scan index

    // Get the indices of points with min x-coord and min|max y-coord
    int minmin = 0, minmax;
    float xmin = inp.at(0).x();
    for (i=1; i<n; i++)
        if (inp.at(i).x() != xmin)
            break;
    minmax = i-1;

    if (minmax == n-1) { // degenerate case: all x-coords == xmin
        ret.set(++top, inp.at(minmin));
        if (inp.at(minmax).y() != inp.at(minmin).y()) // a nontrivial segment
            ret.set(++top, inp.at(minmax));
        ret.set(++top, inp.at(minmin)); // add polygon endpoint
        return top+1;
    }

    // Get the indices of points with max x-coord and min|max y-coord
    int maxmin, maxmax = n-1;
    float xmax = inp.at(n-1).x();
    for (i=n-2; i>=0; i--)
        if (inp.at(i).x() != xmax)
            break;
    maxmin = i+1;

    // Compute the lower hull on the stack H
    ret.set(++top, inp.at(minmin)); // push minmin point onto stack
    i = minmax;
    while (++i <= maxmin)
    {
        // the lower line joins P[minmin] with P[maxmin]
        if (isLeft( inp.at(minmin), inp.at(maxmin), inp.at(i)) >= 0 && i < maxmin)
            continue; // ignore P[i] above or on the lower line

        while (top > 0) // there are at least 2 points on the stack
        {
            // test if P[i] is left of the line at the stack top
            if (isLeft( ret.at(top-1), ret.at(top), inp.at(i)) > 0)
                break; // P[i] is a new hull vertex
            else
                top--; // pop top point off stack
        }
        ret.set(++top, inp.at(i)); // push P[i] onto stack
    }

    // Next, compute the upper hull on the stack H above the bottom hull
    if (maxmax != maxmin) // if distinct xmax points
        ret.set(++top, inp.at(maxmax)); // push maxmax point onto stack
    bot = top; // the bottom point of the upper hull stack
    i = maxmin;
    while (--i >= minmax)
    {
        // the upper line joins P[maxmax] with P[minmax]
        if (isLeft( inp.at(maxmax), inp.at(minmax), inp.at(i)) >= 0 && i > minmax)
            continue; // ignore P[i] below or on the upper line

        while (top > bot) // at least 2 points on the upper stack
        {
            // test if P[i] is left of the line at the stack top
            if (isLeft( ret.at(top-1), ret.at(top), inp.at(i)) > 0)
                break; // P[i] is a new hull vertex
            else
                top--; // pop top point off stack
        }
        ret.set(++top, inp.at(i)); // push P[i] onto stack
    }
    if (minmax != minmin)
        ret.set(++top, inp.at(minmin)); // push joining endpoint onto stack

    Position2DVector rret;
    for(size_t j=0; j<top; j++) {
        rret.push_back(ret.at(j));
    }
    return rret;
}


void
Position2DVector::set(size_t pos, const Position2D &p)
{
    myCont[pos] = p;
}



Position2DVector
Position2DVector::intersectsAtPoints(const Position2D &p1,
                                     const Position2D &p2) const
{
    Position2DVector ret;
    for(ContType::const_iterator i=myCont.begin(); i!=myCont.end()-1; i++) {
        if(GeomHelper::intersects(*i, *(i+1), p1, p2)) {
            ret.push_back(GeomHelper::intersection_position(*i, *(i+1), p1, p2));
        }
    }
    /*
    if(GeomHelper::intersects(*(myCont.end()-1), *(myCont.begin()), p1, p2)) {
        ret.push_back(GeomHelper::intersection_position(
            *(myCont.end()-1), *(myCont.begin()), p1, p2));
    }
    */
    return ret;
}


void
Position2DVector::appendWithCrossingPoint(const Position2DVector &v)
{
    //
    Line2D l1(myCont[myCont.size()-2], myCont[myCont.size()-1]);
    l1.extrapolateBy(100);
    Line2D l2(v.myCont[0], v.myCont[1]);
    l2.extrapolateBy(100);
    if(l1.intersects(l2)&&l1.intersectsAtLength(l2)<10) { // !!! heuristic
        Position2D p = l1.intersectsAt(l2);
        myCont[myCont.size()-1] = p;
        copy(v.myCont.begin()+1, v.myCont.end(), back_inserter(myCont));
    } else {
        copy(v.myCont.begin(), v.myCont.end(), back_inserter(myCont));
    }
}


Position2DVector
Position2DVector::getSubpart(double begin, double end) const
{
	Position2DVector ret;
	Position2D begPos = positionAtLengthPosition(begin);
    Position2D endPos = myCont[myCont.size()-1];
    if(length()>end) {
	    endPos = positionAtLengthPosition(end);
    }
	ret.push_back(begPos);

	double seen = 0;
	ContType::const_iterator i = myCont.begin();
	// skip previous segments
	while((i+1)!=myCont.end()
		&&
		seen+GeomHelper::distance((*i), *(i+1))<begin) {
		seen += GeomHelper::distance((*i), *(i+1));
		i++;
	}
	// append segments in between
	while((i+1)!=myCont.end()
		&&
		seen+GeomHelper::distance((*i), *(i+1))<end) {
		ret.push_back(*(i+1));
		seen += GeomHelper::distance((*i), *(i+1));
		i++;
	}
	// append end
	ret.push_back(endPos);
	return ret;
}


void
Position2DVector::pruneFromBeginAt(const Position2D &p)
{
/*    ContType::reverse_iterator i=myCont.rbegin();
    const Position2D &cp = (*i);
    double dist = GeomHelper::distance(p, cp);
    double cdist = dist;
    size_t pos = 0;
    do {
        const Position2D &cp2 = *(++i);
        cdist = GeomHelper::distance(p, cp2);
        pos++;
    } while(cdist<dist&&i!=myCont.rend());
    if(i==myCont.rend()) {
        return;
    }
    while(myCont.size()>pos-1) {
        myCont.erase(myCont.begin());
    }
    assert(myCont.size()>=2);*/

    // find minimum distance (from the begin)
    size_t pos = 0;
    double dist = 1000000;
    size_t currPos = 0;
    double currDist = GeomHelper::DistancePointLine(p,
        GeomHelper::extrapolate_first(*(myCont.begin()), *(myCont.begin()+1), 100),
        *(myCont.begin()+1));
//    assert(currDist>=0);
    if(currDist>=0&&currDist<dist) {
        dist = currDist;
        pos = currPos;
    }

    for(ContType::iterator i=myCont.begin(); i!=myCont.end()-1; i++, currPos++) {
        double currDist = GeomHelper::DistancePointLine(p, *i, *(i+1));
        if(currDist>=0&&currDist<dist) {
            dist = currDist;
            pos = currPos;
        }
    }
    // remove leading items
    for(size_t j=0; j<pos; j++) {
        myCont.erase(myCont.begin());
    }
    // replace first item by the new position
    double lpos = GeomHelper::nearest_position_on_line_to_point(
        myCont[0], myCont[1], p);
    if(lpos==-1) {
        return;
    }
    Position2D np = positionAtLengthPosition(lpos);
    if(np!=*(myCont.begin())) {
        myCont.erase(myCont.begin());
        if(np!=*(myCont.begin())) {
            myCont.push_front(np);
            assert(myCont.size()>1);
            assert(*(myCont.begin())!=*(myCont.end()-1));
        }
    }
}


void
Position2DVector::pruneFromEndAt(const Position2D &p)
{
    // find minimum distance (from the end)
    size_t pos = 0;
    double dist = 1000000;
    size_t currPos = 0;
    double currDist = GeomHelper::DistancePointLine(p,
        *(myCont.end()-1),
        GeomHelper::extrapolate_second(*(myCont.end()-2), *(myCont.end()-1), 100));
//    assert(currDist>=0);
    if(currDist>=0&&currDist<dist) {
        dist = currDist;
        pos = currPos;
    }

    for(ContType::reverse_iterator i=myCont.rbegin(); i!=myCont.rend()-1; i++, currPos++) {
        double currDist = GeomHelper::DistancePointLine(p, *(i), *(i+1));
        if(currDist>=0&&currDist<dist) {
            dist = currDist;
            pos = currPos;
        }
    }
    // remove trailing items
    for(size_t j=0; j<pos; j++) {
        myCont.erase(myCont.end()-1);
    }
    // replace last item by the new position
    double lpos =
        GeomHelper::nearest_position_on_line_to_point(
            myCont[myCont.size()-1], myCont[myCont.size()-2], p);
    if(lpos==-1) {
        return;
    }
    Position2D np = positionAtLengthPosition(
        length() - lpos);
    if(np!=*(myCont.end()-1)) {
        myCont.erase(myCont.end()-1);
        if(np!=*(myCont.end()-1)) {
            myCont.push_back(np);
            assert(myCont.size()>1);
            assert(*(myCont.begin())!=*(myCont.end()-1));
        }
    }
}


double
Position2DVector::beginEndAngle() const
{
    Line2D tmp(getBegin(), getEnd());
    return tmp.atan2Angle();
}


void
Position2DVector::eraseAt(size_t i)
{
    myCont.erase(myCont.begin()+i);
}


double
Position2DVector::nearest_position_on_line_to_point(const Position2D &p) const
{
    double shortestDist = 10000000;
    double nearestPos = 10000;
    double seen = 0;
    for(ContType::const_iterator i=myCont.begin(); i!=myCont.end()-1; i++) {
        double pos = seen +
            GeomHelper::nearest_position_on_line_to_point(*i, *(i+1), p);
        double dist =
            GeomHelper::distance(p, positionAtLengthPosition(pos));
        //
        if(shortestDist<dist) {
            nearestPos = pos;
            shortestDist = dist;
        }
        //
    }
    return nearestPos;
}


double
Position2DVector::distance(const Position2D &p) const
{
    double shortestDist = 10000000;
    double nearestPos = 10000;
    double seen = 0;
    for(ContType::const_iterator i=myCont.begin(); i!=myCont.end()-1; i++) {
        double pos = seen +
            GeomHelper::nearest_position_on_line_to_point(*i, *(i+1), p);
        double dist =
            GeomHelper::distance(p, positionAtLengthPosition(pos));
        //
        if(shortestDist>dist) {
            nearestPos = pos;
            shortestDist = dist;
        }
        //
    }
    if(shortestDist==10000000) {
        return -1;
    }
    return shortestDist;
}

DoubleVector
Position2DVector::intersectsAtLengths(const Position2DVector &s) const
{
    DoubleVector ret;
    double pos = 0;
    for(ContType::const_iterator i=myCont.begin(); i!=myCont.end()-1; i++) {
        Line2D l((*i), *(i+1));
        DoubleVector atSegment = l.intersectsAtLengths(s);
        DoubleVectorHelper::add2All(atSegment, pos);
        copy(atSegment.begin(), atSegment.end(), back_inserter(ret));
        pos += l.length();
    }
    return ret;
}

DoubleVector
Position2DVector::intersectsAtLengths(const Line2D &s) const
{
    DoubleVector ret;
    double pos = 0;
    for(ContType::const_iterator i=myCont.begin(); i!=myCont.end()-1; i++) {
        Line2D l((*i), *(i+1));
        if(GeomHelper::intersects(l.p1(), l.p2(), s.p1(), s.p2())) {
            Position2D p =
                GeomHelper::intersection_position(l.p1(), l.p2(), s.p1(), s.p2());
            double atLength = GeomHelper::distance(l.p1(), p);
            ret.push_back(atLength+pos);
        }
        pos += l.length();
    }
    return ret;
}


void
Position2DVector::extrapolate(double val)
{
    assert(myCont.size()>1);
    Position2D nb =
        GeomHelper::extrapolate_first(myCont[0], myCont[1], val);
    Position2D ne =
        GeomHelper::extrapolate_second(
            myCont[myCont.size()-2], myCont[myCont.size()-1], val);
    myCont.erase(myCont.begin());
    push_front(nb);
    myCont.erase(myCont.end()-1);
    push_back(ne);
}


Position2DVector
Position2DVector::reverse() const
{
    Position2DVector ret;
    for(ContType::const_reverse_iterator i=myCont.rbegin(); i!=myCont.rend(); i++) {
        ret.push_back(*i);
    }
    return ret;
}


void
Position2DVector::move2side(double amount)
{
    if(myCont.size()<2) {
        return;
    }
    ContType newCont;
    std::pair<double, double> p;
    Position2D newPos;
    // first point
    newPos = (*(myCont.begin()));
    p = GeomHelper::getNormal90D_CW(*(myCont.begin()), *(myCont.begin()+1), amount);
    newPos.add(p.first, p.second);
    newCont.push_back(newPos);
    // middle points
    for(ContType::const_iterator i=myCont.begin()+1; i!=myCont.end()-1; i++) {
        std::pair<double, double> oldp = p;
        newPos = *i;
        newPos.add(p.first, p.second);
        newCont.push_back(newPos);
        p = GeomHelper::getNormal90D_CW(*i, *(i+1), amount);
//        Position2D newPos(*i);
//        newPos.add((p.first+oldp.first)/2.0, (p.second+oldp.second)/2.0);
//        newCont.push_back(newPos);
    }
    // last point
    newPos = (*(myCont.end()-1));
    newPos.add(p.first, p.second);
    newCont.push_back(newPos);
    myCont = newCont;
}


Line2D
Position2DVector::lineAt(size_t pos) const
{
    assert(myCont.size()>pos);
    return Line2D(myCont[pos], myCont[pos+1]);
}


Line2D
Position2DVector::getBegLine() const
{
    return lineAt(0);
}


Line2D
Position2DVector::getEndLine() const
{
    return lineAt(myCont.size()-2);
}


void
Position2DVector::closePolygon()
{
    push_back(myCont[0]);
}


Position2DVector::Position2DVector(const Position2DVector &s)
{
    myCont.assign(s.myCont.begin(), s.myCont.end());
}


Position2DVector &
Position2DVector::operator=(const Position2DVector &s)
{
    myCont.assign(s.myCont.begin(), s.myCont.end());
    return *this;
}


DoubleVector
Position2DVector::distances(const Position2DVector &s) const
{
    DoubleVector ret;
    ContType::const_iterator i;
    for(i=myCont.begin(); i!=myCont.end(); i++) {
        double dist = s.distance(*i);
        // !!! aeh, possible at the ends?
        if(dist!=-1) {
            ret.push_back(dist);
        }
    }
    for(i=s.myCont.begin(); i!=s.myCont.end(); i++) {
        double dist = distance(*i);
        // !!! aeh, possible at the ends?
        if(dist!=-1) {
            ret.push_back(dist);
        }
    }
    return ret;
}


DoubleVector
Position2DVector::distancesExt(const Position2DVector &s) const
{
    DoubleVector ret = distances(s);
    ContType::const_iterator i;
    for(i=myCont.begin(); i!=myCont.end()-1; i++) {
        Position2D p = Position2D(*i);
        p.add(*(i+1));
        p.mul(0.5);
        double dist = s.distance(p);
        // !!! aeh, possible at the ends?
        if(dist!=-1) {
            ret.push_back(dist);
        }
    }
    for(i=s.myCont.begin(); i!=s.myCont.end()-1; i++) {
        Position2D p = Position2D(*i);
        p.add(*(i+1));
        p.mul(0.5);
        double dist = s.distance(p);
        // !!! aeh, possible at the ends?
        if(dist!=-1) {
            ret.push_back(dist);
        }
    }
    return ret;
}

/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

// Local Variables:
// mode:C++
// End:


