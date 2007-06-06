/****************************************************************************/
/// @file    NIVisumParser_Phases.h
/// @author  Daniel Krajzewicz
/// @date    Fri, 09 May 2003
/// @version $Id$
///
// missing_desc
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// copyright : (C) 2001-2007
//  by DLR (http://www.dlr.de/) and ZAIK (http://www.zaik.uni-koeln.de/AFS)
/****************************************************************************/
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/
#ifndef NIVisumParser_Phases_h
#define NIVisumParser_Phases_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include "NIVisumLoader.h"


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class NIVisumParser_Phases
 * This class parses Phases from visum-files.
 */
class NIVisumParser_Phases :
            public NIVisumLoader::NIVisumSingleDataTypeParser
{
public:
    /// Constructor
    NIVisumParser_Phases(NIVisumLoader &parent,
                         const std::string &dataName, NIVisumLoader::NIVisumTL_Map &NIVisumTLs);

    /// Destructor
    ~NIVisumParser_Phases();

protected:
    /** @brief Parses a single node using data from the inherited NamedColumnsParser. */
    void myDependentReport();
private:
    NIVisumLoader::NIVisumTL_Map &myNIVisumTLs;
};


#endif

/****************************************************************************/

