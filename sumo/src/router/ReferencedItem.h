#ifndef ReferencedItem_h
#define ReferencedItem_h
//---------------------------------------------------------------------------//
//                        ReferencedItem.h -
//  Something that holds the number of references to it
//      (destructor not yet as sophisticated as should be)
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
// $Log$
// Revision 1.3  2004/01/26 08:01:21  dkrajzew
// loaders and route-def types are now renamed in an senseful way; further changes in order to make both new routers work; documentation added
//
// Revision 1.2  2003/02/07 10:45:07  dkrajzew
// updated
//
/* =========================================================================
 * included modules
 * ======================================================================= */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H


/* =========================================================================
 * class definitions
 * ======================================================================= */
/**
 * @class ReferencedItem
 * Basic class for structures which are referenced by other classes. The wish
 * is to have an information whether an instance of a derived class was already
 * saved (together with a referencing class) or shall be saved this time.
 */
class ReferencedItem {
public:
    /// Constructor
    ReferencedItem() : _wasSaved(false) { }

    /// Destructor
    virtual ~ReferencedItem() { }

    /// Returns the information whether this item was already saved
    bool isSaved() const { return _wasSaved; }

    /// Marks the item as saved
    void markSaved() { _wasSaved = true; }

private:
    /// Information whether the item was already saved
    bool _wasSaved;

};


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

#endif

// Local Variables:
// mode:C++
// End:

